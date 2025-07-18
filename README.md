# mod-visible-npc (NPC 숨김 모듈)

이 모듈은 AzerothCore 서버 관리자가 특정 NPC를 설정 파일을 통해 보이지 않게 숨길 수 있도록 하는 커스텀 모듈입니다.

## 🔧 주요 기능

- NPC Entry ID를 기반으로 특정 NPC를 숨기거나 표시할 수 있습니다.
- `worldserver.conf` 또는 별도의 `mod_visible_npc.conf` 파일을 통해 설정을 관리합니다.
- **동적 로딩:** NPC 목록을 논리적인 그룹으로 나누어 관리할 수 있으며, `VisibleNPC.HiddenEntries.` 로 시작하는 모든 설정은 자동으로 로드됩니다.
- 설정 스위치 하나로 모듈 전체를 켜고 끌 수 있습니다.
- **NPC 스폰 제어:** 서버 시작 시 데이터베이스의 `spawnMask` 값을 직접 수정하여 NPC의 스폰을 영구적으로 제어합니다.
- **안전한 복원:** 모듈 비활성화 시, 숨겨졌던 NPC들의 `spawnMask` 값을 원래대로 복원합니다.

## 📁 파일 구성

- `src/mod_visible_npc.cpp` – 모듈의 핵심 로직이 담긴 C++ 소스 파일입니다.
- `conf/mod_visible_npc.conf.dist` – 설정 예시 파일입니다.
- `CMakeLists.txt` – 모듈 빌드 설정 파일입니다.

## ⚙️ 설정 방법

모듈을 사용하기 전에 다음 단계를 따라야 합니다.

**1. 데이터베이스 테이블 생성 (필수):**

이 모듈은 NPC의 원래 스폰 정보를 백업하고 복원하기 위해 `acore_world` 데이터베이스에 `mod_visible_npc_backup`이라는 보조 테이블을 사용합니다. 모듈을 사용하기 전에 반드시 이 테이블을 생성해야 합니다.

**실행 이유:**
NPC를 숨기기 위해 `creature` 테이블의 `spawnMask` 값을 변경합니다. 모듈을 비활성화했을 때 NPC를 원래대로 복원하려면, 변경하기 전의 `spawnMask` 값을 알아야 합니다. 이 테이블이 그 원본 값을 안전하게 저장하는 역할을 합니다.

**실행 방법:**
MySQL 클라이언트(예: HeidiSQL, DBeaver, MySQL Workbench)를 사용하여 `acore_world` 데이터베이스에 연결한 후 다음 SQL 쿼리를 실행하십시오. 이 쿼리는 테이블이 이미 존재하더라도 오류 없이 실행되므로, 여러 번 실행해도 안전합니다.

```sql
USE acore_world;

CREATE TABLE IF NOT EXISTS `mod_visible_npc_backup` (
  `entry` INT UNSIGNED NOT NULL,
  `original_spawnmask` INT UNSIGNED NOT NULL,
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
```

**2. 모듈 활성화:**

`worldserver.conf` 파일에 직접 내용을 추가하거나, 더 나은 관리를 위해 `mod_visible_npc.conf` 라는 별도의 파일을 만들어 아래 내용을 작성하십시오.

`VisibleNPC.Enable` 값을 `1`로 설정하면 모듈이 활성화되고, `0`으로 설정하면 비활성화됩니다.

```ini
# NPC 숨김 모듈을 활성화하거나 비활성화합니다.
# 1 = 활성화, 0 = 비활성화
VisibleNPC.Enable = 1
```

**3. NPC 목록 설정:**

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

# --- 샤트라스 (예시) ---
# 이 그룹은 현재 주석 처리되어 비활성화된 상태입니다.
# VisibleNPC.HiddenEntries.Shattrath = 12345, 67890
```

## 🔄 설정 리로드

설정 파일을 수정한 후에는, 변경 사항을 적용하기 위해 월드서버를 재시작해야 합니다. 실시간 리로드 기능은 아직 구현되지 않았습니다.