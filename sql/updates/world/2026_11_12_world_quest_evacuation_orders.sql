-- Quest 29982 "Evacuation Orders" (Valley of the Four Winds, level 86): the four
-- objectives are "Evacuate" the Blacksoil, Spadepaw, Greentill and Marlfur
-- families, credited to Wei Blacksoil (57120), Feng Spadepaw (57121), Haiyun
-- Greentill (57123) and Zhang Marlfur (57126). On alexkulya's 5.4.8 core the
-- script npc_q29982 gives each family member a gossip option while the quest is
-- taken. Feng, Haiyun and Zhang credit on the first pick (Shu-Li, Ying and Mia
-- give their sibling's credit); Wei first says his lines and a mantid ambusher
-- (57134) attacks the player, and only after it dies does a second option give
-- Wei's credit. TEST had no menus, no AI and no gossip flags, and the ambusher
-- was friendly (faction 35), so the quest could not complete.
-- Here: alexkulya's menus 13310, 13311 and 13316 to 13320 with their npc_text
-- and option text, shown only while 29982 is taken; the gossip flag, menu and
-- SmartAI on the seven templates; Wei summons the ambusher 3 seconds after the
-- pick (text 0, then text 1 and the summon at alexkulya's position); the
-- ambusher attacks the nearest player, casts Swift Ambush once at 2 s and Slit
-- Throat at 7 s then every 10 to 12 s, and on death gives Wei's credit to the
-- loot recipient and tells Wei (within 100 yards of where it died), who says
-- text 2.
-- Differences from alexkulya, kept simple on purpose: Wei's credit comes when
-- the ambusher dies, not from a second gossip pick; the family cutscenes (the
-- summoned look-alikes that walk away, 107515 to 107546) are left out, as are
-- the texts alexkulya's script gives them (no creature_text exists for them);
-- the option stays visible after its objective is done; a second pick on Wei
-- while the ambusher lives summons a second one (25 s despawn out of combat).
-- The creature_text rows these lines need were already on TEST (queried
-- 2026-11-12: Wei 57120 groups 0 to 2, Feng 57121 group 0, Shu-Li 57122 groups 0
-- and 1); none are added here.
-- Values before this update, queried on TEST mopworld on 2026-11-12: no
-- gossip_menu, gossip_menu_option, conditions (type 15) or smart_scripts rows
-- for these ids, no npc_text for the seven text ids, npcflag 0, gossip_menu_id 0
-- and AIName '' on the seven NPCs, and Ik'thik Ambusher (57134) with
-- faction_A and faction_H 35, AIName ''.
DELETE FROM `npc_text` WHERE `ID` IN (18832, 18844, 18845, 18834, 18846, 18843, 18847);
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`, `lang0`, `prob0`, `em0_0`, `em0_1`, `em0_2`, `em0_3`, `em0_4`, `em0_5`, `text1_0`, `text1_1`, `lang1`, `prob1`, `em1_0`, `em1_1`, `em1_2`, `em1_3`, `em1_4`, `em1_5`, `text2_0`, `text2_1`, `lang2`, `prob2`, `em2_0`, `em2_1`, `em2_2`, `em2_3`, `em2_4`, `em2_5`, `text3_0`, `text3_1`, `lang3`, `prob3`, `em3_0`, `em3_1`, `em3_2`, `em3_3`, `em3_4`, `em3_5`, `text4_0`, `text4_1`, `lang4`, `prob4`, `em4_0`, `em4_1`, `em4_2`, `em4_3`, `em4_4`, `em4_5`, `text5_0`, `text5_1`, `lang5`, `prob5`, `em5_0`, `em5_1`, `em5_2`, `em5_3`, `em5_4`, `em5_5`, `text6_0`, `text6_1`, `lang6`, `prob6`, `em6_0`, `em6_1`, `em6_2`, `em6_3`, `em6_4`, `em6_5`, `text7_0`, `text7_1`, `lang7`, `prob7`, `em7_0`, `em7_1`, `em7_2`, `em7_3`, `em7_4`, `em7_5`, `WDBVerified`) VALUES (18832,'Yes?','',0,100,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,18019);
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`, `lang0`, `prob0`, `em0_0`, `em0_1`, `em0_2`, `em0_3`, `em0_4`, `em0_5`, `text1_0`, `text1_1`, `lang1`, `prob1`, `em1_0`, `em1_1`, `em1_2`, `em1_3`, `em1_4`, `em1_5`, `text2_0`, `text2_1`, `lang2`, `prob2`, `em2_0`, `em2_1`, `em2_2`, `em2_3`, `em2_4`, `em2_5`, `text3_0`, `text3_1`, `lang3`, `prob3`, `em3_0`, `em3_1`, `em3_2`, `em3_3`, `em3_4`, `em3_5`, `text4_0`, `text4_1`, `lang4`, `prob4`, `em4_0`, `em4_1`, `em4_2`, `em4_3`, `em4_4`, `em4_5`, `text5_0`, `text5_1`, `lang5`, `prob5`, `em5_0`, `em5_1`, `em5_2`, `em5_3`, `em5_4`, `em5_5`, `text6_0`, `text6_1`, `lang6`, `prob6`, `em6_0`, `em6_1`, `em6_2`, `em6_3`, `em6_4`, `em6_5`, `text7_0`, `text7_1`, `lang7`, `prob7`, `em7_0`, `em7_1`, `em7_2`, `em7_3`, `em7_4`, `em7_5`, `WDBVerified`) VALUES (18834,'Stay back!$b$bWait... you\'re not one of them. We\'re not accustomed to outsiders, and with the mantid breaking through the wall, I...$b$bAnyhow, what can we do to help you?','',0,100,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,18019);
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`, `lang0`, `prob0`, `em0_0`, `em0_1`, `em0_2`, `em0_3`, `em0_4`, `em0_5`, `text1_0`, `text1_1`, `lang1`, `prob1`, `em1_0`, `em1_1`, `em1_2`, `em1_3`, `em1_4`, `em1_5`, `text2_0`, `text2_1`, `lang2`, `prob2`, `em2_0`, `em2_1`, `em2_2`, `em2_3`, `em2_4`, `em2_5`, `text3_0`, `text3_1`, `lang3`, `prob3`, `em3_0`, `em3_1`, `em3_2`, `em3_3`, `em3_4`, `em3_5`, `text4_0`, `text4_1`, `lang4`, `prob4`, `em4_0`, `em4_1`, `em4_2`, `em4_3`, `em4_4`, `em4_5`, `text5_0`, `text5_1`, `lang5`, `prob5`, `em5_0`, `em5_1`, `em5_2`, `em5_3`, `em5_4`, `em5_5`, `text6_0`, `text6_1`, `lang6`, `prob6`, `em6_0`, `em6_1`, `em6_2`, `em6_3`, `em6_4`, `em6_5`, `text7_0`, `text7_1`, `lang7`, `prob7`, `em7_0`, `em7_1`, `em7_2`, `em7_3`, `em7_4`, `em7_5`, `WDBVerified`) VALUES (18843,'How can I help you, $c?','',0,100,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,18019);
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`, `lang0`, `prob0`, `em0_0`, `em0_1`, `em0_2`, `em0_3`, `em0_4`, `em0_5`, `text1_0`, `text1_1`, `lang1`, `prob1`, `em1_0`, `em1_1`, `em1_2`, `em1_3`, `em1_4`, `em1_5`, `text2_0`, `text2_1`, `lang2`, `prob2`, `em2_0`, `em2_1`, `em2_2`, `em2_3`, `em2_4`, `em2_5`, `text3_0`, `text3_1`, `lang3`, `prob3`, `em3_0`, `em3_1`, `em3_2`, `em3_3`, `em3_4`, `em3_5`, `text4_0`, `text4_1`, `lang4`, `prob4`, `em4_0`, `em4_1`, `em4_2`, `em4_3`, `em4_4`, `em4_5`, `text5_0`, `text5_1`, `lang5`, `prob5`, `em5_0`, `em5_1`, `em5_2`, `em5_3`, `em5_4`, `em5_5`, `text6_0`, `text6_1`, `lang6`, `prob6`, `em6_0`, `em6_1`, `em6_2`, `em6_3`, `em6_4`, `em6_5`, `text7_0`, `text7_1`, `lang7`, `prob7`, `em7_0`, `em7_1`, `em7_2`, `em7_3`, `em7_4`, `em7_5`, `WDBVerified`) VALUES (18844,'What, another one? Did Commander Mai send you?$b$bHow many times do I have to tell that bonehead - we\'re not evacuating!','',0,100,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,18019);
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`, `lang0`, `prob0`, `em0_0`, `em0_1`, `em0_2`, `em0_3`, `em0_4`, `em0_5`, `text1_0`, `text1_1`, `lang1`, `prob1`, `em1_0`, `em1_1`, `em1_2`, `em1_3`, `em1_4`, `em1_5`, `text2_0`, `text2_1`, `lang2`, `prob2`, `em2_0`, `em2_1`, `em2_2`, `em2_3`, `em2_4`, `em2_5`, `text3_0`, `text3_1`, `lang3`, `prob3`, `em3_0`, `em3_1`, `em3_2`, `em3_3`, `em3_4`, `em3_5`, `text4_0`, `text4_1`, `lang4`, `prob4`, `em4_0`, `em4_1`, `em4_2`, `em4_3`, `em4_4`, `em4_5`, `text5_0`, `text5_1`, `lang5`, `prob5`, `em5_0`, `em5_1`, `em5_2`, `em5_3`, `em5_4`, `em5_5`, `text6_0`, `text6_1`, `lang6`, `prob6`, `em6_0`, `em6_1`, `em6_2`, `em6_3`, `em6_4`, `em6_5`, `text7_0`, `text7_1`, `lang7`, `prob7`, `em7_0`, `em7_1`, `em7_2`, `em7_3`, `em7_4`, `em7_5`, `WDBVerified`) VALUES (18845,'','Those had better not be evacuation papers I see.',0,100,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,18019);
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`, `lang0`, `prob0`, `em0_0`, `em0_1`, `em0_2`, `em0_3`, `em0_4`, `em0_5`, `text1_0`, `text1_1`, `lang1`, `prob1`, `em1_0`, `em1_1`, `em1_2`, `em1_3`, `em1_4`, `em1_5`, `text2_0`, `text2_1`, `lang2`, `prob2`, `em2_0`, `em2_1`, `em2_2`, `em2_3`, `em2_4`, `em2_5`, `text3_0`, `text3_1`, `lang3`, `prob3`, `em3_0`, `em3_1`, `em3_2`, `em3_3`, `em3_4`, `em3_5`, `text4_0`, `text4_1`, `lang4`, `prob4`, `em4_0`, `em4_1`, `em4_2`, `em4_3`, `em4_4`, `em4_5`, `text5_0`, `text5_1`, `lang5`, `prob5`, `em5_0`, `em5_1`, `em5_2`, `em5_3`, `em5_4`, `em5_5`, `text6_0`, `text6_1`, `lang6`, `prob6`, `em6_0`, `em6_1`, `em6_2`, `em6_3`, `em6_4`, `em6_5`, `text7_0`, `text7_1`, `lang7`, `prob7`, `em7_0`, `em7_1`, `em7_2`, `em7_3`, `em7_4`, `em7_5`, `WDBVerified`) VALUES (18846,'','Welcome to Stoneplow. What can we do for you?',0,100,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,18019);
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`, `lang0`, `prob0`, `em0_0`, `em0_1`, `em0_2`, `em0_3`, `em0_4`, `em0_5`, `text1_0`, `text1_1`, `lang1`, `prob1`, `em1_0`, `em1_1`, `em1_2`, `em1_3`, `em1_4`, `em1_5`, `text2_0`, `text2_1`, `lang2`, `prob2`, `em2_0`, `em2_1`, `em2_2`, `em2_3`, `em2_4`, `em2_5`, `text3_0`, `text3_1`, `lang3`, `prob3`, `em3_0`, `em3_1`, `em3_2`, `em3_3`, `em3_4`, `em3_5`, `text4_0`, `text4_1`, `lang4`, `prob4`, `em4_0`, `em4_1`, `em4_2`, `em4_3`, `em4_4`, `em4_5`, `text5_0`, `text5_1`, `lang5`, `prob5`, `em5_0`, `em5_1`, `em5_2`, `em5_3`, `em5_4`, `em5_5`, `text6_0`, `text6_1`, `lang6`, `prob6`, `em6_0`, `em6_1`, `em6_2`, `em6_3`, `em6_4`, `em6_5`, `text7_0`, `text7_1`, `lang7`, `prob7`, `em7_0`, `em7_1`, `em7_2`, `em7_3`, `em7_4`, `em7_5`, `WDBVerified`) VALUES (18847,'','I apologize for the mess. Things are a little hectic right now.$b$bHow can I be of service to you?',0,100,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,18019);

DELETE FROM `gossip_menu` WHERE `MenuID` IN (13310, 13317, 13318, 13311, 13319, 13316, 13320);
INSERT INTO `gossip_menu` (`MenuID`, `TextID`) VALUES
(13310, 18832),
(13317, 18844),
(13318, 18845),
(13311, 18834),
(13319, 18846),
(13316, 18843),
(13320, 18847);

DELETE FROM `gossip_menu_option` WHERE `MenuID` IN (13310, 13317, 13318, 13311, 13319, 13316, 13320);
INSERT INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcflag`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextID`) VALUES
(13310, 0, 0, 'Loon Mai has issued evacuation orders.', 0, 1, 1, 0, 0, 0, 0, '', 0),
(13317, 0, 0, 'Yes, Commander Mai sent me, and I carry his evacuation orders.', 0, 1, 1, 0, 0, 0, 0, '', 0),
(13318, 0, 0, 'Yes, Commander Mai sent me, and I carry his evacuation orders.', 0, 1, 1, 0, 0, 0, 0, '', 0),
(13311, 0, 0, 'Loon Mai has issued evacuation orders.', 0, 1, 1, 0, 0, 0, 0, '', 0),
(13319, 0, 0, 'Loon Mai has issued evacuation orders.', 0, 1, 1, 0, 0, 0, 0, '', 0),
(13316, 0, 0, 'Loon Mai has issued evacuation orders.', 0, 1, 1, 0, 0, 0, 0, '', 0),
(13320, 0, 0, 'Loon Mai has issued evacuation orders.', 0, 1, 1, 0, 0, 0, 0, '', 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` IN (13310, 13317, 13318, 13311, 13319, 13316, 13320);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 13310, 0, 0, 0, 9, 0, 29982, 0, 0, 0, 0, 0, '', 'Wei Blacksoil - Show gossip option if quest Evacuation Orders is taken'),
(15, 13317, 0, 0, 0, 9, 0, 29982, 0, 0, 0, 0, 0, '', 'Feng Spadepaw - Show gossip option if quest Evacuation Orders is taken'),
(15, 13318, 0, 0, 0, 9, 0, 29982, 0, 0, 0, 0, 0, '', 'Shu-Li Spadepaw - Show gossip option if quest Evacuation Orders is taken'),
(15, 13311, 0, 0, 0, 9, 0, 29982, 0, 0, 0, 0, 0, '', 'Haiyun Greentill - Show gossip option if quest Evacuation Orders is taken'),
(15, 13319, 0, 0, 0, 9, 0, 29982, 0, 0, 0, 0, 0, '', 'Ying Greentill - Show gossip option if quest Evacuation Orders is taken'),
(15, 13316, 0, 0, 0, 9, 0, 29982, 0, 0, 0, 0, 0, '', 'Zhang Marlfur - Show gossip option if quest Evacuation Orders is taken'),
(15, 13320, 0, 0, 0, 9, 0, 29982, 0, 0, 0, 0, 0, '', 'Mia Marlfur - Show gossip option if quest Evacuation Orders is taken');

