-- Timeless Isle celestial Yu'lon (71955): binds the C++ script boss_yu_lon.cpp, which follows
-- alexkulya/pandaria_5.4.8 boss_yu_lon.cpp (Jadefire Breath, Jadefire Buffet, Jadefire Bolt with
-- Jadefire Blaze, rows of Jadefire Walls with one gap). Spell ids checked in the 5.4.8 Spell.dbc;
-- text groups as in that world DB.
-- Health_mod and faction as in that world DB: Yu'lon had Health_mod 1 and faction 190,
-- which is hostile to some players; the celestials are friendly (35) until Shaohao calls them.
-- The Jadefire Blaze (72016) and Jadefire Wall (72020) templates did not exist, so Yu'lon
-- had nothing to summon: added with the values of that world DB.
UPDATE `creature_template` SET `ScriptName` = 'boss_yu_lon', `Health_mod` = 212.5, `faction_A` = 35, `faction_H` = 35 WHERE `entry` = 71955;

DELETE FROM `creature_template` WHERE `entry` IN (72016, 72020);
INSERT INTO `creature_template` (`entry`, `modelid1`, `modelid2`, `name`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `speed_walk`, `speed_run`, `mindmg`, `maxdmg`, `attackpower`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `type`, `type_flags`, `InhabitType`, `HoverHeight`, `Health_mod`, `movementId`, `ScriptName`, `flags_extra`) VALUES
(72016, 31174, 11686, 'Jadefire Blaze', 90, 90, 4, 14, 14, 1, 1.14286, 9839, 14339, 42299, 2000, 2000, 1, 0, 2048, 10, 1024, 3, 1, 1, 61, '', 128),
(72020, 31174, 11686, 'Jadefire Wall', 90, 90, 4, 14, 14, 1, 1.14286, 9839, 14339, 42299, 2000, 2000, 1, 0, 2048, 10, 1024, 3, 1, 1, 156, 'npc_jadefire_wall', 128);

DELETE FROM `spell_script_names` WHERE `spell_id` = 144545;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(144545, 'spell_yulon_jadefire_bolt_selector');
