-- Weapon enchant procs of Mists of Pandaria that did nothing: Dancing Steel (118333) and
-- Bloody Dancing Steel (142531) are bare dummy auras, Jade Spirit (120033) and Spirit of
-- Conquest (142536) proc nothing, and the Jade Spirit buffs (104993, 142535) gave their
-- Spirit bonus at any mana (142535 is the buff, 142536 its passive).
-- Proc rates come from the 5.4.8 client's SpellProcsPerMinute.dbc, reached through each
-- spell's SpellAuraOptions row: 2.53 PPM for Dancing Steel and Bloody Dancing Steel
-- (id 67), 2.2 PPM for Jade Spirit and Spirit of Conquest (id 65). The core does not read
-- that table, so the rate goes into spell_proc_event. All four spells carry the proc flags
-- 0x55554 in the DBC; the Dancing Steel rows keep alexkulya's narrower mask 332116 and the
-- Jade Spirit rows leave procFlags 0 to fall back to the DBC flags.
-- No spell_proc_event rows existed for the four proc ids and no spell_script_names rows
-- for the six script ids.
DELETE FROM `spell_script_names` WHERE `spell_id` IN (118333, 142531, 120033, 142536, 104993, 142535);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(118333, 'spell_ench_dancing_steel'),
(142531, 'spell_ench_dancing_steel'),
(120033, 'spell_ench_jade_spirit_passive'),
(142536, 'spell_ench_jade_spirit_passive'),
(104993, 'spell_ench_jade_spirit'),
(142535, 'spell_ench_jade_spirit');

DELETE FROM `spell_proc_event` WHERE `entry` IN (118333, 142531, 120033, 142536);
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES
(118333, 0, 0, 0, 0, 0, 0, 332116, 0, 2.53, 0, 0),
(142531, 0, 0, 0, 0, 0, 0, 332116, 0, 2.53, 0, 0),
(120033, 0, 0, 0, 0, 0, 0, 0, 0, 2.2, 0, 0),
(142536, 0, 0, 0, 0, 0, 0, 0, 0, 2.2, 0, 0);
