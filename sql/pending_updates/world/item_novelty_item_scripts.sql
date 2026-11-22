-- Five novelty items that did nothing when used: Sylvanas' Music Box (item 52253, reward of
-- quest 24918) plays its lament, Flippable Table (88801, sold by Nam Ironpaw 64395) casts
-- its flip, Hardened Shell (86584, dropped by Nessos the Oracle 50789) dismounts its target,
-- Celestial Defender's Medallion (103685, sold by Mistweaver Ku 73306) and Spectral Grog
-- (104316, dropped by Spectral Pirate 71987) transform the user by gender.
-- No spell_script_names rows existed for 73331, 129339, 129787, 149228 and 148565.
DELETE FROM `spell_script_names` WHERE `spell_id` IN (73331, 129339, 129787, 149228, 148565);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(73331, 'spell_item_sylvanas_music_box'),
(129339, 'spell_item_flippable_table'),
(129787, 'spell_item_hardened_shell'),
(149228, 'spell_item_celestial_defender'),
(148565, 'spell_item_spectral_grog');
