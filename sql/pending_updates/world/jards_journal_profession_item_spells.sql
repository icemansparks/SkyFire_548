-- Profession and quest item spells with a bare script effect or dummy aura: Chief Engineer
-- Jard's Journal, the three "... and Its Uses" patterns (cloth, hide, ingot), Stardust No. 2
-- and the three Jewelcrafting dailies. They need a script to teach recipes or give quest
-- credit. No spell_script_names rows existed for these eight ids.
DELETE FROM `spell_script_names` WHERE `spell_id` IN (143743, 143626, 143644, 143646, 73667, 73227, 73243, 73274);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(143743, 'spell_chief_engineer_jards_journal'),
(143626, 'spell_learn_some_ingredient_and_its_uses'),
(143644, 'spell_learn_some_ingredient_and_its_uses'),
(143646, 'spell_learn_some_ingredient_and_its_uses'),
(73667, 'spell_item_stardust_no_2'),
(73227, 'spell_jewelcrafting_daily_quests_cata'),
(73243, 'spell_jewelcrafting_daily_quests_cata'),
(73274, 'spell_jewelcrafting_daily_quests_cata');
