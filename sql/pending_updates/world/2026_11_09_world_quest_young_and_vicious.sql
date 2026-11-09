-- Quest 24626 "Young and Vicious" (troll start, Echo Isles): Swiftclaw stood
-- still at Kijara and the quest could not be done. SFDB had no script, path,
-- vehicle or condition for the quest's raptor; the lasso (item 50053, spell
-- 70927 "Raptor Rope") hit nothing and neither objective could be credited.
--
-- Flow, from the alexkulya 5.4.8 world DB (2026_08_16): accepting the quest at
-- Kijara (37969) makes the player cast 70898 "Summon Named Raptor", which
-- summons a personal Swiftclaw (38002, vehicle 617) that runs the island path.
-- The lasso on him seats the player through 48678 (control vehicle, seat 1 of
-- vehicle 617 is the control seat) and gives "Capture Swiftclaw" through 70943
-- (kill credit 37989). The player steers him to the raptor pens; Notera (38988)
-- or Kortin (38990) cast 70941 "Raptor Turn-in Credit" (kill credit 38002) on
-- the rider within 10 yards and pull them off. All spell effects cross-checked
-- against the 5.4.8 DBC. The static spawn 37989 gets alexkulya's (and the Cata
-- TDB's) Generic Quest Invisibility 1: it is the credit entry only.
--
-- Changes against alexkulya:
-- * The lasso mounts the player on the raptor that was hit (target self), not
--   the closest 38002 within 30 yards, which could be another player's.
-- * The path does not repeat (as in alexkulya): with repeat on, SkyFire's
--   path stop restarts the path and the ridden raptor would run off. After
--   the loop he waits at the last point, still in lasso range.
-- * alexkulya's "combat stop" action (203) does not exist in SkyFire; dropped.
-- * Cast flag 0x10 is not implemented in SkyFire; the turn-in credit is cast
--   triggered instead, and the line-of-sight event only reacts to players.
-- * A player who lost his raptor (fell off before the pens) gets a new one
--   when he comes within 10 yards of Kijara with the quest taken and no
--   Swiftclaw near; alexkulya only summons on accept.
-- * Swiftclaw is no quest giver in 5.4.8 (alexkulya npcflag 0, no
--   creature_queststarter rows); SFDB's Cata-era rows for 37989 and 38002 go.
--
-- Values before this update, queried on TEST mopworld (SFDB 26.002) on
-- 2026-11-09: AIName '' for 37969, 37989, 38002, 38988, 38990; 37989 and 38002
-- npcflag 2, 38002 VehicleId 0, speed_walk 1, speed_run 1.14286; no
-- creature_template_addon for 37989; no smart_scripts, waypoints,
-- creature_text or conditions for these entries or spell 70927.
-- The waypoints table loads only at worldserver start.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (37969, 38988, 38990);
UPDATE `creature_template` SET `npcflag` = 0 WHERE `entry` = 37989;
UPDATE `creature_template` SET `AIName` = 'SmartAI', `npcflag` = 0, `VehicleId` = 617, `speed_walk` = 1.6, `speed_run` = 1.71429 WHERE `entry` = 38002;

DELETE FROM `creature_queststarter` WHERE `quest` = 24626 AND `id` IN (37989, 38002);

DELETE FROM `creature_template_addon` WHERE `entry` = 37989;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(37989, 0, 0, 0, 1, 0, '49414');

DELETE FROM `creature_text` WHERE `entry` = 38002;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(38002, 0, 0, 'Swiftclaw isn\'t stopping!  Steer him back to the raptor pens near Darkspear Hold.', 41, 0, 100, 0, 0, 0, 'Swiftclaw - On Passenger Boarded');

