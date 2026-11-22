-- Crafted items and a quest spell whose recipes are on trainer lists, but whose spells
-- have a bare dummy or create-item effect: Wormhole Generator: Pandaria (126755, the nine
-- destination spells and their spell_target_position rows exist), Alchemist's Flask
-- (105617) and the Ghostly Skeleton Key (122633, kill credit for quests 32683 and 32726).
-- No spell_script_names rows existed for these three ids.
DELETE FROM `spell_script_names` WHERE `spell_id` IN (126755, 105617, 122633);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(126755, 'spell_item_wormhole_pandaria'),
(105617, 'spell_item_alchemists_flask'),
(122633, 'spell_q32683_q32726_ghostly_skeleton_key');
