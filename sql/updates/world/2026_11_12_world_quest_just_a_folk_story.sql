-- Quest 31313 "Just A Folk Story" (Valley of the Four Winds, level 90): the
-- four objectives are "Speak to" Grainsorter Pei (64312), Stonecarver Mac
-- (64315), Mama Min (64313) and Old Man Whitewhiskers (64327). On alexkulya's
-- 5.4.8 core the script npc_just_a_folk_story_quest gives each of them a gossip
-- option; picking it closes the menu, gives the NPC's kill credit and says the
-- NPC's text 0 to the player. TEST had no menu, no option, no npc_text and no
-- AI on them (Pei, Mama Min and Whitewhiskers also lacked the gossip flag), so
-- the quest could not complete.
-- Here: alexkulya's gossip menus 14423, 14424, 14426 and 14430 with their
-- npc_text rows and option text, the option shown only while 31313 is taken,
-- the gossip flag and menu on the four templates, and SmartAI that closes the
-- menu, says text 0 and gives the credit. The creature_text rows were already
-- on TEST.
-- Values before this update, queried on TEST mopworld on 2026-11-12: no
-- gossip_menu, gossip_menu_option, conditions (type 15) or smart_scripts rows
-- for these ids, no npc_text 20348/20350/20352/20361; template npcflag Pei 0,
-- Mama Min 0, Whitewhiskers 0, Mac 2, gossip_menu_id 0 and AIName '' on all four.
DELETE FROM `npc_text` WHERE `ID` IN (20348, 20350, 20352, 20361);
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`, `lang0`, `prob0`, `em0_0`, `em0_1`, `em0_2`, `em0_3`, `em0_4`, `em0_5`, `text1_0`, `text1_1`, `lang1`, `prob1`, `em1_0`, `em1_1`, `em1_2`, `em1_3`, `em1_4`, `em1_5`, `text2_0`, `text2_1`, `lang2`, `prob2`, `em2_0`, `em2_1`, `em2_2`, `em2_3`, `em2_4`, `em2_5`, `text3_0`, `text3_1`, `lang3`, `prob3`, `em3_0`, `em3_1`, `em3_2`, `em3_3`, `em3_4`, `em3_5`, `text4_0`, `text4_1`, `lang4`, `prob4`, `em4_0`, `em4_1`, `em4_2`, `em4_3`, `em4_4`, `em4_5`, `text5_0`, `text5_1`, `lang5`, `prob5`, `em5_0`, `em5_1`, `em5_2`, `em5_3`, `em5_4`, `em5_5`, `text6_0`, `text6_1`, `lang6`, `prob6`, `em6_0`, `em6_1`, `em6_2`, `em6_3`, `em6_4`, `em6_5`, `text7_0`, `text7_1`, `lang7`, `prob7`, `em7_0`, `em7_1`, `em7_2`, `em7_3`, `em7_4`, `em7_5`, `WDBVerified`) VALUES (20348,'The farmers from the nearby fields bring their wheat, and we mill and sort the grains here.$B$BSome of the grain will be made into food, some of it is traded, but most of it is sent to the Stormstout Brewery for... you guessed it... beer!','',0,100,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,18019);
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`, `lang0`, `prob0`, `em0_0`, `em0_1`, `em0_2`, `em0_3`, `em0_4`, `em0_5`, `text1_0`, `text1_1`, `lang1`, `prob1`, `em1_0`, `em1_1`, `em1_2`, `em1_3`, `em1_4`, `em1_5`, `text2_0`, `text2_1`, `lang2`, `prob2`, `em2_0`, `em2_1`, `em2_2`, `em2_3`, `em2_4`, `em2_5`, `text3_0`, `text3_1`, `lang3`, `prob3`, `em3_0`, `em3_1`, `em3_2`, `em3_3`, `em3_4`, `em3_5`, `text4_0`, `text4_1`, `lang4`, `prob4`, `em4_0`, `em4_1`, `em4_2`, `em4_3`, `em4_4`, `em4_5`, `text5_0`, `text5_1`, `lang5`, `prob5`, `em5_0`, `em5_1`, `em5_2`, `em5_3`, `em5_4`, `em5_5`, `text6_0`, `text6_1`, `lang6`, `prob6`, `em6_0`, `em6_1`, `em6_2`, `em6_3`, `em6_4`, `em6_5`, `text7_0`, `text7_1`, `lang7`, `prob7`, `em7_0`, `em7_1`, `em7_2`, `em7_3`, `em7_4`, `em7_5`, `WDBVerified`) VALUES (20350,'','I told my son, I told Dion, don\'t take the yaks to the field when it\'s raining. And what does he do? He takes out the yaks when it is raining.$B$BIt is no surprise that we now have sick yaks!$B$BAi-ya! How did I raise such a foolish son?',0,100,0,0,0,0,0,0,'','My son does not eat enough. Look at him! So scrawny and skinny! I must feed him more.',0,100,0,0,0,0,0,0,'','Why my son must be so short? I wanted tall, strong young son.$B$BI told him not to open umbrellas indoors when he was little. You will grow up short, I say!$B$BWhat does he do? He play with umbrellas all the time! Inside! Tcha!',0,100,0,0,0,0,0,0,'','My son Dion say Mama make the best dumplings in all of Valley!$B$BOoohoohoohooo. That\'s my boy.',0,100,0,0,0,0,0,0,'','Mama loves oranges, but oranges are so hard to find in the Valley.$B$BI tell my son to buy some oranges at the market, so I give him some copper.$B$BWhat does he do? He buys some beans!$B$BWe are not farmers! We are yak-herders! What do I do with stupid beans, I ask you?',0,100,0,0,0,0,0,0,'','My son Dion is out in fields all day. He works so hard. Makes Mama so proud.',0,100,0,0,0,0,0,0,'','Do you come to see Mama? Oh, sit for a while!$B$BLet me tell you, I hear some gossip the other day.$B$BI heard that Gina Mudclaw and her father, Haohan Mudclaw, don\'t get along!$B$BHe wanted to marry her to Farmer Fung up by the Belt, but she wouldn\'t have it! Wanted to run her market, she said.$B$BAi-ya! What has become of the young girls these days? Don\'t want to marry handsome young man and raise a family?$B$BEspecially that Fung... he\'s such a catch! So good-looking, and grows such big vegetables too.$B$BWhy I not have handsome young son like that?!',0,100,0,0,0,0,0,0,'','Oh, when is my son going to meet a nice young lady? All he does is hang around yaks all day!$B$BWhat girl is going to want a stinky yakherd, huh? Ai-ya, Mama worry so much...',0,100,0,0,0,0,0,0,18019);
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`, `lang0`, `prob0`, `em0_0`, `em0_1`, `em0_2`, `em0_3`, `em0_4`, `em0_5`, `text1_0`, `text1_1`, `lang1`, `prob1`, `em1_0`, `em1_1`, `em1_2`, `em1_3`, `em1_4`, `em1_5`, `text2_0`, `text2_1`, `lang2`, `prob2`, `em2_0`, `em2_1`, `em2_2`, `em2_3`, `em2_4`, `em2_5`, `text3_0`, `text3_1`, `lang3`, `prob3`, `em3_0`, `em3_1`, `em3_2`, `em3_3`, `em3_4`, `em3_5`, `text4_0`, `text4_1`, `lang4`, `prob4`, `em4_0`, `em4_1`, `em4_2`, `em4_3`, `em4_4`, `em4_5`, `text5_0`, `text5_1`, `lang5`, `prob5`, `em5_0`, `em5_1`, `em5_2`, `em5_3`, `em5_4`, `em5_5`, `text6_0`, `text6_1`, `lang6`, `prob6`, `em6_0`, `em6_1`, `em6_2`, `em6_3`, `em6_4`, `em6_5`, `text7_0`, `text7_1`, `lang7`, `prob7`, `em7_0`, `em7_1`, `em7_2`, `em7_3`, `em7_4`, `em7_5`, `WDBVerified`) VALUES (20352,'Hello there, traveler. I\'m hard at work making my next masterpiece!','',0,100,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,18019);
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`, `lang0`, `prob0`, `em0_0`, `em0_1`, `em0_2`, `em0_3`, `em0_4`, `em0_5`, `text1_0`, `text1_1`, `lang1`, `prob1`, `em1_0`, `em1_1`, `em1_2`, `em1_3`, `em1_4`, `em1_5`, `text2_0`, `text2_1`, `lang2`, `prob2`, `em2_0`, `em2_1`, `em2_2`, `em2_3`, `em2_4`, `em2_5`, `text3_0`, `text3_1`, `lang3`, `prob3`, `em3_0`, `em3_1`, `em3_2`, `em3_3`, `em3_4`, `em3_5`, `text4_0`, `text4_1`, `lang4`, `prob4`, `em4_0`, `em4_1`, `em4_2`, `em4_3`, `em4_4`, `em4_5`, `text5_0`, `text5_1`, `lang5`, `prob5`, `em5_0`, `em5_1`, `em5_2`, `em5_3`, `em5_4`, `em5_5`, `text6_0`, `text6_1`, `lang6`, `prob6`, `em6_0`, `em6_1`, `em6_2`, `em6_3`, `em6_4`, `em6_5`, `text7_0`, `text7_1`, `lang7`, `prob7`, `em7_0`, `em7_1`, `em7_2`, `em7_3`, `em7_4`, `em7_5`, `WDBVerified`) VALUES (20361,'Eh? What did you say?','',0,100,0,0,0,0,0,0,'Who are you again?','',0,100,0,0,0,0,0,0,'My memory isn\'t what it used to be.','',0,100,0,0,0,0,0,0,'What day is it?','',0,100,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,0,0,0,0,0,0,0,0,18019);

