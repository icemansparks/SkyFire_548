-- Quests 30348 "Immortality?" and 30445 "The Waters of Youth" (Krasarang
-- Wilds, level 86): both objectives are "observe" credits, 58894 and 58970.
-- They are given by alexkulya's script npc_lorekeeper_vaeldrin when the
-- player accepts the quest from Lorekeeper Vaeldrin (58745): at the Fountain
-- camp (area 6009) he eats and kneels in a ritual with Lyalia (58735)
-- answering, credit after 32 s; at the Lyalia camp (area 6016) he is replaced
-- by a ritual copy (58970) and a dying Lyalia copy (58833), the ritual runs
-- with four lines and a Transfer Life cast (111849), credit after 31.5 s,
-- then both copies despawn. The world DB had no Vaeldrin spawn at all, so
-- neither quest could be accepted. Here: the two spawns from alexkulya (guids
-- 517760 in area 6009, 518192 in area 6016) with SmartAI scripts per spawn
-- guid that run the sequences as timed action lists (5874501 and 5874502;
-- each row's delay counts from the previous row, so the totals match
-- alexkulya's timings); the ritual copies are summoned by the Lyalia camp
-- list, Transfer Life is cast by the ritual copy on a data trigger, and
-- Lyalia (58821) hides and shows herself on data triggers because SmartAI's
-- visibility action only changes its own creature. The third alexkulya spawn
-- (area 6058, guid 518736) is left out: it would offer both quests at a place
-- where nothing happens when they are accepted. Differences from alexkulya:
-- the Fountain camp ritual runs on the original Vaeldrin instead of a
-- hidden-and-replaced copy; the dying Lyalia copy does not get her
-- feign-death flag removed and the ritual copy does not die at the end (no
-- such SmartAI action, both just despawn); the quest giver flag is not
-- toggled during the sequences; a second accept while a sequence runs is
-- ignored (the accept event only runs in event phase 1; the accept moves the
-- NPC to phase 2 and the end of the sequence, or a reset, moves it back), so
-- the running sequence is never restarted and its player keeps the credit;
-- the second player abandons and accepts again after it. Before this update:
-- no creature with id 58745 or guids 517760 and 518192, 58745, 58821 and
-- 58970 AIName ''.
DELETE FROM `creature` WHERE `guid` IN (517760, 518192);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(517760, 58745, 870, 1, 0, 0, 0, 0, -1127.42, -208.356, 31.3687, 3.79179, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(518192, 58745, 870, 1, 0, 0, 0, 0, -1172.53, 1789.74, 18.128, 5.56365, 300, 0, 0, 0, 0, 0, 0, 0, 0);

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (58745, 58821, 58970);

DELETE FROM `smart_scripts` WHERE (`entryorguid` IN (-517760, -518192, 58970, 58821) AND `source_type` = 0) OR (`entryorguid` IN (5874501, 5874502) AND `source_type` = 9);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(-517760, 0, 0, 0, 25, 0, 100, 0, 0, 0, 0, 0, 0, 17, 483, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Vaeldrin (Fountain camp) - On reset - Read emote state'),
(-517760, 0, 1, 3, 19, 1, 100, 0, 30348, 0, 0, 0, 0, 80, 5874501, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Vaeldrin (Fountain camp) - On quest Immortality accepted in phase 1 - Run ritual'),
(-517760, 0, 2, 0, 25, 0, 100, 0, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Vaeldrin (Fountain camp) - On reset - Event phase 1'),
(-517760, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 22, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Vaeldrin (Fountain camp) - On linked - Event phase 2 while the ritual runs'),
(-518192, 0, 0, 2, 19, 1, 100, 0, 30445, 0, 0, 0, 0, 80, 5874502, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Vaeldrin (Lyalia camp) - On quest Waters of Youth accepted in phase 1 - Run ritual'),
(-518192, 0, 1, 0, 25, 0, 100, 0, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Vaeldrin (Lyalia camp) - On reset - Event phase 1'),
(-518192, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 22, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Vaeldrin (Lyalia camp) - On linked - Event phase 2 while the ritual runs'),
(5874501, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Vaeldrin - Stop reading'),
(5874501, 9, 1, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Vaeldrin - Say And now for the moment of truth'),
(5874501, 9, 2, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 5, 432, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Vaeldrin - Use emote'),
(5874501, 9, 3, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 5, 7, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Vaeldrin - Eat emote'),
(5874501, 9, 4, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 11, 112853, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Vaeldrin - Cast ritual spell'),
(5874501, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 17, 550, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Vaeldrin - Kneel'),
(5874501, 9, 6, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 84, 0, 0, 0, 0, 0, 0, 9, 58735, 0, 50, 0, 0, 0, 0, 'Lyalia - Say Father are you okay'),
(5874501, 9, 7, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Vaeldrin - Yell Aaaaaaaaahhh'),
(5874501, 9, 8, 0, 0, 0, 100, 0, 8000, 8000, 0, 0, 0, 33, 58894, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Vaeldrin - Credit Observe Lorekeeper Vaeldrin''s ritual to the player'),
(5874501, 9, 9, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Vaeldrin - Say That hurt'),
(5874501, 9, 10, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Vaeldrin - Stop kneeling'),
(5874501, 9, 11, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 17, 483, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Vaeldrin - Read emote state'),
(5874501, 9, 12, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Vaeldrin - Back to event phase 1'),
(5874502, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 47, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Vaeldrin - Hide'),
(5874502, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 45, 1, 2, 0, 0, 0, 0, 9, 58821, 0, 50, 0, 0, 0, 0, 'Lyalia - Hide (data 2)'),
(5874502, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 12, 58970, 3, 60000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Vaeldrin - Summon ritual Vaeldrin'),
(5874502, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 12, 58833, 3, 60000, 0, 0, 0, 9, 58821, 0, 50, 0, 0, 0, 0, 'Lyalia - Summon dying Lyalia at her spot'),
(5874502, 9, 4, 0, 0, 0, 100, 0, 500, 500, 0, 0, 0, 84, 0, 0, 0, 0, 0, 0, 9, 58970, 0, 50, 0, 0, 0, 0, 'Ritual Vaeldrin - Say I spent so long'),
(5874502, 9, 5, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 17, 550, 0, 0, 0, 0, 0, 9, 58970, 0, 50, 0, 0, 0, 0, 'Ritual Vaeldrin - Kneel'),
(5874502, 9, 6, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 84, 1, 0, 0, 0, 0, 0, 9, 58970, 0, 50, 0, 0, 0, 0, 'Ritual Vaeldrin - Say And now this'),
(5874502, 9, 7, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 84, 2, 0, 0, 0, 0, 0, 9, 58970, 0, 50, 0, 0, 0, 0, 'Ritual Vaeldrin - Say The water may not create life'),
(5874502, 9, 8, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 84, 3, 0, 0, 0, 0, 0, 9, 58970, 0, 50, 0, 0, 0, 0, 'Ritual Vaeldrin - Say I love you'),
(5874502, 9, 9, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 17, 0, 0, 0, 0, 0, 0, 9, 58970, 0, 50, 0, 0, 0, 0, 'Ritual Vaeldrin - Stop kneeling'),
(5874502, 9, 10, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 5, 7, 0, 0, 0, 0, 0, 9, 58970, 0, 50, 0, 0, 0, 0, 'Ritual Vaeldrin - Eat emote'),
(5874502, 9, 11, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 9, 58970, 0, 50, 0, 0, 0, 0, 'Ritual Vaeldrin - Start Transfer Life'),
(5874502, 9, 12, 0, 0, 0, 100, 0, 9000, 9000, 0, 0, 0, 84, 0, 0, 0, 0, 0, 0, 9, 58833, 0, 50, 0, 0, 0, 0, 'Dying Lyalia - Say F father No'),
(5874502, 9, 13, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 33, 58970, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Vaeldrin - Credit Observe Vaeldrin''s ritual to the player'),
(5874502, 9, 14, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 9, 58970, 0, 50, 0, 0, 0, 0, 'Ritual Vaeldrin - Despawn'),
(5874502, 9, 15, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 9, 58833, 0, 50, 0, 0, 0, 0, 'Dying Lyalia - Despawn'),
(5874502, 9, 16, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 47, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Vaeldrin - Show'),
(5874502, 9, 17, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 45, 1, 3, 0, 0, 0, 0, 9, 58821, 0, 50, 0, 0, 0, 0, 'Lyalia - Show (data 3)'),
(5874502, 9, 18, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Vaeldrin - Back to event phase 1'),
(58970, 0, 0, 0, 38, 0, 100, 0, 1, 1, 0, 0, 0, 11, 111849, 2, 0, 0, 0, 0, 9, 58833, 0, 50, 0, 0, 0, 0, 'Ritual Vaeldrin - On data 1 - Cast Transfer Life on dying Lyalia'),
(58821, 0, 0, 0, 38, 0, 100, 0, 1, 2, 0, 0, 0, 47, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lyalia (Lyalia camp) - On data 2 - Hide'),
(58821, 0, 1, 0, 38, 0, 100, 0, 1, 3, 0, 0, 0, 47, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lyalia (Lyalia camp) - On data 3 - Show');
