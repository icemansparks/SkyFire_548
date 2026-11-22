-- Quest 29586 "The Splintered Path" (The Jade Forest, level 85): the
-- objective is kill credit "Mogu Camp Discovered" (54999). alexkulya's script
-- AreaTrigger_q29586 gives it when a player on the quest enters area trigger
-- 6976 with Shao the Defiant within 150 yards (Shao then speaks), and
-- ambushes the player with two Gormali Stalkers (55084) that jump in from the
-- trees. The world DB had no areatrigger_scripts row for 6976, so the credit
-- never came and the quest could not be completed. Here: trigger 6976 uses
-- SmartTrigger with a credit action for the player who entered, limited by a
-- condition (type 22 smart event, quest state incomplete) to players on the
-- quest. Differences from alexkulya: no Shao line, no Gormali Stalker ambush,
-- and no check that Shao is nearby. Before this update: no
-- areatrigger_scripts, smart_scripts (source type 2) or conditions rows for
-- 6976.
DELETE FROM `areatrigger_scripts` WHERE `entry` = 6976;
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(6976, 'SmartTrigger');

DELETE FROM `smart_scripts` WHERE `entryorguid` = 6976 AND `source_type` = 2;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(6976, 2, 0, 0, 46, 0, 100, 0, 6976, 0, 0, 0, 0, 33, 54999, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Mogu camp trigger - On trigger - Credit Mogu Camp Discovered');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceId` = 2 AND `SourceEntry` = 6976;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(22, 1, 6976, 2, 0, 47, 0, 29586, 8, 0, 0, 0, 0, '', 'Mogu camp trigger - only with The Splintered Path incomplete');
