-- Class trainers sold class spells for gold, a Cataclysm leftover.
-- Since patch 5.0.4 class abilities are learned automatically on
-- reaching their level, and the core does so in Player::GiveLevel and
-- at login from the class skill lines of SkillLineAbility.dbc.
-- Class trainers only reset talents and specializations and handle
-- dual specialization.
-- Source: the 5.4.8 world DB of alexkulya/pandaria_5.4.8 (2026_08_16)
-- lists no spells for any class trainer; its portal trainers keep
-- their teleports and portals, so the portal lists 200200-200202 stay.
-- Cross-check against the 5.4.8 DBC: 317 of the 339 spells in the
-- reference lists 200001-200011 and 200100-200103 are learned
-- automatically. The rest no longer exist in MoP or have a MoP
-- replacement (Swipe 106785, Stampeding Roar 106898, Thrash and the
-- armor specializations as specialization spells); Plate Mail comes at
-- level 40 from Player::GiveLevel.
-- Swift Flight Form (40120) stays on the druid list: in 5.0.4 it
-- replaced Flight Form, but it has no automatic path in this core
-- (SkillLineAbility acquire method 0), so the trainer is still the only
-- way to get it.

-- References to the class spell lists
DELETE FROM `npc_trainer` WHERE `spell` BETWEEN -200010 AND -200001;
DELETE FROM `npc_trainer` WHERE `spell` BETWEEN -200103 AND -200100;

-- The class spell lists themselves
DELETE FROM `npc_trainer` WHERE `entry` BETWEEN 200001 AND 200010;
DELETE FROM `npc_trainer` WHERE `entry` BETWEEN 200100 AND 200103;
DELETE FROM `npc_trainer` WHERE `entry` = 200011 AND `spell` <> 40120;
