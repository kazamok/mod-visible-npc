#include "ScriptMgr.h"
#include "Config.h"
#include "World.h" // sWorld 사용을 위해 포함
#include "Log.h"   // LOG_INFO, LOG_ERROR 사용을 위해 포함
#include "DatabaseEnv.h" // WorldDatabase.Execute() 사용을 위해 포함
#include <set>
#include <string>
#include <sstream>
#include <vector>

// 모듈의 설정을 관리하는 싱글톤 클래스
class ModVisibleNpcConfig
{
public:
    // 싱글톤 인스턴스를 반환합니다.
    static ModVisibleNpcConfig* instance()
    {
        static ModVisibleNpcConfig instance;
        return &instance;
    }

    // 설정 파일(worldserver.conf)에서 설정을 로드하고 DB 작업을 수행합니다.
    void Load()
    {
        // "VisibleNPC.Enable" 값을 읽어 모듈 활성화 여부를 결정합니다.
        bool isEnabled = sConfigMgr->GetOption<bool>("VisibleNPC.Enable", false);

        // 숨길 NPC ID 목록을 설정 파일에서 로드합니다.
        std::set<uint32> hiddenEntriesFromConfig;
        LoadEntriesFromConfig("VisibleNPC.HiddenEntries.QuelDanas_Scourge", hiddenEntriesFromConfig);
        LoadEntriesFromConfig("VisibleNPC.HiddenEntries.QuelDanas_ShatteredSun", hiddenEntriesFromConfig);
        LoadEntriesFromConfig("VisibleNPC.HiddenEntries.QuelDanas_Dawnblade", hiddenEntriesFromConfig);
        LoadEntriesFromConfig("VisibleNPC.HiddenEntries.QuelDanas_Misc", hiddenEntriesFromConfig);
        LoadEntriesFromConfig("VisibleNPC.HiddenEntries.Shattrath", hiddenEntriesFromConfig); // 예시용 샤트라스 그룹

        // 숨길 NPC ID 목록이 비어있으면 아무 작업도 하지 않습니다.
        if (hiddenEntriesFromConfig.empty())
        {
            LOG_INFO("mod.visible_npc", "mod-visible-npc: 숨길 NPC ID가 설정되지 않았습니다. 모듈이 비활성화됩니다.");
            // 모듈이 비활성화 상태이고, 숨길 ID도 없다면, 복원 로직을 실행하여 혹시 모를 잔여 NPC를 복원합니다.
            // 이는 이전에 숨겨졌던 NPC가 있다면 복원하기 위함입니다.
            if (!isEnabled)
            {
                RestoreNpcs();
            }
            return;
        }

        // 숨길 NPC ID 목록을 SQL 쿼리용 문자열로 변환
        std::string hiddenIdsStr;
        for (uint32 entryId : hiddenEntriesFromConfig)
        {
            if (!hiddenIdsStr.empty())
                hiddenIdsStr += ",";
            hiddenIdsStr += std::to_string(entryId);
        }

        if (isEnabled)
        {
            // 모듈 활성화 시: NPC 숨기기 로직
            LOG_INFO("mod.visible_npc", "mod-visible-npc: 모듈 활성화. NPC 숨김 작업을 시작합니다.");

            // 1. 백업 테이블에 원본 spawnMask 백업
            // creature 테이블에서 숨길 NPC들의 현재 spawnMask를 조회하여 백업 테이블에 삽입
            // 이미 백업된 NPC는 무시 (PRIMARY KEY 충돌 방지)
            std::string backupQuery = "INSERT IGNORE INTO mod_visible_npc_backup (entry, original_spawnmask) SELECT id1, spawnMask FROM creature WHERE id1 IN (" + hiddenIdsStr + ");";
            WorldDatabase.Execute(backupQuery);
            LOG_INFO("mod.visible_npc", "mod-visible-npc: NPC spawnMask 백업 쿼리 실행 요청 완료.");

            // 2. creature 테이블의 spawnMask를 0으로 업데이트하여 숨김
            std::string hideQuery = "UPDATE creature SET spawnMask = 0 WHERE id1 IN (" + hiddenIdsStr + ");";
            WorldDatabase.Execute(hideQuery);
            LOG_INFO("mod.visible_npc", "mod-visible-npc: NPC 숨김 쿼리 실행 요청 완료.");
        }
        else
        {
            // 모듈 비활성화 시: NPC 복원 로직
            LOG_INFO("mod.visible_npc", "mod-visible-npc: 모듈 비활성화. NPC 복원 작업을 시작합니다.");
            RestoreNpcs();
        }
    }

private:
    // 생성자 (private으로 외부 생성 방지)
    ModVisibleNpcConfig() {}

