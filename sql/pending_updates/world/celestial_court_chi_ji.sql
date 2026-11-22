-- Timeless Isle celestial Chi-Ji (71952): binds the C++ script boss_chi_ji.cpp, which follows
-- alexkulya/pandaria_5.4.8 boss_chi_ji.cpp (Firestorm, Inspiring Song, Beacon of Hope with Blazing Song,
-- Crane Rush with the Children of Chi-Ji at 66% and 33%). Spell ids checked in the 5.4.8 Spell.dbc;
-- text groups as in that world DB.
-- Health_mod and unit_flags as in that world DB: Chi-Ji had Health_mod 1 and the
-- immune-to-players flag (256). The Firestorm (71971), Beacon of Hope (71978) and Child of
-- Chi-Ji (71990) templates did not exist, so Chi-Ji had nothing to summon: added with
-- the values of that world DB.
UPDATE `creature_template` SET `ScriptName` = 'boss_chi_ji', `Health_mod` = 200, `unit_flags` = 0 WHERE `entry` = 71952;

DELETE FROM `creature_template` WHERE `entry` IN (71971, 71978, 71990);
INSERT INTO `creature_template` (`entry`, `modelid1`, `modelid2`, `name`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `speed_walk`, `speed_run`, `mindmg`, `maxdmg`, `attackpower`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `type`, `type_flags`, `InhabitType`, `HoverHeight`, `Health_mod`, `movementId`, `ScriptName`, `flags_extra`) VALUES
(71971, 31174, 11686, 'Firestorm', 90, 90, 4, 16, 16, 1, 1.14286, 9839, 14339, 42299, 2000, 2000, 1, 0, 2048, 10, 1024, 4, 1, 1, 61, '', 128),
(71978, 31174, 11686, 'Beacon of Hope', 90, 90, 4, 14, 14, 1, 1.14286, 9839, 14339, 42299, 2000, 2000, 1, 0, 2048, 10, 1024, 4, 1, 1, 61, '', 128),
(71990, 39553, 0, 'Child of Chi-Ji', 90, 90, 4, 16, 16, 1, 1.14286, 9839, 14339, 42299, 2000, 2000, 1, 131074, 2048, 10, 0, 3, 1, 100, 907, 'npc_child_of_chi_ji', 0);

DELETE FROM `spell_script_names` WHERE `spell_id` IN (144470, 144475);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(144470, 'spell_chi_ji_crane_rush'),
(144475, 'spell_chi_ji_beacon_of_hope');
