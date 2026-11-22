-- Shadowmeld (Night Elf racial) never left combat, Rocket Barrage (Goblin racial)
-- dealt the 1 base point of the 5.4.8 client DBC, and the Inactive debuff from the
-- battleground AFK report never removed the player when it expired. All three need
-- a script.
-- No spell_script_names rows existed for these three ids.
DELETE FROM `spell_script_names` WHERE `spell_id` IN (58984, 69041, 43681);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(58984, 'spell_gen_shadowmeld'),
(69041, 'spell_gen_rocket_barrage'),
(43681, 'spell_gen_inactive');
