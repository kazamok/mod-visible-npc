# mod-visible-npc (NPC 숨김 모듈)

이 모듈은 AzerothCore 서버 관리자가 특정 NPC를 설정 파일을 통해 보이지 않게 숨길 수 있도록 하는 커스텀 모듈입니다.

## 🔧 주요 기능

- NPC Entry ID를 기반으로 특정 NPC를 숨기거나 표시할 수 있습니다.
- `mod_visible_npc.conf` 파일을 통해 설정을 관리합니다.
- **동적 로딩:** NPC 목록을 논리적인 그룹으로 나누어 관리할 수 있으며, `VisibleNPC.HiddenEntries.` 로 시작하는 모든 설정은 자동으로 로드됩니다.
- 설정 스위치 하나로 모듈 전체를 켜고 끌 수 있습니다.
- 불타는 성정 태양샘 고원 함락 이벤트를 끄도록 옵션을 추가하고 별도의 NPC들의 가시성을 조절할 때 사용합니다.

## 📁 파일 구성

- `src/mod_visible_npc.cpp` – 모듈의 핵심 로직이 담긴 C++ 소스 파일입니다.
- `conf/mod_visible_npc.conf.dist` – 설정 파일입니다.
- `CMakeLists.txt` – 모듈 빌드 설정 파일입니다.

## ⚙️ 설정 방법

모듈을 설정하려면, `mod_visible_npc.conf` 라는 파일에 아래 내용을 작성하십시오.

**1. 모듈 활성화:**

`VisibleNPC.Enable` 값을 `1`로 설정하면 모듈이 활성화되고, `0`으로 설정하면 비활성화됩니다.

```ini
# NPC 숨김 모듈을 활성화하거나 비활성화합니다.
# 1 = 활성화, 0 = 비활성화
VisibleNPC.Enable = 1
```

**2. NPC 목록 설정:**

숨기고 싶은 NPC의 Entry ID를 목록에 추가하여 관리할 수 있습니다. 각 목록의 설정 키는 반드시 `VisibleNPC.HiddenEntries.` 로 시작해야 합니다.

- ID는 쉼표(`,`)로 구분해야 합니다.
- `VisibleNPC.HiddenEntries.나만의그룹`과 같이 필요한 만큼 자유롭게 카테고리를 만들 수 있습니다.
- `#`으로 시작하는 줄은 주석으로 처리되어 무시됩니다.

아래는 쿠엘다나스 섬의 NPC를 4개의 논리적 그룹으로 나눈 설정 예시입니다.

```ini
# --- 쿠엘다나스 섬: 스컬지 및 악마 ---
VisibleNPC.HiddenEntries.QuelDanas_Scourge = 37541,37539,37538,27059,25003

# --- 쿠엘다나스 섬: 무너진 태양 공격대 ---
VisibleNPC.HiddenEntries.QuelDanas_ShatteredSun = 37523,37527,25170,25175,24994

# --- 쿠엘다나스 섬: 새벽칼날단 ---
VisibleNPC.HiddenEntries.QuelDanas_Dawnblade = 24979,25087,24978,25063,24976

# --- 쿠엘다나스 섬: 기타 ---
VisibleNPC.HiddenEntries.QuelDanas_Misc = 37542,37552,37205,25174,25169

# --- 샤트라스 ---
VisibleNPC.HiddenEntries.Shattrath = 12345, 67890
```

## 🔄 설정 리로드

설정 파일을 수정한 후에는, 변경 사항을 적용하기 위해 월드서버를 재시작해야 합니다. 실시간 리로드 기능은 아직 구현되지 않았습니다.
