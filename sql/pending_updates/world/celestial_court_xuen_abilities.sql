-- Xuen's two area spells (Chi Barrage 144642, Crackling Lightning 144634) pick up to 8 players
-- and cast the spell stored in effect 0's base points on each; the script does that.
-- Follows alexkulya/pandaria_5.4.8 boss_xuen.cpp. No spell_script_names row existed for either spell.
DELETE FROM `spell_script_names` WHERE `spell_id` IN (144634, 144642);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(144634, 'spell_xuen_area_barrage'),
(144642, 'spell_xuen_area_barrage');
