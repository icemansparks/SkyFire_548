-- Quest 30480 "The Ritual" (Kun-Lai Summit, level 87): the objectives are "Speak
-- to Waterspeaker Gorai to Begin the Ritual" (60973), "Jinyu corpses cleansed"
-- (kill credit 61654) and kill Explosive Hatred (61530). On alexkulya's 5.4.8 core
-- the script npc_waterspeaker_gorai offers the gossip option "I'm ready. Begin the
-- ritual." while the quest is taken; picking it gives the 60973 credit, then a
-- timed sequence: staff dummy cast (119729), the 61654 credit, Swordmistress Mei
-- (59273) casts the sha pool effect (119542), Gorai summons Explosive Hatred at
-- 1921.28 167.02 477.382 and it attacks the player (here it attacks the nearest
-- player on summon), and when it dies Mei casts the pool cleanup visual (120138).
-- There was no gossip and no AI on Gorai, and Explosive Hatred was friendly
-- (faction 35), so the quest could not complete.
-- Timed action list delays are relative to the previous step (22, 7, 4, 5 s).
-- Here: that gossip option (menu 51201, condition: quest taken), the same
-- sequence in SmartAI (timed action lists 6097300 and 6097301, Mei's two casts on
-- Mei's own AI), and faction 14 on Explosive Hatred. Gorai ignores the option
-- while a ritual runs (event phase 2) and goes back to phase 1 when the summon
-- dies or despawns. Gorai's, Mei's and Explosive Hatred's creature_text groups
-- are empty in alexkulya's DB as well, so no text lines are added.
-- Before this update there were no gossip_menu, gossip_menu_option, conditions
-- (type 15) or smart_scripts rows for these ids; Gorai had gossip_menu_id 0, npcflag 2,
-- AIName ''; Mei had npcflag 2, AIName ''; Explosive Hatred had faction 35/35,
-- AIName ''; no creature rows existed for 61530.
DELETE FROM `gossip_menu` WHERE `MenuID` = 51201;
INSERT INTO `gossip_menu` (`MenuID`, `TextID`) VALUES
(51201, 68);

DELETE FROM `gossip_menu_option` WHERE `MenuID` = 51201;
INSERT INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcflag`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextID`) VALUES
(51201, 0, 0, 'I''m ready. Begin the ritual.', 0, 1, 1, 0, 0, 0, 0, '', 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 51201;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 51201, 0, 0, 0, 9, 0, 30480, 0, 0, 0, 0, 0, '', 'Waterspeaker Gorai - Show gossip option if quest The Ritual is taken');

UPDATE `creature_template` SET `npcflag` = `npcflag` | 1, `gossip_menu_id` = 51201, `AIName` = 'SmartAI' WHERE `entry` = 60973;
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (59273, 61530);
UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14 WHERE `entry` = 61530;

DELETE FROM `smart_scripts` WHERE (`entryorguid` IN (60973, 59273, 61530) AND `source_type` = 0) OR (`entryorguid` IN (6097300, 6097301) AND `source_type` = 9);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(60973, 0, 0, 0, 25, 0, 100, 0, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Waterspeaker Gorai - On reset - Set event phase 1'),
(60973, 0, 1, 2, 62, 1, 100, 0, 51201, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Waterspeaker Gorai - On gossip select - Close gossip'),
(60973, 0, 2, 3, 61, 1, 100, 0, 0, 0, 0, 0, 0, 33, 60973, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Waterspeaker Gorai - On linked - Player gets quest credit'),
(60973, 0, 3, 4, 61, 1, 100, 0, 0, 0, 0, 0, 0, 22, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Waterspeaker Gorai - On linked - Set event phase 2'),
(60973, 0, 4, 0, 61, 2, 100, 0, 0, 0, 0, 0, 0, 80, 6097300, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Waterspeaker Gorai - On linked - Run ritual action list'),
(60973, 0, 5, 0, 38, 0, 100, 0, 1, 1, 0, 0, 0, 80, 6097301, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Waterspeaker Gorai - On data 1 1 (Explosive Hatred died) - Run finish action list'),
(60973, 0, 6, 0, 35, 0, 100, 0, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Waterspeaker Gorai - On summon despawned - Set event phase 1'),
(6097300, 9, 0, 0, 0, 0, 100, 0, 22000, 22000, 0, 0, 0, 11, 119729, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Waterspeaker Gorai ritual - After 22 s - Cast staff dummy on self'),
(6097300, 9, 1, 0, 0, 0, 100, 0, 7000, 7000, 0, 0, 0, 33, 61654, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Waterspeaker Gorai ritual - 7 s later - Player gets Jinyu corpses cleansed credit'),
(6097300, 9, 2, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 0, 45, 1, 0, 0, 0, 0, 0, 19, 59273, 30, 0, 0, 0, 0, 0, 'Waterspeaker Gorai ritual - 4 s later - Swordmistress Mei data 1 0'),
(6097300, 9, 3, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 12, 61530, 7, 60000, 0, 0, 0, 8, 0, 0, 0, 1921.28, 167.02, 477.382, 4.82, 'Waterspeaker Gorai ritual - 5 s later - Summon Explosive Hatred'),
(6097301, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 19, 59273, 30, 0, 0, 0, 0, 0, 'Waterspeaker Gorai finish - Swordmistress Mei data 1 1'),
(6097301, 9, 1, 0, 0, 0, 100, 0, 11000, 11000, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Waterspeaker Gorai finish - After 11 s - Set event phase 1'),
(59273, 0, 0, 0, 38, 0, 100, 0, 1, 0, 0, 0, 0, 11, 119542, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Swordmistress Mei - On data 1 0 - Cast sha pool effect on self'),
(59273, 0, 1, 0, 38, 0, 100, 0, 1, 1, 0, 0, 0, 11, 120138, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Swordmistress Mei - On data 1 1 - Cast sha pool cleanup visual on self'),
(61530, 0, 0, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 19, 60973, 100, 0, 0, 0, 0, 0, 'Explosive Hatred - On death - Waterspeaker Gorai data 1 1'),
(61530, 0, 1, 0, 54, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 21, 80, 0, 0, 0, 0, 0, 0, 'Explosive Hatred - On just summoned - Attack the nearest player'),
(61530, 0, 2, 0, 7, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Explosive Hatred - On evade - Despawn');
