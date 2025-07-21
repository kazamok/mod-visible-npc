
/*
filename: mod-visible-npc.sql
테이블 생성 코드 */

USE acore_world;

CREATE TABLE IF NOT EXISTS `mod_visible_npc_backup` (
 `entry` INT UNSIGNED NOT NULL, -- 이 entry 컬럼이 creature.id1 값을 저장합니다.
 `original_spawnmask` INT UNSIGNED NOT NULL,
 PRIMARY KEY (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
