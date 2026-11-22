-- Zen Alchemist Stone (item 75274): its spell 105574 is a proc-trigger aura with no trigger
-- spell in the 5.4.8 client DBC (35 percent proc chance, proc flags 349456), so it did
-- nothing. The script gives the Strength, Agility or Intellect proc buff (60229, 60233,
-- 60234) for the highest stat, with the amount of the stone's own effect, and a 55 second
-- cooldown.
-- Before this update there was no spell_script_names row for 105574 and no spell_proc_event row.
DELETE FROM `spell_script_names` WHERE `spell_id` = 105574;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(105574, 'spell_item_zen_alchemist_stone');
