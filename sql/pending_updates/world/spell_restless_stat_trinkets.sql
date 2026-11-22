-- Steadfast Talisman of the Shado-Pan Assault (item 94507, sold by Ao Pye 70346) and the
-- Siege of Orgrimmar restless trinkets: the buff is meant to start at full stacks (10, or
-- 20 for Restless Agility and Restless Spirit) and lose one stack per tick of its periodic
-- dummy effect (2 s, or 0.5 s). With no script it started at one stack and never counted down.
-- Before this update there were no spell_script_names rows for 138728, 146310 and 146317.
DELETE FROM `spell_script_names` WHERE `spell_id` IN (138728, 146310, 146317);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(138728, 'spell_item_restless_stat_trinket'),
(146310, 'spell_item_restless_stat_trinket'),
(146317, 'spell_item_restless_stat_trinket');
