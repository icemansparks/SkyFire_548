-- Timeless Isle world boss Ordos (72057): binds the C++ script boss_ordos.cpp, ported from
-- alexkulya's 5.4.8 boss_ordos.cpp (Magma Crush, Burning Soul, Ancient Flame, Eternal Agony,
-- Pool of Fire cast). Spell ids checked in the 5.4.8 Spell.dbc. Health_mod 1500 is already set.
-- The Ancient Flame template (72059) did not exist, so Ancient Flame had nothing to summon:
-- added with alexkulya's 5.4.8 values.
-- The Ordos template already names boss_ordos. No spell_script_names row existed for 144688 or
-- 144689.
DELETE FROM `creature_template` WHERE `entry` = 72059;
INSERT INTO `creature_template` (`entry`, `modelid1`, `modelid2`, `name`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `speed_walk`, `speed_run`, `mindmg`, `maxdmg`, `attackpower`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags2`, `type`, `type_flags`, `InhabitType`, `HoverHeight`, `Health_mod`, `movementId`, `ScriptName`) VALUES
(72059, 31174, 11686, 'Ancient Flame', 90, 90, 4, 14, 14, 1, 1.14286, 9839, 14339, 42299, 2000, 2000, 1, 2048, 10, 1024, 4, 1, 1, 61, 'npc_ordos_ancient_flame');

DELETE FROM `spell_script_names` WHERE `spell_id` IN (144688, 144689);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(144688, 'spell_ordos_magma_crush'),
(144689, 'spell_ordos_burning_soul');
