-- Quest 30174 "For Family" (Krasarang Wilds, level 86): the objective is
-- "kill Groundbreaker Brojai" (58224), who has no spawn because alexkulya's
-- scripts summon him. On their 5.4.8 core Kor Bloodtusk (58670, while 30174
-- is incomplete) shows the option "It's a trap!"; the pick summons Sunwalker
-- Dezco (58666), Lorekeeper Vaeldrin (58975) and Brojai for 180 seconds.
-- Brojai is pacified and immune to players, walks 5 yards, speaks, becomes
-- attackable and kills Kor; Dezco shouts. When Brojai dies, Vaeldrin and
-- Dezco speak three more lines. The world DB had no Kor spawn, no gossip flag
-- or menu on him, no AI on any of the four, and Brojai and Dezco (58666) were
-- level 1 with faction 35, so the quest could not be completed. Here: Kor's
-- spawn from alexkulya (guid 518117); gossip menu 58670 (generic npc_text 68)
-- with the option shown only while 30174 is incomplete; SmartAI on Kor,
-- Brojai and Dezco; Brojai and Dezco (58666) to level 90 and Brojai to
-- faction 16, as on alexkulya. The creature_text rows for 58224, 58666 and
-- 58975 were already in the world DB and none are added. Differences from
-- alexkulya: the three summons belong to Kor, not to the player who picked
-- the option, so another player can attack Brojai too; the 5 yard walk is
-- waited out with a fixed 1 second instead of the spline length; Dezco runs
-- the aftermath lines (including Vaeldrin's) from one timed list on a data
-- trigger from Brojai's death; Brojai kills Kor, and his respawn runs
-- SmartAI::JustRespawned, which calls Reset and so raises the reset event
-- that gives the gossip flag back. Before this update: no creature with id
-- 58670 and no guid 518117; 58670 npcflag 0, gossip_menu_id 0, AIName '';
-- 58224 minlevel and maxlevel 1, faction_A and faction_H 35, AIName ''; 58666
-- minlevel and maxlevel 1, AIName ''; no gossip_menu, gossip_menu_option,
-- conditions (type 15) or smart_scripts rows for these ids.
DELETE FROM `creature` WHERE `guid` = 518117;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(518117, 58670, 870, 1, 0, 0, 0, 0, -1638.58, 1840.27, -2.17, 1.65684, 300, 0, 0, 0, 0, 0, 0, 0, 0);

DELETE FROM `gossip_menu` WHERE `MenuID` = 58670;
INSERT INTO `gossip_menu` (`MenuID`, `TextID`) VALUES
(58670, 68);

DELETE FROM `gossip_menu_option` WHERE `MenuID` = 58670;
INSERT INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcflag`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextID`) VALUES
(58670, 0, 0, 'It\'s a trap!', 0, 1, 1, 0, 0, 0, 0, '', 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 58670;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 58670, 0, 0, 0, 47, 0, 30174, 8, 0, 0, 0, 0, '', 'Kor Bloodtusk - Show gossip option while For Family is incomplete');

UPDATE `creature_template` SET `npcflag` = `npcflag` | 1, `gossip_menu_id` = `entry`, `AIName` = 'SmartAI' WHERE `entry` = 58670;
UPDATE `creature_template` SET `minlevel` = 90, `maxlevel` = 90, `faction_A` = 16, `faction_H` = 16, `AIName` = 'SmartAI' WHERE `entry` = 58224;
UPDATE `creature_template` SET `minlevel` = 90, `maxlevel` = 90, `AIName` = 'SmartAI' WHERE `entry` = 58666;

DELETE FROM `smart_scripts` WHERE (`entryorguid` IN (58670, 58224, 58666) AND `source_type` = 0) OR (`entryorguid` IN (5822401, 5866601) AND `source_type` = 9);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(58670, 0, 0, 1, 62, 0, 100, 0, 58670, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Kor Bloodtusk - On gossip select - Close gossip'),
(58670, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 83, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kor Bloodtusk - On linked - Remove gossip flag'),
(58670, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 12, 58666, 3, 180000, 0, 0, 0, 8, 0, 0, 0, -1648.94, 1842.76, -2.51, 5.98, 'Kor Bloodtusk - On linked - Summon Sunwalker Dezco'),
(58670, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 12, 58975, 3, 180000, 0, 0, 0, 8, 0, 0, 0, -1626.92, 1854.67, -2.49, 4.73, 'Kor Bloodtusk - On linked - Summon Lorekeeper Vaeldrin'),
(58670, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 12, 58224, 3, 180000, 0, 0, 0, 8, 0, 0, 0, -1623.08, 1836.52, -2.56, 3.09, 'Kor Bloodtusk - On linked - Summon Groundbreaker Brojai'),
(58670, 0, 5, 0, 25, 0, 100, 0, 0, 0, 0, 0, 0, 82, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kor Bloodtusk - On reset - Show gossip flag'),
(58224, 0, 0, 0, 54, 0, 100, 0, 0, 0, 0, 0, 0, 80, 5822401, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Brojai - On summoned - Run intro'),
(58224, 0, 1, 0, 0, 0, 100, 0, 3500, 12000, 3500, 12000, 0, 11, 80117, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Brojai - In combat - Cast Earth Spike'),
(58224, 0, 2, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 9, 58666, 0, 50, 0, 0, 0, 0, 'Brojai - On death - Data to Dezco'),
(5822401, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 18, 131328, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Brojai - Pacified and immune to players'),
(5822401, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 69, 1, 0, 0, 0, 0, 0, 8, 0, 0, 0, -1628.08, 1836.57, -2.56, 0, 'Brojai - Walk 5 yards forward'),
(5822401, 9, 2, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Brojai - Say You fail to bend the knee'),
(5822401, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 19, 131328, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Brojai - Remove pacified and immune'),
(5822401, 9, 4, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 51, 0, 0, 0, 0, 0, 0, 9, 58670, 0, 40, 0, 0, 0, 0, 'Brojai - Kill Kor'),
(5822401, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 84, 0, 0, 0, 0, 0, 0, 9, 58666, 0, 50, 0, 0, 0, 0, 'Dezco - Say KOR NOOO'),
(58666, 0, 0, 0, 38, 0, 100, 0, 1, 1, 0, 0, 0, 80, 5866601, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dezco - On data 1 - Run aftermath'),
(5866601, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 84, 0, 0, 0, 0, 0, 0, 9, 58975, 0, 50, 0, 0, 0, 0, 'Vaeldrin - Say What now Sunwalker'),
(5866601, 9, 1, 0, 0, 0, 100, 0, 3500, 3500, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dezco - Say I think we have both lost enough'),
(5866601, 9, 2, 0, 0, 0, 100, 0, 5100, 5100, 0, 0, 0, 84, 1, 0, 0, 0, 0, 0, 9, 58975, 0, 50, 0, 0, 0, 0, 'Vaeldrin - Say Agreed'),
(5866601, 9, 3, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 0, 84, 2, 0, 0, 0, 0, 0, 9, 58975, 0, 50, 0, 0, 0, 0, 'Vaeldrin - Say Lyalia I am so sorry');
