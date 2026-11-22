-- Quest 29883 "The Pearlfin Situation" (Jade Forest, level 85): the four
-- objectives are "Speak to" Pearlkeeper Fujin (59058), Watersmith Ot-Temmdo
-- (56693), Instructor Sharpfin (56690) and Elder Lusshan (54960). On
-- alexkulya's 5.4.8 core the script npc_the_pearlfin_situation_q gives each
-- of them a gossip option while the quest is taken; picking it gives the
-- NPC's kill credit and, for Lusshan, says his text 0. The world DB had no
-- option, no AI and no Lusshan spawn, so the quest could not complete. Here:
-- one gossip menu per NPC (option text from the C++ script) shown while quest
-- 29883 is taken, SmartAI that closes the menu, says text 0 where the NPC has
-- one (only Lusshan; the other three have no creature_text rows) and gives
-- the credit, the gossip flag on the templates, and Lusshan's spawn
-- (alexkulya guid 503397, default phase; the same phase as Fujin). Menu ids
-- 51101 to 51104 and gossip_menu rows are new; TEXT 68 is "Greetings $N".
-- Before this update: no gossip_menu, gossip_menu_option, conditions (type
-- 15) or smart_scripts rows for these ids, no creature with id 54960 or guid
-- 503397; template npcflag Lusshan 2, Sharpfin 0, Ot-Temmdo 4224, Fujin 2,
-- gossip_menu_id 0 and AIName '' on all four.
DELETE FROM `gossip_menu` WHERE `MenuID` BETWEEN 51101 AND 51104;
INSERT INTO `gossip_menu` (`MenuID`, `TextID`) VALUES
(51101, 68),
(51102, 68),
(51103, 68),
(51104, 68);

DELETE FROM `gossip_menu_option` WHERE `MenuID` BETWEEN 51101 AND 51104;
INSERT INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcflag`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextID`) VALUES
(51101, 0, 0, 'I come from the Alliance. We wish to be allies, not enemies.', 0, 1, 1, 0, 0, 0, 0, '', 0),
(51102, 0, 0, 'My friends and I come with peaceful intentions.', 0, 1, 1, 0, 0, 0, 0, '', 0),
(51103, 0, 0, 'What are you doing?', 0, 1, 1, 0, 0, 0, 0, '', 0),
(51104, 0, 0, 'Please allow us a chance to prove our friendship. We wish you no harm', 0, 1, 1, 0, 0, 0, 0, '', 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` BETWEEN 51101 AND 51104;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 51101, 0, 0, 0, 9, 0, 29883, 0, 0, 0, 0, 0, '', 'Elder Lusshan - Show gossip option if quest The Pearlfin Situation is taken'),
(15, 51102, 0, 0, 0, 9, 0, 29883, 0, 0, 0, 0, 0, '', 'Instructor Sharpfin - Show gossip option if quest The Pearlfin Situation is taken'),
(15, 51103, 0, 0, 0, 9, 0, 29883, 0, 0, 0, 0, 0, '', 'Watersmith Ot-Temmdo - Show gossip option if quest The Pearlfin Situation is taken'),
(15, 51104, 0, 0, 0, 9, 0, 29883, 0, 0, 0, 0, 0, '', 'Pearlkeeper Fujin - Show gossip option if quest The Pearlfin Situation is taken');

UPDATE `creature_template` SET `npcflag` = `npcflag` | 1, `gossip_menu_id` = 51101, `AIName` = 'SmartAI' WHERE `entry` = 54960;
UPDATE `creature_template` SET `npcflag` = `npcflag` | 1, `gossip_menu_id` = 51102, `AIName` = 'SmartAI' WHERE `entry` = 56690;
UPDATE `creature_template` SET `npcflag` = `npcflag` | 1, `gossip_menu_id` = 51103, `AIName` = 'SmartAI' WHERE `entry` = 56693;
UPDATE `creature_template` SET `npcflag` = `npcflag` | 1, `gossip_menu_id` = 51104, `AIName` = 'SmartAI' WHERE `entry` = 59058;

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (54960, 56690, 56693, 59058) AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(54960, 0, 0, 1, 62, 0, 100, 0, 51101, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Elder Lusshan - On gossip select - Close gossip'),
(54960, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Elder Lusshan - On linked - Say text 0 to player'),
(54960, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 54960, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Elder Lusshan - On linked - Player gets quest credit'),
(56690, 0, 0, 1, 62, 0, 100, 0, 51102, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Instructor Sharpfin - On gossip select - Close gossip'),
(56690, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 56690, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Instructor Sharpfin - On linked - Player gets quest credit'),
(56693, 0, 0, 1, 62, 0, 100, 0, 51103, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Watersmith Ot-Temmdo - On gossip select - Close gossip'),
(56693, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 56693, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Watersmith Ot-Temmdo - On linked - Player gets quest credit'),
(59058, 0, 0, 1, 62, 0, 100, 0, 51104, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Pearlkeeper Fujin - On gossip select - Close gossip'),
(59058, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 59058, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Pearlkeeper Fujin - On linked - Player gets quest credit');

DELETE FROM `creature` WHERE `guid` = 503397;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(503397, 54960, 870, 1, 0, 0, 0, 0, -219.344, -2704.9, 5.28804, 5.36064, 60, 0, 0, 0, 0, 0, 0, 0, 0);
