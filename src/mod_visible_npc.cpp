#include "ScriptMgr.h"
#include "Creature.h"
#include "Config.h"
#include <set>
#include <string>
#include <sstream>

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

    // 모듈 활성화 여부를 반환합니다.
    bool IsEnabled() const { return _isEnabled; }
    // 숨길 NPC 목록을 반환합니다.
    const std::set<uint32>& GetHiddenEntries() const { return _hiddenEntries; }

    // 설정 파일(worldserver.conf)에서 설정을 로드합니다.
    void Load()
    {
        // "VisibleNPC.Enable" 값을 읽어 모듈 활성화 여부를 결정합니다.
        _isEnabled = sConfigMgr->GetOption<bool>("VisibleNPC.Enable", false);
        if (!_isEnabled)
        {
            return; // 비활성화 시, 더 이상 진행하지 않습니다.
        }

        _hiddenEntries.clear(); // 기존 목록을 초기화합니다.

        // 설정 파일에 정의된 각 그룹의 키를 직접 읽어옵니다.
        LoadEntriesFromConfig("VisibleNPC.HiddenEntries.QuelDanas_Scourge");
        LoadEntriesFromConfig("VisibleNPC.HiddenEntries.QuelDanas_ShatteredSun");
        LoadEntriesFromConfig("VisibleNPC.HiddenEntries.QuelDanas_Dawnblade");
        LoadEntriesFromConfig("VisibleNPC.HiddenEntries.QuelDanas_Misc");
        LoadEntriesFromConfig("VisibleNPC.HiddenEntries.Shattrath"); // 예시용 샤트라스 그룹
    }

private:
    // 생성자 (private으로 외부 생성 방지)
    ModVisibleNpcConfig() : _isEnabled(false) {}

    // 특정 설정 키로부터 NPC ID 목록을 파싱하여 _hiddenEntries에 추가합니다.
    void LoadEntriesFromConfig(const std::string& configKey)
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
                    _hiddenEntries.insert(std::stoul(entry));
                }
            }
            catch (const std::invalid_argument& ia)
            {
                // 유효하지 않은 숫자 형식일 경우 로그를 남기고 무시할 수 있습니다. (에러 처리)
            }
            catch (const std::out_of_range& oor)
            {
                // 숫자의 범위가 너무 클 경우 로그를 남기고 무시할 수 있습니다. (에러 처리)
            }
        }
    }

    bool _isEnabled; // 모듈 활성화 상태
    std::set<uint32> _hiddenEntries; // 숨길 NPC ID 목록
};

// 모든 크리쳐에 적용되는 스크립트
class mod_visible_npc_AllCreatureScript : public AllCreatureScript
{
public:
    mod_visible_npc_AllCreatureScript() : AllCreatureScript("mod_visible_npc_AllCreatureScript") {}

    // 크리쳐가 월드에 추가될 때 호출되는 함수
    void OnCreatureAddWorld(Creature* creature) override
    {
        // 모듈이 비활성화 상태이면 아무것도 하지 않습니다.
        if (!ModVisibleNpcConfig::instance()->IsEnabled())
        {
            return;
        }

        // 숨길 NPC 목록을 가져옵니다.
        const auto& hiddenEntries = ModVisibleNpcConfig::instance()->GetHiddenEntries();
        // 현재 크리쳐의 ID가 숨길 목록에 있는지 확인합니다.
        if (hiddenEntries.find(creature->GetEntry()) != hiddenEntries.end())
        {
            // 리스폰 시간을 0으로 설정하여 다시 생성되지 않도록 합니다.
            creature->SetRespawnTime(0);
            // 즉시 월드에서 제거합니다.
            creature->DespawnOrUnsummon();
        }
    }
};

// 월드 서버의 상태 변화에 따라 동작하는 스크립트
class mod_visible_npc_WorldScript : public WorldScript
{
public:
    mod_visible_npc_WorldScript() : WorldScript("mod_visible_npc_WorldScript") { }

    // 설정 파일을 로드하기 전에 호출되는 함수
    void OnBeforeConfigLoad(bool /*reload*/) override
    {
        // 모듈 설정을 로드합니다.
        ModVisibleNpcConfig::instance()->Load();
    }
};

// 모듈의 모든 스크립트를 등록하는 함수
void Addmod_visible_npcScripts()
{
    new mod_visible_npc_AllCreatureScript();
    new mod_visible_npc_WorldScript();
}