DELETE FROM `locales_creature_text` WHERE `entry` = 38002;
INSERT INTO `locales_creature_text` (`entry`, `groupid`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(38002, 0, 0, '날쌘발톱이 멈추지 않습니다! 검은창 요새 근처에 있는 랩터 우리로 몰아넣으십시오.', 'Vivegriffe ne s\'arrête pas ! Faites-le retourner à l\'enclos à raptors près du bastion des Sombrelances.', 'Flinkklaue hält nicht ein! Lenkt ihn zurück zu den Raptorenställen, in der Nähe der Dunkelspeerfeste.', '迅爪不会停下来！控制它返回暗矛要塞旁边的迅猛龙围栏。', '迅爪停不下來!將牠拉回靠近暗矛要塞的迅猛龍圍欄那。', '¡Garrapresta no va a parar! Condúcelo de nuevo al Cercado de Raptores cerca del Bastión Lanza Negra.', '¡Garrapresta no va a parar! Condúcelo de nuevo al Cercado de Raptores cerca del Bastión Lanza Negra.', 'Стремительный Коготь не останавливается! Направляй его и доберись до самого логова, что находится рядом с крепостью Черного Копья.');

DELETE FROM `waypoints` WHERE `entry` = 38002;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(38002, 1, -1599.41, -5349.48, 6.14603, 'Swiftclaw'),
(38002, 2, -1576.45, -5351.05, 6.13749, 'Swiftclaw'),
(38002, 3, -1562.01, -5336.09, 7.90577, 'Swiftclaw'),
(38002, 4, -1549.8, -5324.86, 7.53791, 'Swiftclaw'),
(38002, 5, -1532.8, -5335.1, 6.86731, 'Swiftclaw'),
(38002, 6, -1542.22, -5354.58, 5.66456, 'Swiftclaw'),
(38002, 7, -1531.81, -5367.34, 4.45769, 'Swiftclaw'),
(38002, 8, -1516.96, -5356.29, 6.37049, 'Swiftclaw'),
(38002, 9, -1509.05, -5333.04, 5.51505, 'Swiftclaw'),
(38002, 10, -1531.29, -5315.33, 7.24088, 'Swiftclaw'),
(38002, 11, -1552.43, -5298.37, 9.10266, 'Swiftclaw'),
(38002, 12, -1576.74, -5285.85, 8.21355, 'Swiftclaw'),
(38002, 13, -1585.99, -5272.23, 7.84856, 'Swiftclaw'),
(38002, 14, -1561.39, -5265.12, 7.27806, 'Swiftclaw'),
(38002, 15, -1526.95, -5293.34, 7.03678, 'Swiftclaw'),
(38002, 16, -1513.42, -5261.77, 6.02278, 'Swiftclaw'),
(38002, 17, -1529.59, -5251.99, 5.53645, 'Swiftclaw'),
(38002, 18, -1546.02, -5244.1, 5.90306, 'Swiftclaw'),
(38002, 19, -1571.21, -5227.31, 2.96685, 'Swiftclaw'),
(38002, 20, -1588.76, -5226.84, 2.73052, 'Swiftclaw'),
(38002, 21, -1592.73, -5240.25, 4.78311, 'Swiftclaw'),
(38002, 22, -1587.37, -5256.42, 6.22281, 'Swiftclaw'),
(38002, 23, -1558.64, -5270.48, 7.74767, 'Swiftclaw'),
(38002, 24, -1550.73, -5285.78, 8.72766, 'Swiftclaw'),
(38002, 25, -1562.5, -5319.79, 8.16291, 'Swiftclaw'),
(38002, 26, -1584.17, -5326.92, 7.51514, 'Swiftclaw'),
(38002, 27, -1589.43, -5312.1, 7.5586, 'Swiftclaw'),
(38002, 28, -1588.48, -5282.4, 7.86883, 'Swiftclaw'),
(38002, 29, -1631.59, -5272.9, 3.32178, 'Swiftclaw'),
(38002, 30, -1633.12, -5292.29, 3.2417, 'Swiftclaw'),
(38002, 31, -1606.47, -5307.9, 6.64991, 'Swiftclaw'),
(38002, 32, -1603.61, -5325.47, 6.39991, 'Swiftclaw'),
(38002, 33, -1647.53, -5337.8, 0.968718, 'Swiftclaw'),
(38002, 34, -1646.44, -5353.68, 1.35641, 'Swiftclaw'),
(38002, 35, -1613.68, -5353.11, 4.66571, 'Swiftclaw');

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (37969, 38002, 38988, 38990) AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(37969, 0, 0, 0, 19, 0, 100, 0, 24626, 0, 0, 0, 0, 86, 70898, 0, 7, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kijara - On Quest Accepted - Invoker Casts Summon Named Raptor'),
(37969, 0, 1, 0, 10, 0, 100, 0, 1, 10, 10000, 10000, 1, 86, 70898, 0, 7, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kijara - OOC LoS Player - Invoker Casts Summon Named Raptor'),
(38002, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 0, 62, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, -1565.1, -5319.76, 8.37026, 0.61391, 'Swiftclaw - On Just Summoned - Teleport'),
(38002, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 81, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Swiftclaw - Linked - Set Npc Flag 0'),
(38002, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 53, 1, 38002, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Swiftclaw - Linked - Start Path (run)'),
(38002, 0, 3, 4, 8, 0, 100, 0, 70927, 0, 0, 0, 0, 86, 48678, 0, 7, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Swiftclaw - On Spellhit Raptor Rope - Invoker Rides Self'),
(38002, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 86, 70943, 2, 7, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Swiftclaw - Linked - Invoker Casts Raptor Lasso Credit'),
(38002, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 55, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Swiftclaw - Linked - Stop Path'),
(38002, 0, 6, 0, 27, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Swiftclaw - On Passenger Boarded - Say Line 0'),
(38002, 0, 7, 8, 28, 0, 100, 0, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Swiftclaw - On Passenger Removed - Set Run'),
(38002, 0, 8, 9, 61, 0, 100, 0, 0, 0, 0, 0, 0, 89, 10, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Swiftclaw - Linked - Random Move'),
(38002, 0, 9, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 5000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Swiftclaw - Linked - Despawn In 5 Seconds'),
(38988, 0, 0, 1, 10, 0, 100, 0, 1, 10, 1000, 1000, 1, 11, 70941, 2, 0, 0, 0, 0, 21, 10, 0, 0, 0, 0, 0, 0, 'Notera - OOC LoS Player - Cast Raptor Turn-in Credit'),
(38988, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 28, 48678, 0, 0, 0, 0, 0, 11, 38002, 10, 0, 0, 0, 0, 0, 'Notera - Linked - Remove Swiftclaw Ride'),
(38990, 0, 0, 1, 10, 0, 100, 0, 1, 10, 1000, 1000, 1, 11, 70941, 2, 0, 0, 0, 0, 21, 10, 0, 0, 0, 0, 0, 0, 'Kortin - OOC LoS Player - Cast Raptor Turn-in Credit'),
(38990, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 28, 48678, 0, 0, 0, 0, 0, 11, 38002, 10, 0, 0, 0, 0, 0, 'Kortin - Linked - Remove Swiftclaw Ride');

-- Kijara summons only when no Swiftclaw is near the player; Notera and Kortin
-- credit only a player with a Swiftclaw near; the lasso hits only 38002.
-- Kijara's re-summon needs the quest taken (incomplete) and no Swiftclaw near.
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` IN (1, 2) AND `SourceEntry` IN (37969, 38988, 38990) AND `SourceId` = 0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 17 AND `SourceEntry` = 70927;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(22, 1, 37969, 0, 0, 29, 0, 38002, 50, 0, 1, 0, 0, '', 'Kijara - summon Swiftclaw only if none is near the player'),
(22, 2, 37969, 0, 0, 9, 0, 24626, 0, 0, 0, 0, 0, '', 'Kijara - re-summon only with Young and Vicious taken'),
(22, 2, 37969, 0, 0, 29, 0, 38002, 50, 0, 1, 0, 0, '', 'Kijara - re-summon only if no Swiftclaw is near the player'),
(22, 1, 38988, 0, 0, 29, 0, 38002, 50, 0, 0, 0, 0, '', 'Notera - turn-in credit only with Swiftclaw near'),
(22, 1, 38990, 0, 0, 29, 0, 38002, 50, 0, 0, 0, 0, '', 'Kortin - turn-in credit only with Swiftclaw near'),
(17, 0, 70927, 0, 0, 31, 1, 3, 38002, 0, 0, 0, 0, '', 'Raptor Rope - only on Swiftclaw');
