-- Quest 31395 "The Celestial Experience" (Kun-Lai Summit): Sunwalker Dezco (64542) gives
-- and ends it, the objectives are credit Xuen (64528) and kills of Spirit of Violence
-- (64656), Spirit of Anger (64684) and Spirit of Hatred (64742). On alexkulya's 5.4.8 core
-- the script npc_xuen_celestial_experience gives the Xuen credit when a player with the
-- quest talks to Xuen, then summons phased copies of Taran Zhu, Dezco, Anduin and Zhi,
-- plays their texts and summons the three spirits one after another at the arena centre
-- (3795.73 533.61 639.0), each time after the previous spirit dies; the spirit AI
-- (celestial_experience_sha) casts Hated Blast (126628), Sha Corruption (126625), Devastation
-- (126631, Anger and Hatred) and Whirlwind of Anger (126633, Hatred). There was no Xuen
-- spawn (the entry binds a script), no gossip and no AI, and the spirits were friendly
-- (faction 35).
-- Here: Xuen's spawn (alexkulya guid 509940), gossip menu 64528 with an option shown while
-- quest 31395 is taken, the opening and the two follow-up stages as SmartAI timed action
-- lists (delays are relative to the previous step) and spirit AI; hostile faction 16 on the
-- spirits (alexkulya's SetFaction value). The spirits tell Xuen when they die (data 1 1,
-- 1 2); the Hatred kill needs no follow-up, its credit completes the quest.
-- Differences from alexkulya: the four defenders already have spawns at their phase 2
-- positions (alexkulya guids 510059, 510058, 509938, 509939), so they talk in place and are
-- not summoned; no phasing (this core phases by id), every player sees the encounter; the
-- sha-fountain pulls that jump the player to the arena are left out (SmartAI cannot jump a
-- player), so the player fights where they stand; the option text has no source in
-- alexkulya (its script acts on gossip hello) and is written here. The quest needs Spirit of
-- Hatred 64742, but alexkulya summons 64744 (Varian) for Alliance players; the quest data and
-- Anduin's text 3 ("looks just like Garrosh") say 64742, so 64742 is summoned and gets
-- the Garrosh transform (126562); 64744 and the Horde quest 31394 (no starter anywhere) stay
-- unscripted. Xuen's lists hold his own lines, the defenders' opening lines, the summons
-- and the defender list starts; each defender runs its own list per stage (started with
-- action 80 when the spirit is summoned), because starting a list replaces the one still
-- running on that creature. A defender line from one stage is only lost if the next stage
-- starts before it is spoken (Zhi's first-stage line, 30.2 s after the first summon, if
-- Violence dies within 14.7 s). Choosing the option again while a trial runs restarts it:
-- the spirits still alive are told to despawn themselves and the opening list starts over.
-- Before this update there was no creature row for 64528 or guid 509940; no gossip_menu,
-- gossip_menu_option, conditions (type 15) or smart_scripts rows for these ids; Xuen had
-- gossip_menu_id 0, npcflag 0, AIName ''; the three spirits had AIName '' and faction 35/35;
-- Taran Zhu, Zhi and Anduin had AIName ''.

DELETE FROM `creature` WHERE `guid` = 509940;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(509940, 64528, 870, 1, 0, 0, 0, 0, 3837.46, 527.898, 639.689, 3.02528, 60, 0, 0, 0, 0, 0, 0, 0, 0);

DELETE FROM `gossip_menu` WHERE `MenuID` = 64528;
INSERT INTO `gossip_menu` (`MenuID`, `TextID`) VALUES
(64528, 68);

DELETE FROM `gossip_menu_option` WHERE `MenuID` = 64528;
INSERT INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcflag`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextID`) VALUES
(64528, 0, 0, 'I am ready for the trial.', 0, 1, 1, 0, 0, 0, 0, '', 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 64528;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 64528, 0, 0, 0, 9, 0, 31395, 0, 0, 0, 0, 0, '', 'Xuen - Show gossip option if quest The Celestial Experience is taken');

UPDATE `creature_template` SET `npcflag` = `npcflag` | 1, `gossip_menu_id` = 64528, `AIName` = 'SmartAI' WHERE `entry` = 64528;
UPDATE `creature_template` SET `AIName` = 'SmartAI', `faction_A` = 16, `faction_H` = 16 WHERE `entry` IN (64656, 64684, 64742);
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (64536, 64537, 64540);

