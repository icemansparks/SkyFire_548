-- Dread Wastes: "On the Crab" (31187) and the Silent Beacon objective of "Extending Our
-- Coverage" (31087, repeat 31679).
-- alexkulya binds go_full_crab_pot (credit 64006 when a player on 31187 uses a pot),
-- go_silent_beacon and AreaTrigger_q31087 (summons Adjunct Zet'uk 65478 on trigger 8080) to
-- C++ scripts. The Full Crab Pot (213508) had a template but no script, so the eight
-- credits never came, and only 3 of alexkulya's 15 pots were spawned. The Silent Beacon
-- (213250), a use objective of 31087 and 31679, was a stub template (name "unk name",
-- type 0, data3 0) without spawn.
-- Here: SmartGameObjectAI on 213508 credits the user on use. 213250 gets alexkulya's name,
-- type 10 and data3 3000, plus alexkulya's spawn.
-- Differences from alexkulya: no crab trap visual summon; the Silent Beacon makes no Kor'ik
-- speak. Not covered: the Adjunct Zet'uk kill of 31087 (alexkulya's trigger 8080 summons it,
-- plus a Shek'zeer Wingblade ambush and Kor'ik): a SmartTrigger summon has no base object
-- and would crash, so it needs a C++ trigger script.
-- Before this update only crab pots 261266, 261267 and 261269 were spawned (kept), no
-- gameobject rows existed for the guids below, gameobject_template 213250 had name
-- 'unk name', type 0, data3 0, and 213508 had an empty AIName and no smart_scripts
-- (source type 1).
DELETE FROM `gameobject` WHERE `guid` IN (525727,525728,525729,525730,525731,525732,525733,525734,525736,525742,525836,525849,525850,528901);
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(525727, 213508, 870, 1, 0, 0, -1119.94, 3793, -18.3221, 6.20169, 0, 0, 0, 1, 120, 255, 1),
(525728, 213508, 870, 1, 0, 0, -1262.92, 3714.33, -45.8073, 6.20169, 0, 0, 0, 1, 120, 255, 1),
(525729, 213508, 870, 1, 0, 0, -1349.86, 3692.92, -45.0088, 0.686481, 0, 0, 0, 1, 120, 255, 1),
(525730, 213508, 870, 1, 0, 0, -1234.16, 3634.33, -59.1007, 6.20169, 0, 0, 0, 1, 120, 255, 1),
(525731, 213508, 870, 1, 0, 0, -1301.76, 3681.49, -45.2169, 0.948659, 0, 0, 0, 1, 120, 255, 1),
(525732, 213508, 870, 1, 0, 0, -1381.61, 3652.4, -59.8042, 0.709052, 0, 0, 0, 1, 120, 255, 1),
(525733, 213508, 870, 1, 0, 0, -1348.07, 3633.41, -43.7743, 0.87918, 0, 0, 0, 1, 120, 255, 1),
(525734, 213508, 870, 1, 0, 0, -1259.18, 3578.65, -41.0018, 0.598933, 0, 0, 0, 1, 120, 255, 1),
(525736, 213508, 870, 1, 0, 0, -1269.42, 3880.66, -45.2066, 6.20169, 0, 0, 0, 1, 120, 255, 1),
(525742, 213508, 870, 1, 0, 0, -1291.52, 3510.52, -40.2003, 6.20169, 0, 0, 0, 1, 120, 255, 1),
(525836, 213508, 870, 1, 0, 0, -1099.81, 3722.54, -20.209, 0.85309, 0, 0, 0, 1, 120, 255, 1),
(525849, 213508, 870, 1, 0, 0, -1057.68, 3685.67, -18.2847, 4.54997, 0, 0, 0, 1, 120, 255, 1),
(525850, 213508, 870, 1, 0, 0, -1133.94, 3659.32, -59.8042, 6.20169, 0, 0, 0, 1, 120, 255, 1),
(528901, 213250, 870, 1, 0, 0, -357.299, 3561.73, 39.8547, 2.48087, 0, 0, 0, 1, 120, 255, 1);

UPDATE `gameobject_template` SET `name` = 'Silent Beacon', `type` = 10, `data3` = 3000 WHERE `entry` = 213250;
UPDATE `gameobject_template` SET `AIName` = 'SmartGameObjectAI' WHERE `entry` = 213508;

DELETE FROM `smart_scripts` WHERE `entryorguid` = 213508 AND `source_type` = 1;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(213508, 1, 0, 0, 64, 0, 100, 0, 0, 0, 0, 0, 0, 33, 64006, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Full Crab Pot - On use - Credit Crab Trap');
