-- Quests 30133 and 30178 "Into the Wilds" (Krasarang Wilds, level 86, Alliance
-- and Horde variants): the one objective is "Zhu's Watch Courier Found", credited
-- to Zhu's Watch Courier (59151). On alexkulya's 5.4.8 core the script
-- npc_zhus_watch_courier gives the credit to any player who comes within 7
-- yards of the courier. TEST never spawned the courier (the spawn port skips
-- entries that name a script) and the template had no AI, so neither quest could
-- complete.
-- Here: the courier's spawn (alexkulya guid 517850, default phase, wandering 4
-- yards) and SmartAI that gives the credit to a player within 7 yards, checked
-- once a second.
-- Values before this update, queried on TEST mopworld on 2026-11-12: no creature
-- with id 59151 or guid 517850, no smart_scripts rows for 59151, template
-- AIName ''.
DELETE FROM `creature` WHERE `guid` = 517850;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(517850, 59151, 870, 1, 0, 0, 0, 0, -905.767, 16.0764, 20.936, 3.28122, 60, 4, 0, 0, 0, 1, 0, 0, 0);

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 59151;

DELETE FROM `smart_scripts` WHERE `entryorguid` = 59151 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(59151, 0, 0, 0, 10, 0, 100, 0, 1, 7, 1000, 1000, 1, 33, 59151, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Zhu''s Watch Courier - Player within 7 yards - Player gets quest credit');
