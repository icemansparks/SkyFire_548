-- Weapon enchants and the glove tinker of Mists of Pandaria that did nothing, although
-- their recipes are on trainer lists: Windsong (104561) and Colossus (118314) are bare
-- dummy auras, Elemental Force (104428) triggers its damage spell on every hit, and the
-- Synapse Springs on-use spells (82174, 126734) have a bare dummy effect.
-- Proc rates come from the 5.4.8 client's SpellProcsPerMinute.dbc, reached through each
-- spell's SpellAuraOptions row: 2.2 PPM for Windsong (id 65), 5.5 PPM for Colossus
-- (id 66) and 9.17 PPM for Elemental Force (id 63). The core does not read that table, so
-- the rate goes into spell_proc_event. procFlags 0 falls back to the DBC flags (0x55554).
-- No spell_proc_event rows existed for the three proc ids and no spell_script_names rows
-- for the four script ids.
DELETE FROM `spell_script_names` WHERE `spell_id` IN (104561, 118314, 82174, 126734);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(104561, 'spell_ench_windsong'),
(118314, 'spell_ench_colossus'),
(82174, 'spell_ench_synapse_springs'),
(126734, 'spell_ench_synapse_springs');

DELETE FROM `spell_proc_event` WHERE `entry` IN (104561, 118314, 104428);
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES
(104561, 0, 0, 0, 0, 0, 0, 0, 0, 2.2, 0, 0),
(118314, 0, 0, 0, 0, 0, 0, 0, 0, 5.5, 0, 0),
(104428, 0, 0, 0, 0, 0, 0, 0, 0, 9.17, 0, 0);
