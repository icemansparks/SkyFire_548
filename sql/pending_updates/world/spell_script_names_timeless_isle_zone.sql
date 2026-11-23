-- Timeless Isle zone spell scripts
DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_bloody_coin_proc', 'spell_censer_of_eternal_agony', 'spell_gulp_frog_toxin', 'spell_spirit_world', 'spell_cat_defensive_leap', 'spell_timelesscat_pounce', 'spell_evermaw_time_portals');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(148385, 'spell_bloody_coin_proc'),
(148429, 'spell_bloody_coin_proc'),
(148385, 'spell_censer_of_eternal_agony'),
(147655, 'spell_gulp_frog_toxin'),
(144145, 'spell_spirit_world'),
(147650, 'spell_cat_defensive_leap'),
(147649, 'spell_timelesscat_pounce'),
(147812, 'spell_evermaw_time_portals'),
(147841, 'spell_evermaw_time_portals');