DELETE FROM `gossip_menu` WHERE `MenuID` IN (14423, 14424, 14426, 14430);
INSERT INTO `gossip_menu` (`MenuID`, `TextID`) VALUES
(14423, 20348),
(14424, 20350),
(14426, 20352),
(14430, 20361);

DELETE FROM `gossip_menu_option` WHERE `MenuID` IN (14423, 14424, 14426, 14430);
INSERT INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcflag`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextID`) VALUES
(14423, 0, 0, 'Can you make anything of these clues?', 0, 1, 1, 0, 0, 0, 0, '', 0),
(14424, 0, 0, 'Have you ever heard of a maiden laughing behind a fan?', 0, 1, 1, 0, 0, 0, 0, '', 0),
(14426, 0, 0, 'Where can I find the ancestors\' stair?', 0, 1, 1, 0, 0, 0, 0, '', 0),
(14430, 0, 0, 'Where can someone whisper secrets into the echoing dark?', 0, 1, 1, 0, 0, 0, 0, '', 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` IN (14423, 14424, 14426, 14430);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 14423, 0, 0, 0, 9, 0, 31313, 0, 0, 0, 0, 0, '', 'Grainsorter Pei - Show gossip option if quest Just A Folk Story is taken'),
(15, 14424, 0, 0, 0, 9, 0, 31313, 0, 0, 0, 0, 0, '', 'Mama Min - Show gossip option if quest Just A Folk Story is taken'),
(15, 14426, 0, 0, 0, 9, 0, 31313, 0, 0, 0, 0, 0, '', 'Stonecarver Mac - Show gossip option if quest Just A Folk Story is taken'),
(15, 14430, 0, 0, 0, 9, 0, 31313, 0, 0, 0, 0, 0, '', 'Old Man Whitewhiskers - Show gossip option if quest Just A Folk Story is taken');

UPDATE `creature_template` SET `npcflag` = `npcflag` | 1, `gossip_menu_id` = 14423, `AIName` = 'SmartAI' WHERE `entry` = 64312;
UPDATE `creature_template` SET `npcflag` = `npcflag` | 1, `gossip_menu_id` = 14424, `AIName` = 'SmartAI' WHERE `entry` = 64313;
UPDATE `creature_template` SET `npcflag` = `npcflag` | 1, `gossip_menu_id` = 14426, `AIName` = 'SmartAI' WHERE `entry` = 64315;
UPDATE `creature_template` SET `npcflag` = `npcflag` | 1, `gossip_menu_id` = 14430, `AIName` = 'SmartAI' WHERE `entry` = 64327;

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (64312, 64313, 64315, 64327) AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(64312, 0, 0, 1, 62, 0, 100, 0, 14423, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Grainsorter Pei - On gossip select - Close gossip'),
(64312, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Grainsorter Pei - On linked - Say text 0 to player'),
(64312, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 64312, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Grainsorter Pei - On linked - Player gets quest credit'),
(64313, 0, 0, 1, 62, 0, 100, 0, 14424, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Mama Min - On gossip select - Close gossip'),
(64313, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Mama Min - On linked - Say text 0 to player'),
(64313, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 64313, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Mama Min - On linked - Player gets quest credit'),
(64315, 0, 0, 1, 62, 0, 100, 0, 14426, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Stonecarver Mac - On gossip select - Close gossip'),
(64315, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Stonecarver Mac - On linked - Say text 0 to player'),
(64315, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 64315, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Stonecarver Mac - On linked - Player gets quest credit'),
(64327, 0, 0, 1, 62, 0, 100, 0, 14430, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Old Man Whitewhiskers - On gossip select - Close gossip'),
(64327, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Old Man Whitewhiskers - On linked - Say text 0 to player'),
(64327, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 64327, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Old Man Whitewhiskers - On linked - Player gets quest credit');
