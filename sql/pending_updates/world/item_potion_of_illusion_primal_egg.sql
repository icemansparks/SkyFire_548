-- Potion of Illusion (item 58489, craftable with Alchemy through spell 80269): spell 80265
-- had no script, so drinking it did nothing. The script turns the caster into a mirror image
-- of the selected or a random nearby unit.
-- Primal Egg (item 94295, dropped by Primal Devilsaurs and Direhorns): the egg has a 3 day
-- duration and no script, so it expired without giving the Cracked Primal Egg (94296,
-- an openable container with loot).
-- No spell_script_names row existed for 80265 and no item_script_names row for 94295.
DELETE FROM `spell_script_names` WHERE `spell_id` = 80265;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(80265, 'spell_item_potion_of_illusion');
DELETE FROM `item_script_names` WHERE `Id` = 94295;
INSERT INTO `item_script_names` (`Id`, `ScriptName`) VALUES
(94295, 'item_primal_egg');