    // 특정 설정 키로부터 NPC ID 목록을 파싱하여 주어진 set에 추가합니다.
    void LoadEntriesFromConfig(const std::string& configKey, std::set<uint32>& targetSet)
    {
        std::string entriesStr = sConfigMgr->GetOption<std::string>(configKey, "");
        if (entriesStr.empty())
        {
            return; // 설정 값이 비어있으면 무시합니다.
        }

        std::stringstream ss(entriesStr);
        std::string entry;
        // 쉼표(,)를 기준으로 문자열을 분리하여 각 ID를 처리합니다.
        while (std::getline(ss, entry, ','))
        {
            try
            {
                // ID 문자열의 앞뒤 공백을 제거합니다.
                entry.erase(0, entry.find_first_not_of(" \t\n\r"));
                entry.erase(entry.find_last_not_of(" \t\n\r") + 1);
                
                if (!entry.empty())
                {
                    // 문자열을 숫자로 변환하여 목록에 추가합니다.
                    targetSet.insert(std::stoul(entry));
                }
            }
            catch (const std::invalid_argument& ia)
            {
                LOG_ERROR("mod.visible_npc", "mod-visible-npc: 유효하지 않은 NPC ID 형식: '%s' (키: %s)", entry.c_str(), configKey.c_str());
            }
            catch (const std::out_of_range& oor)
            {
                LOG_ERROR("mod.visible_npc", "mod-visible-npc: NPC ID가 범위를 벗어남: '%s' (키: %s)", entry.c_str(), configKey.c_str());
            }
        }
    }

    // NPC를 원래 spawnMask로 복원하고 백업 테이블을 비웁니다.
    void RestoreNpcs()
    {
        // 1. 백업 테이블의 정보를 사용하여 creature 테이블의 spawnMask를 원본으로 복원
        std::string restoreQuery = "UPDATE creature c JOIN mod_visible_npc_backup b ON c.id1 = b.entry SET c.spawnMask = b.original_spawnmask;";
        WorldDatabase.Execute(restoreQuery);
        LOG_INFO("mod.visible_npc", "mod-visible-npc: NPC 복원 쿼리 실행 요청 완료.");

        // 2. 백업 테이블 비우기
        std::string clearBackupQuery = "TRUNCATE TABLE mod_visible_npc_backup;";
        WorldDatabase.Execute(clearBackupQuery);
        LOG_INFO("mod.visible_npc", "mod-visible-npc: 백업 테이블 비우기 쿼리 실행 요청 완료.");
    }
};

// 월드 서버의 상태 변화에 따라 동작하는 스크립트
class mod_visible_npc_WorldScript : public WorldScript
{
public:
    mod_visible_npc_WorldScript() : WorldScript("mod_visible_npc_WorldScript") { }

    // 설정 파일을 로드하기 전에 호출되는 함수 (서버 시작 시)
    void OnBeforeConfigLoad(bool /*reload*/) override
    {
        // 모듈 설정을 로드하고 DB 작업을 수행합니다.
        ModVisibleNpcConfig::instance()->Load();
    }
};

// 모듈의 모든 스크립트를 등록하는 함수
void Addmod_visible_npcScripts()
{
    // new mod_visible_npc_AllCreatureScript(); // 더 이상 필요 없음
    new mod_visible_npc_WorldScript();
}