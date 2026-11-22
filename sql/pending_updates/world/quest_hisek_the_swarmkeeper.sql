-- Dread Wastes, Hisek the Swarmkeeper chain: Dropping Our Signal (31439), Corruption Runs
-- Deep (31441) and Extracting Answers (31465). Hisek 64569 (starts 31441, ends 31439) had
-- no spawn, and 64705 (ends 31441, starts 31465) is only ever summoned by alexkulya's
-- npc_hisek_the_swarmkeeper script, so none of the three could be handed in or started.
-- alexkulya's script plays a phased scene on accepting 31441 (a summoned Hisek double and
-- a Kor'ik that turns into the Klaxxi Traitor, 64583) and relies on SetExplicitSeerGuid,
-- which SkyFire lacks. The Klaxxi Traitor is faction 35 in both DBs and cannot be killed.
-- Approximation without the scene: spawn Hisek 64569 at alexkulya's position, spawn 64705
-- at the same spot, and give the kill credit for 64583 when 31441 is accepted.
-- Damage Control (31458) stays blocked: item 87269 has no loot source in either DB.
-- Before this update no creature rows existed for guids 526641 and 526642, no smart_scripts
-- existed for 64569, and its AIName was empty.
DELETE FROM `creature` WHERE `guid` IN (526641,526642);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(526641, 64569, 870, 1, 0, 0, 0, 0, -586.184, 3035.83, 185.935, 5.59798, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(526642, 64705, 870, 1, 0, 0, 0, 0, -586.184, 3035.83, 185.935, 5.59798, 300, 0, 0, 0, 0, 0, 0, 0, 0);
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 64569;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 64569 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(64569, 0, 0, 0, 19, 0, 100, 0, 31441, 0, 0, 0, 0, 33, 64583, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On accepting 31441 - Give credit for the Klaxxi Traitor');
