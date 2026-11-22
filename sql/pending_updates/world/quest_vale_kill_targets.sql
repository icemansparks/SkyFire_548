-- Vale of Eternal Blossoms kill targets that alexkulya's 5.4.8 core binds to a C++ script
-- (npc_bloodtip, npc_quid, npc_aetha, npc_gochao_the_iron_fist, npc_general_temuja). These
-- creatures had no spawn, so the kill objectives of "The Ashweb Matriarch" (30225),
-- "Quid Pro Quo" (30235), "Aetha" (30236), "The Imperion Threat" (30302) and "Behind Our
-- Lines" (31136) had no target. The scripts only add combat casts, so the kills work once
-- the mobs exist. Adds alexkulya's five spawns (same guids, default phase). General Temuja
-- (63101) was faction 35 (friendly, not attackable); alexkulya has 2480.
-- Before this update no creature rows existed for 58474, 58771, 58778, 62880 and 63101, and
-- creature_template 63101 had faction_A = faction_H = 35.
DELETE FROM `creature` WHERE `guid` IN (533854,532040,532745,533821,535335);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(533854, 58474, 870, 1, 0, 0, 0, 0, 1700.17, 1794.99, 252.215, 2.74962, 1800, 0, 0, 0, 0, 0, 0, 0, 0),
(532040, 58771, 870, 1, 0, 0, 0, 0, 1284.43, 796.403, 386.944, 4.52135, 1800, 7.786, 0, 0, 0, 1, 0, 0, 0),
(532745, 58778, 870, 1, 0, 0, 0, 0, 429.816, 1576.18, 516.558, 1.57576, 1800, 0, 0, 0, 0, 0, 0, 0, 0),
(533821, 62880, 870, 1, 0, 0, 0, 0, 1599.36, 1799.89, 266.794, 1.15289, 1800, 0, 0, 0, 0, 0, 0, 0, 0),
(535335, 63101, 870, 1, 0, 0, 0, 0, 968.887, 1712.18, 350.203, 0.827925, 1800, 0, 0, 0, 0, 0, 0, 0, 0);
UPDATE `creature_template` SET `faction_A` = 2480, `faction_H` = 2480 WHERE `entry` = 63101;
