-- Quest 30142 "It's A..." (Jade Forest, level 90, Instructor Skythorn 58228):
-- the only objective is "Egg hatched", a kill credit for 58315. alexkulya's
-- npc_instructor_skythorn gives it 34 seconds after the player accepts the
-- quest (10 s, then three steps of 8 s; each step calls Talk(1..3) and
-- Talk(0) at the start, but neither alexkulya's nor the world DB's
-- creature_text has any row for 58228, so those talks say nothing). Without
-- the script the credit never arrived and the quest could not complete. Here:
-- on accepting 30142 the NPC runs a one-row timed action list that gives the
-- accepting player the credit after 34 seconds. Same limit as alexkulya's
-- script, which keeps one player and one timer: a second player accepting
-- within those 34 seconds restarts the timer and takes over the credit, so
-- the first player has to abandon and accept again. Before this update: no
-- smart_scripts for 58228 or 5822800, AIName '' and ScriptName '' on 58228,
-- no creature_text for 58228.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 58228;

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 58228;
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` = 5822800;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(58228, 0, 0, 0, 19, 0, 100, 0, 30142, 0, 0, 0, 0, 80, 5822800, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Instructor Skythorn - On quest It''s A... accepted - Run the egg timer'),
(5822800, 9, 0, 0, 0, 0, 100, 0, 34000, 34000, 0, 0, 0, 33, 58315, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Instructor Skythorn - Egg timer - Player gets quest credit Egg hatched');
