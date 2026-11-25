-- Quest 30273 "In the House of the Red Crane" (Krasarang Wilds, level 86): the
-- objective is to kill Sha of Despair (59651). alexkulya's script
-- npc_sha_of_despair stands the boss still and has it cast Call of Despair
-- (114849) after 8 to 20 s then every 18 to 36 s, and Gloom Whirl (130288) after
-- 13 s then every 19 s. TEST never spawned the boss (the spawn port skips
-- entries that name a script), so the quest could not complete. The template
-- already has AIName SmartAI (faction 16, unit_flags 512, as alexkulya) but no
-- smart_scripts rows.
-- Here: the spawn (alexkulya guid 518597, default phase) and SmartAI with the
-- two casts and no combat movement. Left out: despawning the spell's summons
-- when the boss dies.
-- Values before this update, queried on TEST mopworld on 2026-11-12: no creature
-- with id 59651 or guid 518597 and no smart_scripts rows for 59651.
DELETE FROM `creature` WHERE `guid` = 518597;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(518597, 59651, 870, 1, 0, 0, 0, 0, -1814.57, 1052.94, -31.7273, 3.14269, 60, 0, 0, 0, 0, 0, 0, 0, 0);

DELETE FROM `smart_scripts` WHERE `entryorguid` = 59651 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(59651, 0, 0, 0, 25, 0, 100, 0, 0, 0, 0, 0, 0, 21, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sha of Despair - On reset - No combat movement'),
(59651, 0, 1, 0, 0, 0, 100, 0, 8000, 20000, 18000, 36000, 0, 11, 114849, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sha of Despair - In combat - Cast Call of Despair'),
(59651, 0, 2, 0, 0, 0, 100, 0, 13000, 13000, 19000, 19000, 0, 11, 130288, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sha of Despair - In combat - Cast Gloom Whirl');
