-- Kun-Lai Summit world boss Sha of Anger (60491): its template already names boss_sha_of_anger,
-- which was not registered; boss_sha_of_anger.cpp now provides it, ported from alexkulya's 5.4.8
-- boss_sha_of_anger.cpp (Seethe, Endless Rage clouds, Growing Anger and Aggressive Behavior,
-- Unleashed Wrath at full rage, Overcome by Anger within 30 yards, Berserk). Spell ids checked in
-- the 5.4.8 Spell.dbc. Text groups as alexkulya's.
-- No spell_script_names rows existed for 119626 or 129356.
DELETE FROM `spell_script_names` WHERE `spell_id` IN (119626, 129356);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(119626, 'spell_sha_of_anger_aggressive_behaviour'),
(129356, 'spell_sha_of_anger_overcome_by_anger');
