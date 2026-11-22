-- Quest 30829 "The Tongue of Ba-Shon" (Kun-Lai Summit, level 87): Lorewalker Cho
-- (60795) offers "Go ahead with the binding, Cho." while the quest is taken. On
-- alexkulya's 5.4.8 core the script npc_lorewalker_cho_bashon casts Summon Cho Force
-- (118996) on the player and gives the player the Summon Cho aura (118953); the
-- summoned Cho (61315, script npc_lorewalker_cho_bashon_summoned) moves into phase 2,
-- says texts 0 and 1, walks to 3485.97 2100.91 1084.13,
-- meditates (118999) and summons Reclaimer Zuan (61305) with Yachi (61307) and Yao
-- (61306); killing Zuan (objective, entry 61305) makes Cho say text 2, end the
-- meditation, remove the aura from the player and despawn. Zuan (script
-- npc_relcaimer_zuan_pets) says text 0, enrages (115430) below 31% health and casts
-- Reclaim Strength (118906) and Wild Magics (120373). There was no Cho spawn (the entry
-- binds a script), no gossip, no AI, and Zuan, Yao, Yachi and Cho (summoned) were
-- friendly (faction 35).
-- Here: Cho's spawn (alexkulya guid 510121, default phase), gossip menu 13723 with the
-- option shown while quest 30829 is taken, the same flow in SmartAI (timed action lists
-- 6131500 and 6131501; delays are relative to the previous step), and hostile faction
-- 14 on Zuan, Yao and Yachi (alexkulya's value). Creature texts for 61315 and 61305 are
-- expected to exist already. Differences from alexkulya: Cho 60795 summons the summoned
-- Cho directly at the player instead of through spells 118996/119017/118953, and nothing
-- moves into phase 2 (no phase aura exists in those spells and this core phases by id),
-- so every player sees the encounter; the 118953 aura is not given. Zuan tells the
-- closest summoned Cho within 100 yards when it dies (SmartAI summons carry no owner
-- guid in this core, so the summoner target finds nothing).
-- Before this update there were no creature rows for 60795, 61315, 61305, 61306, 61307;
-- no gossip_menu, gossip_menu_option, conditions (type 15) or smart_scripts rows for these
-- ids; Cho 60795 had gossip_menu_id 0, npcflag 2, AIName ''; 61315 and 61305 had AIName ''.
DELETE FROM `creature` WHERE `guid` = 510121;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(510121, 60795, 870, 1, 0, 0, 0, 0, 3485.98, 2100.92, 1084.13, 2.41331, 300, 0, 0, 0, 0, 0, 0, 0, 0);

DELETE FROM `gossip_menu` WHERE `MenuID` = 13723;
INSERT INTO `gossip_menu` (`MenuID`, `TextID`) VALUES
(13723, 68);

DELETE FROM `gossip_menu_option` WHERE `MenuID` = 13723;
INSERT INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcflag`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextID`) VALUES
(13723, 0, 0, 'Go ahead with the binding, Cho.', 0, 1, 1, 0, 0, 0, 0, '', 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 13723;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 13723, 0, 0, 0, 9, 0, 30829, 0, 0, 0, 0, 0, '', 'Lorewalker Cho - Show gossip option if quest The Tongue of Ba-Shon is taken');

UPDATE `creature_template` SET `npcflag` = `npcflag` | 1, `gossip_menu_id` = 13723, `AIName` = 'SmartAI' WHERE `entry` = 60795;
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (61315, 61305);
UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14 WHERE `entry` IN (61305, 61306, 61307);

DELETE FROM `smart_scripts` WHERE (`entryorguid` IN (60795, 61315, 61305) AND `source_type` = 0) OR (`entryorguid` IN (6131500, 6131501) AND `source_type` = 9);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(60795, 0, 0, 1, 62, 0, 100, 0, 13723, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Lorewalker Cho - On gossip select - Close gossip'),
(60795, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 12, 61315, 3, 300000, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Lorewalker Cho - On linked - Summon Lorewalker Cho (summoned) at the player'),
(61315, 0, 0, 0, 54, 0, 100, 0, 0, 0, 0, 0, 0, 80, 6131500, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lorewalker Cho (summoned) - On just summoned - Run binding action list'),
(61315, 0, 1, 0, 34, 0, 100, 0, 8, 0, 0, 0, 0, 11, 118999, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lorewalker Cho (summoned) - On reached meditation spot - Cast Cho Meditation'),
(61315, 0, 2, 0, 38, 0, 100, 0, 1, 1, 0, 0, 0, 80, 6131501, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lorewalker Cho (summoned) - On data 1 1 (Zuan defeated) - Run finish action list'),
(6131500, 9, 0, 0, 0, 0, 100, 0, 2500, 2500, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cho binding - After 2.5 s - Say text 0'),
(6131500, 9, 1, 0, 0, 0, 100, 0, 4800, 4800, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cho binding - 4.8 s later - Say text 1'),
(6131500, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 3485.97, 2100.91, 1084.13, 0, 'Cho binding - Move to meditation spot'),
(6131500, 9, 3, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 12, 61305, 3, 300000, 0, 0, 0, 8, 0, 0, 0, 3463.0, 2120.0, 1084.0, 5.6, 'Cho binding - 6 s later - Summon Reclaimer Zuan'),
(6131500, 9, 4, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 12, 61307, 3, 300000, 0, 0, 0, 8, 0, 0, 0, 3460.0, 2118.0, 1084.2, 5.6, 'Cho binding - Summon Yachi'),
(6131500, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 12, 61306, 3, 300000, 0, 0, 0, 8, 0, 0, 0, 3466.0, 2124.0, 1083.0, 5.6, 'Cho binding - Summon Yao'),
(6131500, 9, 6, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 41, 300000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cho binding - Despawn after 300 s'),
(6131501, 9, 0, 0, 0, 0, 100, 0, 3500, 3500, 0, 0, 0, 28, 118999, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cho finish - After 3.5 s - Remove Cho Meditation'),
(6131501, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cho finish - Say text 2'),
(6131501, 9, 2, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 41, 1000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cho finish - 5 s later - Despawn'),
(61305, 0, 0, 0, 54, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Reclaimer Zuan - On just summoned - Say text 0'),
(61305, 0, 1, 0, 2, 0, 100, 1, 0, 30, 0, 0, 0, 11, 115430, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Reclaimer Zuan - At 30% health - Cast Enrage'),
(61305, 0, 2, 0, 0, 0, 100, 0, 5000, 6000, 20000, 20000, 0, 11, 118906, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Reclaimer Zuan - In combat - Cast Reclaim Strength on victim'),
(61305, 0, 3, 0, 0, 0, 100, 0, 15000, 15000, 15000, 15000, 0, 11, 120373, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Reclaimer Zuan - In combat - Cast Wild Magics on victim'),
(61305, 0, 4, 5, 6, 0, 100, 0, 0, 0, 0, 0, 0, 11, 118957, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Reclaimer Zuan - On death - Cast Cho Objective Complete'),
(61305, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 19, 61315, 100, 0, 0, 0, 0, 0, 'Reclaimer Zuan - On linked - Closest Lorewalker Cho (summoned) data 1 1');
