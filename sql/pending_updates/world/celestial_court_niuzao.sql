-- Timeless Isle celestial Niuzao (71954): binds the C++ script boss_niuzao.cpp, which follows
-- alexkulya/pandaria_5.4.8 boss_niuzao.cpp (Oxen Fortitude, Headbutt, Massive Quake, and the charge
-- round through the court at 66% and 33%). Spell ids checked in the 5.4.8 Spell.dbc;
-- text groups as in that world DB.
-- Health_mod and unit_flags as in that world DB: Niuzao had Health_mod 1 and the
-- immune-to-players flag (256).
UPDATE `creature_template` SET `ScriptName` = 'boss_niuzao', `Health_mod` = 225, `unit_flags` = 0 WHERE `entry` = 71954;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (144608, 144610);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(144608, 'spell_niuzao_charge'),
(144610, 'spell_niuzao_headbutt');
