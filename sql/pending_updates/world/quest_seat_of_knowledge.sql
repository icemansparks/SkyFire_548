-- Veiled Stair, "A Pandaren Legend" (33104): one of its two kill objectives is the credit
-- "Wrathion Meet Seat of Knowledge" (72723). alexkulya's script AreaTrigger_at_seat_of_knowledge
-- gives it when a player on the quest enters area trigger 9321. There was no
-- areatrigger_scripts row for 9321, so the credit never came.
-- Here: trigger 9321 uses SmartTrigger with a credit action for the player who entered,
-- limited by a condition (type 22 smart event, quest state incomplete) to players on the quest.
-- Not covered: the other objective of 33104 (credit 61962, from the legendary finale scene).
-- Before this update there were no areatrigger_scripts, smart_scripts (source type 2) or
-- conditions rows for 9321.
DELETE FROM `areatrigger_scripts` WHERE `entry` = 9321;
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(9321, 'SmartTrigger');

DELETE FROM `smart_scripts` WHERE `entryorguid` = 9321 AND `source_type` = 2;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(9321, 2, 0, 0, 46, 0, 100, 0, 9321, 0, 0, 0, 0, 33, 72723, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Seat of Knowledge trigger - On trigger - Credit Wrathion Meet Seat of Knowledge');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceId` = 2 AND `SourceEntry` = 9321;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(22, 1, 9321, 2, 0, 47, 0, 33104, 8, 0, 0, 0, 0, '', 'Seat of Knowledge trigger - only with A Pandaren Legend incomplete');
