-- Isle of Giants world boss Oondasta (69161): binds the C++ script boss_oondasta.cpp, ported
-- from alexkulya's 5.4.8 boss_oondasta.cpp (Alpha Male, Crush, Frill Blast, Piercing Roar,
-- Spiritfire Beam, Growing Fury). Spell ids checked in the 5.4.8 Spell.dbc.
-- Script names and the health modifier as in alexkulya's 5.4.8 DB: the template had no
-- ScriptName and Health_mod 1, so Oondasta died before its first ability.
-- No spell_script_names row existed for 138390.
UPDATE `creature_template` SET `ScriptName` = 'boss_oondasta', `Health_mod` = 1000 WHERE `entry` = 69161;

DELETE FROM `spell_script_names` WHERE `spell_id` = 138390;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(138390, 'spell_alpha_male_eff');
