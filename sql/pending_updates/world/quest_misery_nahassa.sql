-- Quest 30691 "Misery" (Krasarang Wilds, level 86): the objective is "Put
-- Nahassa out of his misery", kill credit for Nahassa (60203). alexkulya's
-- script npc_nahassa shows him dying (Injured Anim 116342 and Dying 116348 on
-- spawn and when he returns home), switches both off when combat starts, and
-- has him cast Jaw Snap (118990) on his victim at once then every 10 to 14 s,
-- and Feverish Deathbite (116343) after 5 s then every 13 to 20 s. The world
-- DB never spawned him, and his template had faction 35 (friendly, so the
-- quest target could not be attacked) and no AI. Here: the spawn (alexkulya
-- guid 519155, default phase), faction 1925 as alexkulya, SmartAI with the
-- two auras on reset, no health regeneration, the auras removed on aggro, and
-- the two casts. Differences from alexkulya: alexkulya also sets his health
-- to 429970 on reset; SmartAI has no absolute health action, so he keeps the
-- template's full health and the fight is longer. Before this update: no
-- creature with id 60203 or guid 519155, no smart_scripts rows for 60203,
-- template faction_A and faction_H 35, AIName ''.
DELETE FROM `creature` WHERE `guid` = 519155;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(519155, 60203, 870, 1, 0, 0, 0, 0, -2722.27, 784.854, -88.5042, 0.0600478, 60, 0, 0, 0, 0, 0, 0, 0, 0);

UPDATE `creature_template` SET `faction_A` = 1925, `faction_H` = 1925, `AIName` = 'SmartAI' WHERE `entry` = 60203;

DELETE FROM `smart_scripts` WHERE `entryorguid` = 60203 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(60203, 0, 0, 1, 25, 0, 100, 0, 0, 0, 0, 0, 0, 102, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Nahassa - On reset - No health regeneration'),
(60203, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 116342, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Nahassa - On linked - Cast Injured Anim'),
(60203, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 116348, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Nahassa - On linked - Cast Dying'),
(60203, 0, 3, 4, 4, 0, 100, 0, 0, 0, 0, 0, 0, 28, 116342, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Nahassa - On aggro - Remove Injured Anim'),
(60203, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 28, 116348, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Nahassa - On linked - Remove Dying'),
(60203, 0, 5, 0, 0, 0, 100, 0, 0, 0, 10000, 14000, 0, 11, 118990, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Nahassa - In combat - Cast Jaw Snap'),
(60203, 0, 6, 0, 0, 0, 100, 0, 5000, 5000, 13000, 20000, 0, 11, 116343, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Nahassa - In combat - Cast Feverish Deathbite');
