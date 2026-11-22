-- Dread Wastes area triggers that alexkulya's 5.4.8 core binds to C++ scripts
-- (at_q_wood_and_shade, AreaTrigger_q31185, AreaTrigger_q31087). There was no
-- areatrigger_scripts row for 8080, 8123, 8124, 8169, 8230 or 8231, so the credits never
-- came. "Wood and Shade" (31074) needs Heartroot and Mark of the Empress, "Walking Dog"
-- (31185) needs Silt Vents, Shipwreck and Whale Corpse, "Extending Our Coverage" (31087,
-- repeat 31679) needs the Amberglow discovery.
-- Here: each trigger uses SmartTrigger with a credit action for the player who entered,
-- limited by a condition (type 22 smart event, quest state incomplete) to players on the
-- quest.
-- Differences from alexkulya: Walking Dog gives credit without the dog-within-10-yards
-- check and without the dog's line; 8080 gives only the credit (no Shek'zeer Wingblade
-- ambush, no Kor'ik and Adjunct Zet'uk scene), so 31087 still lacks its kill.
-- Before this update there were no areatrigger_scripts, smart_scripts (source type 2) or
-- conditions (source type 22, source id 2) rows for the six triggers.
DELETE FROM `areatrigger_scripts` WHERE `entry` IN (8123,8124,8169,8230,8231,8080);
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(8123, 'SmartTrigger'),
(8124, 'SmartTrigger'),
(8169, 'SmartTrigger'),
(8230, 'SmartTrigger'),
(8231, 'SmartTrigger'),
(8080, 'SmartTrigger');

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (8123,8124,8169,8230,8231,8080) AND `source_type` = 2;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(8123, 2, 0, 0, 46, 0, 100, 0, 8123, 0, 0, 0, 0, 33, 62956, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Wood and Shade trigger - On trigger - Credit Heartroot'),
(8124, 2, 0, 0, 46, 0, 100, 0, 8124, 0, 0, 0, 0, 33, 62955, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Wood and Shade trigger - On trigger - Credit Mark of the Empress'),
(8169, 2, 0, 0, 46, 0, 100, 0, 8169, 0, 0, 0, 0, 33, 63879, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Walking Dog trigger - On trigger - Credit Silt Vents'),
(8230, 2, 0, 0, 46, 0, 100, 0, 8230, 0, 0, 0, 0, 33, 63880, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Walking Dog trigger - On trigger - Credit Shipwreck'),
(8231, 2, 0, 0, 46, 0, 100, 0, 8231, 0, 0, 0, 0, 33, 63881, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Walking Dog trigger - On trigger - Credit Whale Corpse'),
(8080, 2, 0, 0, 46, 0, 100, 0, 8080, 0, 0, 0, 0, 33, 65328, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Extending Our Coverage trigger - On trigger - Credit Amberglow');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceId` = 2 AND `SourceEntry` IN (8123,8124,8169,8230,8231,8080);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(22, 1, 8123, 2, 0, 47, 0, 31074, 8, 0, 0, 0, 0, '', 'Wood and Shade trigger - only with quest 31074 incomplete'),
(22, 1, 8124, 2, 0, 47, 0, 31074, 8, 0, 0, 0, 0, '', 'Wood and Shade trigger - only with quest 31074 incomplete'),
(22, 1, 8169, 2, 0, 47, 0, 31185, 8, 0, 0, 0, 0, '', 'Walking Dog trigger - only with quest 31185 incomplete'),
(22, 1, 8230, 2, 0, 47, 0, 31185, 8, 0, 0, 0, 0, '', 'Walking Dog trigger - only with quest 31185 incomplete'),
(22, 1, 8231, 2, 0, 47, 0, 31185, 8, 0, 0, 0, 0, '', 'Walking Dog trigger - only with quest 31185 incomplete'),
(22, 1, 8080, 2, 0, 47, 0, 31087, 8, 0, 0, 0, 0, '', 'Extending Our Coverage trigger - only with quest 31087 incomplete'),
(22, 1, 8080, 2, 1, 47, 0, 31679, 8, 0, 0, 0, 0, '', 'Extending Our Coverage trigger - only with quest 31679 incomplete');
