// mod_visible_npc.cpp
// 특정 NPC들을 숨기는 모듈입니다.
// NPC Entry ID를 코드 내에 직접 하드코딩하여 관리합니다.

#include "ScriptMgr.h"
#include "Creature.h"
#include <set>

// 모듈 클래스 정의
class mod_visible_npc : public AllCreatureScript
{
public:
    mod_visible_npc() : AllCreatureScript("mod_visible_npc") {}

    // NPC 생성 직후 호출되는 함수
    void OnCreatureAddWorld(Creature* creature) override
    {
        // 숨김 처리할 NPC 목록에 해당 Entry가 있으면 즉시 제거
        if (hiddenEntries.find(creature->GetEntry()) != hiddenEntries.end())
        {
            // 리스폰 방지
            creature->SetRespawnTime(0);
            // 즉시 소환 해제
            creature->DespawnOrUnsummon();
        }
    }

private:
    // 숨김할 NPC Entry ID 목록
    // 여기서 직접 원하는 NPC ID들을 추가하거나 제거하세요.
    static const std::set<uint32> hiddenEntries;
};

// 숨김 NPC Entry 리스트 하드코딩
const std::set<uint32> mod_visible_npc::hiddenEntries = {

// 쿠엘 섬
37542	, //	[Morlen Coldgrip]
37552	, //	[Thalorien Dawnseeker's Remains]
37205	, //	[Thalorien Dawnseeker]
37541	, //	[Crypt Raider]
37539	, //	[Ghoul Invader]
37538	, //	[Scourge Zombie]
37523	, //	[Warden of the Sunwell]
27059	, //	[Plague Zombie]
25174	, //	키루
25003	, //	증오의 사절
25169	, //	대마법사 네툴
25031	, //	지옥의 대군주
25001	, //	심연의 화염경비병
25060	, //	어둠가시일족 미르미돈
25073	, //	어둠가시일족 세이렌
25002	, //	해방된 지옥무법자
37527	, //	[Halduron Brightwing]
25170	, //	무너진 태양 대마법사
25175	, //	무너진 태양 용매
24994	, //	무너진 태양 파수병
26253	, //	무너진 태양 평화감시단
26092	, //	소린
25236	, //	속박에서 풀려난 용매
25059	, //	아이렌 클라우드브레이커
26560	, //	오후라
26091	, //	올루스
25977	, //	이르마
26090	, //	카린나
25039	, //	칼리프
26089	, //	케이리
25976	, //	테레미스
25162	, //	훈련교관 바둠
24938	, //	무너진 태양 명사수
25115	, //	무너진 태양 전사
25036	, //	간병인 이나라
24965	, //	구원자 세이얀
25108	, //	구원자 카알란
24967	, //	대장 테리스 돈하스
25046	, //	대장장이 하우타
24975	, //	마르나
25069	, //	마법학자 일라스타
24999	, //	분노의 지휘관
24979	, //	새벽칼날단 명사수
25087	, //	새벽칼날단 보충병
24978	, //	새벽칼날단 소환사
25063	, //	새벽칼날단 용매기수
24976	, //	새벽칼날단 혈기사
25950	, //	샤아니
25061	, //	선구자 이누로
25088	, //	선장 발린드리아
25037	, //	세라피나 블러드하트
25043	, //	세레스 더스크브링어
25112	, //	수도사 아유리
25163	, //	수도사 케어소스
25032	, //	엘다라 돈러너
24972	, //	오작동하는 파수병
25045	, //	장군이
25057	, //	전투마법사 아린나
25133	, //	점성술사 다르나리안
24813	, //	총독 라레소르
25035	, //	티라엘 플레임키스트
25034	, //	포르타누스
25030	, //	격노의 집행자
25027	, //	광포한 구울
25049	, //	돈스타 군마
25028	, //	해골 약탈병
25047	, //	버림받은 엘프 걸신
24966	, //	버림받은 엘프 마귀
24960	, //	버림받은 엘프 포식자
25144	, //	무너진 태양 폭격수
25084	, //	초록아가미일족 노예
6491	, //	영혼의 치유사
30481	, //	다크문 점술사
25225	, //	훈련용 허수아비
25161	, //	새벽칼날단 용매
29341	, //	[Dual-plagued Brain]
37509	, //	[Shattered Sun Sentry]
37512	, //	[Shattered Sun Warrior]
37707	, //	[Silvermoon Builder]
37211	 //	[Sunwell Defender]

// 샤트라스
};

// 모듈 등록 함수 (스크립트 매니저에 등록)
void Addmod_visible_npcScripts()
{
    new mod_visible_npc();
}
