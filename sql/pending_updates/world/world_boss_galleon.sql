-- Valley of the Four Winds world boss Galleon (62346): binds the C++ script boss_galleon.cpp,
-- ported from alexkulya's 5.4.8 boss_galion.cpp (Stomp, Cannon Barrage, Salyin Warmonger waves,
-- Berserk; Salyin Skirmisher Fire Shot; Warmonger Impaling Pull). Spell ids checked in the 5.4.8
-- Spell.dbc. Script names and the Salyin Warmonger health modifier as in alexkulya's 5.4.8 DB.
-- Galleon (62346) already has ScriptName boss_galion and Health_mod 735. The Salyin Skirmisher
-- (62350) had no ScriptName, and the Salyin Warmonger (62351) had npc_galion with Health_mod 1.
-- No spell_script_names row existed for 121673.
UPDATE `creature_template` SET `ScriptName` = 'npc_salyin_skrimisher' WHERE `entry` = 62350;
UPDATE `creature_template` SET `ScriptName` = 'npc_salyin_warmonger', `Health_mod` = 10 WHERE `entry` = 62351;

DELETE FROM `spell_script_names` WHERE `spell_id` = 121673;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(121673, 'spell_galleon_fire_shot');
