-- Quest 29630 "And a Heavy Fist" (Jade Forest, level 85): kill 10 Greenwood
-- Tricksters (54987). The world DB had the template but no spawn, and
-- alexkulya binds npc_greenwood_trickster to it. That script only casts one
-- of four tricks every 15 seconds in combat (115809, 115814, 115816 on
-- itself, 115825 on its victim), starting 5 seconds into the fight; this
-- update does the same with SmartAI and adds the 15 default-phase spawns of
-- the 5.4.8 world DB (alexkulya guids). The four spells exist in the 5.4.8
-- Spell.dbc. Before this update: no creature of id 54987, none of the guids,
-- no smart_scripts for 54987 or 5498700-5498703, AIName '' and ScriptName ''.
-- Spawn rows: the 5.4.8 rows of phase 1 and 65535 both show in the default
-- phase; rows that wander use alexkulya's 7.786 yards, the other four stand
-- still. Health is left to the template (curhealth 0).
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 54987;

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 54987;
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` BETWEEN 5498700 AND 5498703;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(54987, 0, 0, 0, 0, 0, 100, 0, 5000, 5000, 15000, 15000, 0, 87, 5498700, 5498701, 5498702, 5498703, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Greenwood Trickster - In combat - Run a random trick'),
(5498700, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 115809, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Greenwood Trickster - Trick 1 - Cast Trickery on self'),
(5498701, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 115814, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Greenwood Trickster - Trick 2 - Cast Trickery on self'),
(5498702, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 115816, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Greenwood Trickster - Trick 3 - Cast Trickery on self'),
(5498703, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 115825, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Greenwood Trickster - Trick 4 - Cast Trickery on victim');

DELETE FROM `creature` WHERE `guid` IN (500475, 500486, 500487, 500488, 501861, 501863, 501867, 501868, 501869, 501870, 501871, 501874, 501875, 501876, 501879);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(500475, 54987, 870, 1, 0, 0, 0, 0, 2924.44, -1114.6, 259.474, 2.01769, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(500486, 54987, 870, 1, 0, 0, 0, 0, 2892.59, -1163.08, 300.147, 5.65487, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(500487, 54987, 870, 1, 0, 0, 0, 0, 2923.93, -1164.45, 276.435, 0.168075, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(500488, 54987, 870, 1, 0, 0, 0, 0, 2934.77, -1141.66, 262.652, 1.53231, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(501861, 54987, 870, 1, 0, 0, 0, 0, 2620.96, -1195.66, 372.956, 5.40863, 60, 7.786, 0, 0, 0, 1, 0, 0, 0),
(501863, 54987, 870, 1, 0, 0, 0, 0, 2669.39, -1134.43, 354.536, 4.0175, 60, 7.786, 0, 0, 0, 1, 0, 0, 0),
(501867, 54987, 870, 1, 0, 0, 0, 0, 2680.26, -1247.44, 378.357, 0, 60, 7.786, 0, 0, 0, 1, 0, 0, 0),
(501868, 54987, 870, 1, 0, 0, 0, 0, 2816.31, -1181.36, 330.989, 0, 60, 7.786, 0, 0, 0, 1, 0, 0, 0),
(501869, 54987, 870, 1, 0, 0, 0, 0, 2790.75, -1152.45, 318.554, 1.7883, 60, 7.786, 0, 0, 0, 1, 0, 0, 0),
(501870, 54987, 870, 1, 0, 0, 0, 0, 2784.62, -1229.61, 343.493, 1.44721, 60, 7.786, 0, 0, 0, 1, 0, 0, 0),
(501871, 54987, 870, 1, 0, 0, 0, 0, 2846.71, -1151.19, 326.069, 0, 60, 7.786, 0, 0, 0, 1, 0, 0, 0),
(501874, 54987, 870, 1, 0, 0, 0, 0, 2787.93, -1225.81, 341.757, 1.94626, 60, 7.786, 0, 0, 0, 1, 0, 0, 0),
(501875, 54987, 870, 1, 0, 0, 0, 0, 2754.95, -1194.71, 351.308, 5.16206, 60, 7.786, 0, 0, 0, 1, 0, 0, 0),
(501876, 54987, 870, 1, 0, 0, 0, 0, 2864.72, -1139.91, 310.583, 6.06956, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(501879, 54987, 870, 1, 0, 0, 0, 0, 2632.11, -1162.52, 362.481, 4.29942, 60, 7.786, 0, 0, 0, 1, 0, 0, 0);
