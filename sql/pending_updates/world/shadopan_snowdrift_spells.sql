-- Shado-Pan Monastery (map 959): spell scripts of the Master Snowdrift event
-- Idempotent: DELETE-then-INSERT.

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_snowdrift_tornado_slam', 'spell_snowdrift_tornado_slam_effect', 'spell_spm_parry_stance', 'spell_snowdrift_parry_stance', 'spell_spm_swirling_steel', 'spell_snowdrift_ball_of_fire', 'spell_snowdrift_ball_of_fire_aura', 'spell_snowdrift_hurl_chi', 'spell_snowdrift_fists_of_fury', 'spell_snowdrift_ball_of_fire_damage', 'spell_snowdrift_copy_health');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(106332, 'spell_snowdrift_tornado_slam'),
(106352, 'spell_snowdrift_tornado_slam_effect'),
(106447, 'spell_spm_parry_stance'),
(106454, 'spell_snowdrift_parry_stance'),
(106699, 'spell_spm_swirling_steel'),
(106721, 'spell_snowdrift_ball_of_fire'),
(106825, 'spell_snowdrift_ball_of_fire_aura'),
(106825, 'spell_snowdrift_hurl_chi'),
(106853, 'spell_snowdrift_fists_of_fury'),
(115324, 'spell_snowdrift_ball_of_fire_damage'),
(115325, 'spell_snowdrift_ball_of_fire_damage'),
(122074, 'spell_snowdrift_copy_health');
