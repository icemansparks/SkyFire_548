-- Varzok (32009) starts when a Horde battle pet trainer's Got one! is handed in
-- Varzok is the Horde breadcrumb to the battle pet tamer chain: Report to Varzok in
-- Orgrimmar, then Zunta, The Pet Tamer (31812). It carries QUEST_FLAGS_AUTO_ACCEPT and
-- reaches the player without a quest giver once the trainer tutorial ends. Neither the
-- MoP 5.4.8 world DB (alexkulya) nor its core holds the trigger, so the six Horde
-- trainers add it through SmartAI when their own Got one! is rewarded:
-- Narzak 31570, Naleen 31575, Ansel Fincap 31578, Jarson Everlong 31581, Matty 31587,
-- Varzok 31590 (trainers and quests as in the pet trainer chain update).
-- Player::AddQuest completes the talk-to quest at once, so Varzok can take it in.
-- Condition: only when 32009 was never taken (a capital trainer's chain can be the
-- second one a character finishes).

SET @QUEST := 32009;

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (63061, 63067, 63073, 63080, 63086, 63626);

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (63061, 63067, 63073, 63080, 63086, 63626) AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(63061, 0, 0, 0, 20, 0, 100, 0, 31570, 0, 0, 0, 7, @QUEST, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Narzak - On Quest ''Got one!'' Rewarded - Add Quest ''Varzok'''),
(63067, 0, 0, 0, 20, 0, 100, 0, 31575, 0, 0, 0, 7, @QUEST, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Naleen - On Quest ''Got one!'' Rewarded - Add Quest ''Varzok'''),
(63073, 0, 0, 0, 20, 0, 100, 0, 31578, 0, 0, 0, 7, @QUEST, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Ansel Fincap - On Quest ''Got one!'' Rewarded - Add Quest ''Varzok'''),
(63080, 0, 0, 0, 20, 0, 100, 0, 31581, 0, 0, 0, 7, @QUEST, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Jarson Everlong - On Quest ''Got one!'' Rewarded - Add Quest ''Varzok'''),
(63086, 0, 0, 0, 20, 0, 100, 0, 31587, 0, 0, 0, 7, @QUEST, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Matty - On Quest ''Got one!'' Rewarded - Add Quest ''Varzok'''),
(63626, 0, 0, 0, 20, 0, 100, 0, 31590, 0, 0, 0, 7, @QUEST, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Varzok - On Quest ''Got one!'' Rewarded - Add Quest ''Varzok''');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceGroup` = 1 AND `SourceEntry` IN (63061, 63067, 63073, 63080, 63086, 63626) AND `SourceId` = 0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(22, 1, 63061, 0, 0, 14, 0, @QUEST, 0, 0, 0, 0, 0, '', 'Narzak - Add Varzok only if never taken'),
(22, 1, 63067, 0, 0, 14, 0, @QUEST, 0, 0, 0, 0, 0, '', 'Naleen - Add Varzok only if never taken'),
(22, 1, 63073, 0, 0, 14, 0, @QUEST, 0, 0, 0, 0, 0, '', 'Ansel Fincap - Add Varzok only if never taken'),
(22, 1, 63080, 0, 0, 14, 0, @QUEST, 0, 0, 0, 0, 0, '', 'Jarson Everlong - Add Varzok only if never taken'),
(22, 1, 63086, 0, 0, 14, 0, @QUEST, 0, 0, 0, 0, 0, '', 'Matty - Add Varzok only if never taken'),
(22, 1, 63626, 0, 0, 14, 0, @QUEST, 0, 0, 0, 0, 0, '', 'Varzok - Add Varzok only if never taken');
