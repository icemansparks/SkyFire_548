-- Quest 25187 "Lost in the Floods": Thonk's Spyglass (52514) failed with
-- "Requires Razor Hill Watchtower top". Its spell 73817 needs spell focus 1657,
-- and no gameobject provided it. Past that check, the spell's only effect is
-- a script effect with no handler, so it gave none of the four "find" credits.
-- Spell focus template from the Cataclysm 4.3.4 sniff data (TDB 434.22011);
-- Durotar is unchanged in 5.4.8. Name, type and model of 203084 match the MoP
-- object data; focus id 1657 is the 5.4.8 SpellCastingRequirements value of
-- 73817, and its effect 0 is SPELL_EFFECT_SCRIPT_EFFECT (77) on the caster.
-- No source has a spawn for the focus, so it stands on Thonk's spot.

-- Thonk Spell Focus, spell focus 1657, data1 8 (the core checks half: 4 yards)
DELETE FROM `gameobject_template` WHERE `entry` = 203084;
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `data0`, `data1`, `data2`, `data3`, `WDBVerified`) VALUES
(203084, 8, 2650, 'Thonk Spell Focus', '', '', '', 0, 0, 0.5, 1657, 8, 0, 1, 15595);

-- Spawned on Thonk's spot (39323) at the top of the Razor Hill watchtower
DELETE FROM `gameobject` WHERE `guid` = 200275;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(200275, 203084, 1, 1, 0, 0, 392.245, -4584.6, 76.7233, 0, 0, 0, 0, 1, 1, 255, 1);

-- Spyglass script effect: personal kill credit for the four survivors
-- (Durotar Telescope Quest KC 1-4, the quest's objectives)
DELETE FROM `spell_scripts` WHERE `id` = 73817;
INSERT INTO `spell_scripts` (`id`, `effIndex`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`) VALUES
(73817, 0, 0, 8, 39357, 0, 0, 0, 0, 0, 0),
(73817, 0, 0, 8, 39358, 0, 0, 0, 0, 0, 0),
(73817, 0, 0, 8, 39359, 0, 0, 0, 0, 0, 0),
(73817, 0, 0, 8, 39360, 0, 0, 0, 0, 0, 0);