UPDATE `creature_template` SET `npcflag` = `npcflag` | 1, `gossip_menu_id` = CASE `entry` WHEN 57120 THEN 13310 WHEN 57121 THEN 13317 WHEN 57122 THEN 13318 WHEN 57123 THEN 13311 WHEN 57124 THEN 13319 WHEN 57126 THEN 13316 WHEN 57127 THEN 13320 END, `AIName` = 'SmartAI' WHERE `entry` IN (57120, 57121, 57122, 57123, 57124, 57126, 57127);
UPDATE `creature_template` SET `faction_A` = 2550, `faction_H` = 2550, `AIName` = 'SmartAI' WHERE `entry` = 57134;

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (57120, 57121, 57122, 57123, 57124, 57126, 57127, 57134) AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(57120, 0, 0, 1, 62, 0, 100, 0, 13310, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Wei Blacksoil - On gossip select - Close gossip'),
(57120, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Wei Blacksoil - On linked - Say text 0 to player'),
(57120, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 67, 1, 3000, 3000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wei Blacksoil - On linked - Start timer 1 (3 s)'),
(57120, 0, 3, 4, 59, 0, 100, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wei Blacksoil - On timer 1 - Say text 1'),
(57120, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 12, 57134, 4, 25000, 0, 0, 0, 8, 0, 0, 0, -318.643, 2377.642, 140.41, 2.3762, 'Wei Blacksoil - On linked - Summon Ik\'thik Ambusher'),
(57120, 0, 5, 0, 38, 0, 100, 0, 0, 1, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wei Blacksoil - On data 0 = 1 (ambusher died) - Say text 2'),
(57121, 0, 0, 1, 62, 0, 100, 0, 13317, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Feng Spadepaw - On gossip select - Close gossip'),
(57121, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Feng Spadepaw - On linked - Say text 0 to player'),
(57121, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 57121, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Feng Spadepaw - On linked - Player gets quest credit'),
(57122, 0, 0, 1, 62, 0, 100, 0, 13318, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Shu-Li Spadepaw - On gossip select - Close gossip'),
(57122, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 5000, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Shu-Li Spadepaw - On linked - Say text 0 to player'),
(57122, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 57121, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Shu-Li Spadepaw - On linked - Player gets quest credit'),
(57122, 0, 3, 0, 52, 0, 100, 0, 0, 57122, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shu-Li Spadepaw - On text 0 over - Say text 1'),
(57123, 0, 0, 1, 62, 0, 100, 0, 13311, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Haiyun Greentill - On gossip select - Close gossip'),
(57123, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 57123, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Haiyun Greentill - On linked - Player gets quest credit'),
(57124, 0, 0, 1, 62, 0, 100, 0, 13319, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Ying Greentill - On gossip select - Close gossip'),
(57124, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 57123, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Ying Greentill - On linked - Player gets quest credit'),
(57126, 0, 0, 1, 62, 0, 100, 0, 13316, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Zhang Marlfur - On gossip select - Close gossip'),
(57126, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 57126, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Zhang Marlfur - On linked - Player gets quest credit'),
(57127, 0, 0, 1, 62, 0, 100, 0, 13320, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Mia Marlfur - On gossip select - Close gossip'),
(57127, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 57126, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Mia Marlfur - On linked - Player gets quest credit'),
(57134, 0, 0, 0, 54, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 21, 60, 0, 0, 0, 0, 0, 0, 'Ik\'thik Ambusher - On just summoned - Attack nearest player'),
(57134, 0, 1, 0, 0, 0, 100, 1, 2000, 2000, 0, 0, 0, 11, 118355, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Ik\'thik Ambusher - In combat 2 s - Cast Swift Ambush'),
(57134, 0, 2, 0, 0, 0, 100, 0, 7000, 7000, 10000, 12000, 0, 11, 118370, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Ik\'thik Ambusher - In combat 7 s, then every 10-12 s - Cast Slit Throat'),
(57134, 0, 3, 4, 6, 0, 100, 0, 0, 0, 0, 0, 0, 33, 57120, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Ik\'thik Ambusher - On death - Loot recipient gets Wei Blacksoil credit'),
(57134, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 9, 57120, 0, 100, 0, 0, 0, 0, 'Ik\'thik Ambusher - On linked - Tell Wei Blacksoil (data 0 = 1)');
