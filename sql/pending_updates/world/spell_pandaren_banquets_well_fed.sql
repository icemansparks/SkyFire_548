-- Pandaren food that gave no stat buff: the six Great Banquets (Grill, Wok, Pot,
-- Steamer, Oven, Brew) have a bare script effect, and
-- the Food auras of Chao Cookies and the noodle soups have a periodic trigger with an
-- empty trigger spell in the 5.4.8 client DBC. The scripts pick the Well Fed stat buff
-- by role (stamina for tanks, spirit for healers, else the highest stat).
-- No spell_script_names rows existed for these nine ids.
DELETE FROM `spell_script_names` WHERE `spell_id` IN (126532, 126541, 126542, 126543, 126544, 126545, 104922, 126535, 144961);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(126532, 'spell_gen_banquet_of_the_x'),
(126541, 'spell_gen_banquet_of_the_x'),
(126542, 'spell_gen_banquet_of_the_x'),
(126543, 'spell_gen_banquet_of_the_x'),
(126544, 'spell_gen_banquet_of_the_x'),
(126545, 'spell_gen_banquet_of_the_x'),
(104922, 'spell_gen_noodle_soup'),
(126535, 'spell_gen_noodle_soup'),
(144961, 'spell_gen_noodle_soup');
