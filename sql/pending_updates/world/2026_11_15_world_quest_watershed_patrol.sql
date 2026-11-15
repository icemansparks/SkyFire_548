-- Quest 25188 "Watershed Patrol": none of the four "help" objectives could be
-- completed. Each one is a kill credit (Durotar Watershed Quest KC 1-4) that
-- the matching outpost quest gives on turn-in, and no script gave it.
-- Smart scripts from the alexkulya 5.4.8 world DB: the NPC that takes the
-- outpost quest credits the player on reward.
--   KC 1 (39331) Help Grandmatron Tekla: Spirits Be Praised (25189), ends at Raggaran
--   KC 2 (39332) Help Raggaran:          Raggaran's Fury (25192)
--   KC 3 (39333) Help Misha Tor'kren:    Lost But Not Forgotten (25193)
--   KC 4 (39334) Help Zen'Taji:          That's the End of That Raptor (25195)

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (3193, 39324, 39326);

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (3193, 39324, 39326) AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(3193, 0, 0, 0, 20, 0, 100, 0, 25193, 0, 0, 0, 33, 39333, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Misha Tor''kren - On Quest Reward - Kill Credit 25188'),
(39324, 0, 0, 0, 20, 0, 100, 0, 25195, 0, 0, 0, 33, 39334, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Zen''Taji - On Quest Reward - Kill Credit 25188'),
(39326, 0, 0, 0, 20, 0, 100, 0, 25192, 0, 0, 0, 33, 39332, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Raggaran - On Quest Reward - Kill Credit 25188'),
(39326, 0, 1, 0, 20, 0, 100, 0, 25189, 0, 0, 0, 33, 39331, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Raggaran - On Quest Reward - Kill Credit 25188');