DELETE FROM `smart_scripts` WHERE (`entryorguid` IN (64528, 64656, 64684, 64742) AND `source_type` = 0) OR (`entryorguid` IN (6452800, 6452801, 6452802, 6453600, 6453601, 6453602, 6453700, 6453701, 6453702, 6454000, 6454001, 6454002) AND `source_type` = 9);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(64528, 0, 0, 1, 62, 0, 100, 0, 64528, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Xuen - On gossip select - Close gossip'),
(64528, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 64528, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Xuen - On linked - Quest credit Xuen to the player'),
(64528, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 2, 1, 0, 0, 0, 0, 19, 64656, 200, 0, 0, 0, 0, 0, 'Xuen - On linked - Tell a living Spirit of Violence to despawn (restart)'),
(64528, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 2, 1, 0, 0, 0, 0, 19, 64684, 200, 0, 0, 0, 0, 0, 'Xuen - On linked - Tell a living Spirit of Anger to despawn (restart)'),
(64528, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 2, 1, 0, 0, 0, 0, 19, 64742, 200, 0, 0, 0, 0, 0, 'Xuen - On linked - Tell a living Spirit of Hatred to despawn (restart)'),
(64528, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 6452800, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Xuen - On linked - Run trial opening action list'),
(64528, 0, 6, 0, 38, 0, 100, 0, 1, 1, 0, 0, 0, 80, 6452801, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Xuen - On data 1 1 (Spirit of Violence defeated) - Run second stage action list'),
(64528, 0, 7, 0, 38, 0, 100, 0, 1, 2, 0, 0, 0, 80, 6452802, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Xuen - On data 1 2 (Spirit of Anger defeated) - Run third stage action list'),
(6452800, 9, 0, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Trial opening - After 3 s - Xuen says text 0'),
(6452800, 9, 1, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 19, 64536, 100, 0, 0, 0, 0, 0, 'Trial opening - 6 s later - Taran Zhu says text 0'),
(6452800, 9, 2, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 19, 64540, 100, 0, 0, 0, 0, 0, 'Trial opening - 6 s later - Anduin says text 0'),
(6452800, 9, 3, 0, 0, 0, 100, 0, 6250, 6250, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 19, 64536, 100, 0, 0, 0, 0, 0, 'Trial opening - 6.25 s later - Taran Zhu says text 1'),
(6452800, 9, 4, 0, 0, 0, 100, 0, 14500, 14500, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Trial opening - 14.5 s later - Xuen says text 1'),
(6452800, 9, 5, 0, 0, 0, 100, 0, 15000, 15000, 0, 0, 0, 12, 64656, 3, 600000, 0, 0, 0, 8, 0, 0, 0, 3795.73, 533.61, 639.0, 2.97, 'Trial opening - 15 s later - Summon Spirit of Violence at the arena'),
(6452800, 9, 6, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 80, 6453600, 2, 0, 0, 0, 0, 19, 64536, 100, 0, 0, 0, 0, 0, 'Trial opening - Taran Zhu runs his first stage lines'),
(6452800, 9, 7, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 80, 6454000, 2, 0, 0, 0, 0, 19, 64540, 100, 0, 0, 0, 0, 0, 'Trial opening - Anduin runs his first stage lines'),
(6452800, 9, 8, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 80, 6453700, 2, 0, 0, 0, 0, 19, 64537, 100, 0, 0, 0, 0, 0, 'Trial opening - Zhi runs his first stage lines'),
(6452801, 9, 0, 0, 0, 0, 100, 0, 5500, 5500, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Second stage - After 5.5 s - Xuen says text 2'),
(6452801, 9, 1, 0, 0, 0, 100, 0, 10000, 10000, 0, 0, 0, 12, 64684, 3, 600000, 0, 0, 0, 8, 0, 0, 0, 3795.73, 533.61, 639.0, 2.97, 'Second stage - 10 s later - Summon Spirit of Anger at the arena'),
(6452801, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 80, 6453601, 2, 0, 0, 0, 0, 19, 64536, 100, 0, 0, 0, 0, 0, 'Second stage - Taran Zhu runs his stage lines'),
(6452801, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 80, 6454001, 2, 0, 0, 0, 0, 19, 64540, 100, 0, 0, 0, 0, 0, 'Second stage - Anduin runs his stage lines'),
(6452801, 9, 4, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 80, 6453701, 2, 0, 0, 0, 0, 19, 64537, 100, 0, 0, 0, 0, 0, 'Second stage - Zhi runs his stage lines'),
(6452802, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Third stage - Xuen says text 3'),
(6452802, 9, 1, 0, 0, 0, 100, 0, 17000, 17000, 0, 0, 0, 12, 64742, 3, 600000, 0, 0, 0, 8, 0, 0, 0, 3795.73, 533.61, 639.0, 2.97, 'Third stage - 17 s later - Summon Spirit of Hatred at the arena'),
(6452802, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 80, 6453602, 2, 0, 0, 0, 0, 19, 64536, 100, 0, 0, 0, 0, 0, 'Third stage - Taran Zhu runs his stage lines'),
(6452802, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 80, 6454002, 2, 0, 0, 0, 0, 19, 64540, 100, 0, 0, 0, 0, 0, 'Third stage - Anduin runs his stage lines'),
(6452802, 9, 4, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 80, 6453702, 2, 0, 0, 0, 0, 19, 64537, 100, 0, 0, 0, 0, 0, 'Third stage - Zhi runs his stage lines'),
(6453600, 9, 0, 0, 0, 0, 100, 0, 12000, 12000, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Taran Zhu stage 1 - 12 s after the summon - Taran Zhu says text 2'),
(6454000, 9, 0, 0, 0, 0, 100, 0, 18000, 18000, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Anduin stage 1 - 18 s after the summon - Anduin says text 1'),
(6453700, 9, 0, 0, 0, 0, 100, 0, 30200, 30200, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Zhi stage 1 - 30.2 s after the summon - Zhi says text 0'),
(6453601, 9, 0, 0, 0, 0, 100, 0, 20100, 20100, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Taran Zhu stage 2 - 20.1 s after the summon - Taran Zhu says text 3'),
(6454001, 9, 0, 0, 0, 0, 100, 0, 6600, 6600, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Anduin stage 2 - 6.6 s after the summon - Anduin says text 2'),
(6453701, 9, 0, 0, 0, 0, 100, 0, 24600, 24600, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Zhi stage 2 - 24.6 s after the summon - Zhi says text 1'),
(6453602, 9, 0, 0, 0, 0, 100, 0, 14000, 14000, 0, 0, 0, 1, 4, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Taran Zhu stage 3 - 14 s after the summon - Taran Zhu says text 4'),
(6454002, 9, 0, 0, 0, 0, 100, 0, 14000, 14000, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Anduin stage 3 - 14 s after the summon - Anduin says text 3'),
(6454002, 9, 1, 0, 0, 0, 100, 0, 9300, 9300, 0, 0, 0, 1, 4, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Anduin stage 3 - 9.3 s later - Anduin says text 4'),
(6454002, 9, 2, 0, 0, 0, 100, 0, 13700, 13700, 0, 0, 0, 1, 5, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Anduin stage 3 - 13.7 s later - Anduin says text 5'),
(6453702, 9, 0, 0, 0, 0, 100, 0, 30000, 30000, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Zhi stage 3 - 30 s after the summon - Zhi says text 2'),
(64656, 0, 0, 0, 54, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 21, 200, 0, 0, 0, 0, 0, 0, 'Spirit of Violence - On just summoned - Attack the closest player'),
(64656, 0, 1, 0, 0, 0, 100, 0, 2000, 3000, 8000, 12000, 0, 11, 126628, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Spirit of Violence - In combat - Cast Hated Blast on victim'),
(64656, 0, 2, 0, 0, 0, 100, 0, 8000, 8000, 16000, 16000, 0, 11, 126625, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Spirit of Violence - In combat - Cast Sha Corruption on victim'),
(64656, 0, 3, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 19, 64528, 200, 0, 0, 0, 0, 0, 'Spirit of Violence - On death - Closest Xuen data 1 1'),
(64656, 0, 4, 0, 38, 0, 100, 0, 2, 1, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Spirit of Violence - On data 2 1 (restart) - Despawn'),
(64684, 0, 0, 0, 54, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 21, 200, 0, 0, 0, 0, 0, 0, 'Spirit of Anger - On just summoned - Attack the closest player'),
(64684, 0, 1, 0, 0, 0, 100, 0, 2000, 3000, 8000, 12000, 0, 11, 126628, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Spirit of Anger - In combat - Cast Hated Blast on victim'),
(64684, 0, 2, 0, 0, 0, 100, 0, 8000, 8000, 16000, 16000, 0, 11, 126625, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Spirit of Anger - In combat - Cast Sha Corruption on victim'),
(64684, 0, 3, 0, 0, 0, 100, 0, 13000, 13000, 25000, 30000, 0, 11, 126631, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Spirit of Anger - In combat - Cast Devastation'),
(64684, 0, 4, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 45, 1, 2, 0, 0, 0, 0, 19, 64528, 200, 0, 0, 0, 0, 0, 'Spirit of Anger - On death - Closest Xuen data 1 2'),
(64684, 0, 5, 0, 38, 0, 100, 0, 2, 1, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Spirit of Anger - On data 2 1 (restart) - Despawn'),
(64742, 0, 0, 0, 54, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 21, 200, 0, 0, 0, 0, 0, 0, 'Spirit of Hatred - On just summoned - Attack the closest player'),
(64742, 0, 1, 0, 0, 0, 100, 0, 2000, 3000, 8000, 12000, 0, 11, 126628, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Spirit of Hatred - In combat - Cast Hated Blast on victim'),
(64742, 0, 2, 0, 0, 0, 100, 0, 8000, 8000, 16000, 16000, 0, 11, 126625, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Spirit of Hatred - In combat - Cast Sha Corruption on victim'),
(64742, 0, 3, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 11, 126562, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Spirit of Hatred - On aggro - Cast Garrosh transform'),
(64742, 0, 4, 0, 0, 0, 100, 0, 13000, 13000, 25000, 25000, 0, 11, 126633, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Spirit of Hatred - In combat - Cast Whirlwind of Anger'),
(64742, 0, 5, 0, 0, 0, 100, 0, 23000, 23000, 25000, 30000, 0, 11, 126631, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Spirit of Hatred - In combat - Cast Devastation'),
(64742, 0, 6, 0, 38, 0, 100, 0, 2, 1, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Spirit of Hatred - On data 2 1 (restart) - Despawn');
