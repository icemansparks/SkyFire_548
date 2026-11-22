-- Drums of Rage (item 102351): spell 146555 gives 25 percent haste to the raid like
-- Bloodlust, but had no script, so it ignored the lockout debuffs and stacked without limit.
-- The script skips raid members with Insanity, Exhaustion, Sated or Temporal Displacement
-- and gives Exhaustion to everyone hit. Ancient Hysteria (90355) is a hunter pet ability
-- and stays with the class spells.
-- Before this update there was no spell_script_names row for 146555.
DELETE FROM `spell_script_names` WHERE `spell_id` = 146555;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(146555, 'spell_item_drums_of_rage');
