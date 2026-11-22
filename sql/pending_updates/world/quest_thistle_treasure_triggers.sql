-- Quest 31314 "Old Man Thistle's Treasure" (Valley of the Four Winds): its
-- two objectives are kill credits "Find the cave entrance" (64328) and "Find
-- Old Man Thistle's Treasure" (64329). alexkulya's script
-- AreaTrigger_cavern_of_endless_echoes gives them from area triggers 8262
-- (cave entrance) and 8263 (treasure) while the quest is incomplete, plus a
-- line from the Fish Fellreed pet (64347, not spawned). The world DB had no
-- areatrigger_scripts row for either trigger, so walking in gave nothing and
-- the quest could not be completed. Here: both triggers use SmartTrigger with
-- a credit action for the player who entered, limited by a condition (type 22
-- smart event, quest state incomplete) to players on the quest. Differences
-- from alexkulya: the Fish Fellreed pet lines are left out. Before this
-- update: no areatrigger_scripts, smart_scripts (source type 2) or conditions
-- rows for 8262 and 8263.
DELETE FROM `areatrigger_scripts` WHERE `entry` IN (8262, 8263);
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(8262, 'SmartTrigger'),
(8263, 'SmartTrigger');

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (8262, 8263) AND `source_type` = 2;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(8262, 2, 0, 0, 46, 0, 100, 0, 8262, 0, 0, 0, 0, 33, 64328, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Cavern of Endless Echoes entrance - On trigger - Credit Find the cave entrance'),
(8263, 2, 0, 0, 46, 0, 100, 0, 8263, 0, 0, 0, 0, 33, 64329, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Cavern of Endless Echoes treasure - On trigger - Credit Find the treasure');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceId` = 2 AND `SourceEntry` IN (8262, 8263);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(22, 1, 8262, 2, 0, 47, 0, 31314, 8, 0, 0, 0, 0, '', 'Cavern of Endless Echoes entrance - only with Old Man Thistle''s Treasure incomplete'),
(22, 1, 8263, 2, 0, 47, 0, 31314, 8, 0, 0, 0, 0, '', 'Cavern of Endless Echoes treasure - only with Old Man Thistle''s Treasure incomplete');
