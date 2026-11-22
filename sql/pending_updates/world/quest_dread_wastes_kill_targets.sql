-- Dread Wastes kill targets that alexkulya's 5.4.8 core binds to a C++ script
-- (npc_adjunct_kree_zot, npc_hulking_kor_thik_fleshrender, npc_shox_tik,
-- npc_manifestation_of_terror). These creatures had no spawn, so "Psycho Mantid" (31003,
-- Adjunct Kree'zot 62301), "The Taking of Dusklight Bridge" (30975, Hulking Kor'thik
-- Fleshrender 62686) and "The Wrath of Shek'zeer" (31609, Shox'tik 66784 and
-- Manifestation of Terror 66785) had no target. The scripts only add combat casts, so the
-- kills work once the mobs exist; all four templates are already faction 14. This adds
-- alexkulya's four spawns (same guids, default phase).
-- Before this update no creature rows existed for 62301, 62686, 66784 and 66785.
DELETE FROM `creature` WHERE `guid` IN (526841,527534,530883,530915);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(526841, 62301, 870, 1, 0, 0, 0, 0, 412.47, 2244.05, 245.24, 1.69861, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(527534, 62686, 870, 1, 0, 0, 0, 0, 1056.68, 3826.33, 254.43, 0.229554, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(530883, 66784, 870, 1, 0, 0, 0, 0, 7.30177, 3882.03, 193.23, 2.75548, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(530915, 66785, 870, 1, 0, 0, 0, 0, 435.306, 3803.44, 218.138, 5.52527, 60, 6, 0, 0, 0, 1, 0, 0, 0);
