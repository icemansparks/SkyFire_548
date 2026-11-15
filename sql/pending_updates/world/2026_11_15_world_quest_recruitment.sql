-- Deathknell (Tirisfal Glades), the two quests that summon Darnell.
-- Neither Mordo nor Deathguard Saltain had a script, so Darnell never came.
--
-- Recruitment (26800) could not be completed: the Scarlet Corpse credit comes
-- from Darnell picking a corpse up. Accepting it now summons Darnell (91938),
-- who follows the player. A corpse within 3 yards of a Darnell has him cast
-- Reverse Cast Ride Vehicle (91945): its kill credit 49340 goes to his summoner,
-- and the corpse boards him (46598, vehicle 1392 has six seats). After the
-- turn-in, Darnell despawns when he next sees his own summoner move.
-- The Shadow Grave (28608): accepting it summons Darnell (91576), who walks to
-- the Shadow Grave, searches it and walks back. Flavour only, the quest items
-- were already obtainable.
--
-- Source: Cata TDB 434.22011 (Deathknell is unchanged in MoP), cross-checked
-- against the 5.4.8 DBC: 91576 and 91938 summon 49141 and 49337 with summon
-- properties 2919, 91945 credits 49340 to the caster's summoner, Vehicle 1392
-- has six seats. The alexkulya 5.4.8 world DB also gives 49337 vehicle 1392.
-- Changes to the source: Darnell follows his summoner explicitly (SkyFire
-- spawns him as a plain guardian without a follow movement), lines with $n
-- target the summoner so the name resolves to the player, and a carried
-- corpse despawns after 5 minutes, so a corpse left behind by a Darnell that
-- vanished without a turn-in comes back. The source has Saltain run the
-- summon after 3 seconds through a shared action list, and on turn-in
-- despawn the closest Darnell and every corpse near him; a second player
-- in Deathknell would lose the summon or their Darnell. Here Saltain casts
-- the summon at once (triggered), and each Darnell despawns himself only
-- for the player who created him. Darnell's Shadow Grave search pause is
-- 8 seconds instead of 6, so his last line comes before he walks on.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (1568, 1740, 49141, 49337, 49340);
UPDATE `creature_template` SET `VehicleId` = 1392 WHERE `entry` = 49337;

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (1568, 1740, 49141, 49337, 49340) AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (4914100, 4914101, 4914102, 4914103, 4914104, 4933700) AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
-- The Shadow Grave
(1568, 0, 0, 0, 19, 0, 100, 0, 28608, 0, 0, 0, 85, 91576, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Undertaker Mordo - On Quest Accept (The Shadow Grave) - Invoker Cast Summon Darnell'),
(49141, 0, 0, 0, 54, 0, 100, 0, 0, 0, 0, 0, 80, 4914100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darnell - On Just Summoned - Run Script'),
(49141, 0, 1, 0, 40, 0, 100, 0, 4, 49141, 0, 0, 80, 4914101, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darnell - On Waypoint 4 Reached - Run Script'),
(49141, 0, 2, 0, 40, 0, 100, 0, 8, 49141, 0, 0, 80, 4914102, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darnell - On Waypoint 8 Reached - Run Script'),
(49141, 0, 3, 0, 40, 0, 100, 0, 9, 49141, 0, 0, 80, 4914103, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darnell - On Waypoint 9 Reached - Run Script'),
(49141, 0, 4, 0, 40, 0, 100, 0, 10, 49141, 0, 0, 80, 4914104, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darnell - On Waypoint 10 Reached - Run Script'),
(49141, 0, 5, 0, 40, 0, 100, 0, 13, 49141, 0, 0, 1, 10, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, 'Darnell - On Waypoint 13 Reached - Say Line 10'),
(49141, 0, 6, 0, 40, 0, 100, 0, 17, 49141, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darnell - On Waypoint 17 Reached - Despawn'),
(4914100, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, 'Darnell - On Script - Say Line 0'),
(4914100, 9, 1, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 1, 1, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, 'Darnell - On Script - Say Line 1'),
(4914100, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 53, 1, 49141, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darnell - On Script - Start Waypoint'),
(4914101, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, 'Darnell - On Script - Say Line 2'),
(4914102, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 54, 4000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darnell - On Script - Pause Waypoint'),
(4914102, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, 'Darnell - On Script - Say Line 3'),
(4914103, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 54, 8000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darnell - On Script - Pause Waypoint'),
(4914103, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 1, 4, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, 'Darnell - On Script - Say Line 4'),
(4914103, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 1, 5, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, 'Darnell - On Script - Say Line 5'),
(4914103, 9, 3, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 1, 6, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, 'Darnell - On Script - Say Line 6'),
(4914103, 9, 4, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 1, 7, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, 'Darnell - On Script - Say Line 7'),
(4914104, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 54, 10000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darnell - On Script - Pause Waypoint'),
(4914104, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 1, 8, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, 'Darnell - On Script - Say Line 8'),
(4914104, 9, 2, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 1, 9, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, 'Darnell - On Script - Say Line 9'),
-- Recruitment
(1740, 0, 0, 0, 19, 0, 100, 0, 26800, 0, 0, 0, 85, 91938, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Deathguard Saltain - On Quest Accept (Recruitment) - Invoker Cast Summon Darnell'),
(49337, 0, 0, 0, 54, 0, 100, 0, 0, 0, 0, 0, 80, 4933700, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darnell - On Just Summoned - Run Script'),
(49337, 0, 1, 0, 10, 0, 100, 0, 1, 30, 1000, 1000, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darnell - On Summoner In Sight With Recruitment Rewarded - Despawn'),
(4933700, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darnell - On Script - Set Passive'),
(4933700, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 29, 2, 3, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, 'Darnell - On Script - Follow Summoner'),
(4933700, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 1, 4, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, 'Darnell - On Script - Say Line 4'),
(4933700, 9, 3, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 1, 5, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, 'Darnell - On Script - Say Line 5'),
(49340, 0, 0, 1, 75, 0, 100, 1, 0, 49337, 3, 0, 86, 91945, 2, 19, 49337, 20, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Corpse - On Darnell Within 3 Yards - Darnell Casts Reverse Cast Ride Vehicle'),
(49340, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 11, 46598, 2, 0, 0, 0, 0, 19, 49337, 20, 0, 0, 0, 0, 0, 'Scarlet Corpse - Linked - Cast Ride Vehicle Hardcoded on Darnell'),
(49340, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 41, 300000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Corpse - Linked - Despawn in 5 Minutes');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceEntry` = 49337 AND `SourceId` = 0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(22, 2, 49337, 0, 0, 8, 0, 26800, 0, 0, 0, 0, 0, '', 'Darnell - despawn only when the unit in sight has Recruitment rewarded'),
(22, 2, 49337, 0, 0, 33, 1, 0, 5, 0, 0, 0, 0, '', 'Darnell - and only when that unit summoned him (created by)');

DELETE FROM `waypoints` WHERE `entry` = 49141;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(49141, 1, 1683.08, 1669.58, 135.688, 'Darnell'),
(49141, 2, 1673.48, 1663.25, 138.565, 'Darnell'),
(49141, 3, 1663.54, 1662.79, 141.851, 'Darnell'),
(49141, 4, 1644.77, 1663.09, 132.478, 'Darnell'),
(49141, 5, 1642.88, 1668.08, 131.366, 'Darnell'),
(49141, 6, 1642.61, 1675.93, 126.932, 'Darnell'),
(49141, 7, 1646.88, 1677.48, 126.52, 'Darnell'),
(49141, 8, 1657.31, 1677.94, 120.712, 'Darnell'),
(49141, 9, 1664.17, 1662.72, 120.719, 'Darnell'),
(49141, 10, 1657.31, 1677.94, 120.772, 'Darnell'),
(49141, 11, 1646.88, 1677.48, 126.52, 'Darnell'),
(49141, 12, 1642.61, 1675.93, 126.932, 'Darnell'),
(49141, 13, 1642.88, 1668.08, 131.366, 'Darnell'),
(49141, 14, 1644.77, 1663.09, 132.478, 'Darnell'),
(49141, 15, 1663.54, 1662.79, 141.851, 'Darnell'),
(49141, 16, 1673.48, 1663.25, 138.565, 'Darnell'),
(49141, 17, 1683.08, 1669.58, 135.688, 'Darnell');

DELETE FROM `creature_text` WHERE `entry` IN (49141, 49337);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(49141, 0, 0, 'Greetings, $n.', 12, 0, 100, 1, 0, 0, 'Darnell'),
(49141, 1, 0, 'The Shadow Grave is this way.  Follow me, $n.', 12, 0, 100, 1, 0, 0, 'Darnell'),
(49141, 2, 0, 'This way!', 12, 0, 100, 1, 0, 0, 'Darnell'),
(49141, 3, 0, 'Now, where could those supplies be?', 12, 0, 100, 1, 0, 0, 'Darnell'),
(49141, 4, 0, 'Maybe they''re over here?', 12, 0, 100, 1, 0, 0, 'Darnell'),
(49141, 5, 0, 'Hmm...', 12, 0, 100, 1, 0, 0, 'Darnell'),
(49141, 6, 0, 'No, not over here.', 12, 0, 100, 1, 0, 0, 'Darnell'),
(49141, 7, 0, 'Hey, give me a hand, $n!  I can''t find the supplies that Mordo needed!', 12, 0, 100, 1, 0, 0, 'Darnell'),
(49141, 8, 0, 'Let''s see now... where could they be...', 12, 0, 100, 1, 0, 0, 'Darnell'),
(49141, 9, 0, 'Nice work!  You''ve found them.  Let''s bring these back to Mordo.', 12, 0, 100, 1, 0, 0, 'Darnell'),
(49141, 10, 0, 'I saw someone up there whose jaw fell off.  I wonder if Mordo can fix him up?', 12, 0, 100, 1, 0, 0, 'Darnell'),
(49337, 4, 0, 'Let''s get moving, $n.  Saltain said that we''d find some corpses up here.', 12, 0, 100, 1, 0, 0, 'Darnell'),
(49337, 5, 0, 'I think I see some corpses up ahead.  Let''s go, $n!  You do the searching and fighting.  I''ll do the lifting.', 12, 0, 100, 0, 0, 0, 'Darnell');

DELETE FROM `locales_creature_text` WHERE `entry` IN (49141, 49337);
INSERT INTO `locales_creature_text` (`entry`, `groupid`, `id`, `text_loc3`) VALUES
(49141, 0, 0, 'Grüße, $n.'),
(49141, 1, 0, 'Das Schattengrab ist in dieser Richtung. Folgt mir, $n.'),
(49141, 2, 0, 'Hier lang!'),
(49141, 3, 0, 'Also, wo könnten diese Vorräte sein?'),
(49141, 4, 0, 'Vielleicht hier drüben?'),
(49141, 5, 0, 'Hmm...'),
(49141, 6, 0, 'Nein, nicht hier drüben.'),
(49141, 7, 0, 'Hey, helft mir mal, $n! Ich kann die Vorräte nicht finden, die Mordo wollte!'),
(49141, 8, 0, 'Mal sehen... wo sind sie nur?'),
(49141, 9, 0, 'Gute Arbeit! Ihr habt sie gefunden. Bringen wir sie zu Mordo zurück.'),
(49141, 10, 0, 'Ich hab da oben jemanden gesehen, der kein Kinn hatte. Vielleicht kann Mordo ihm helfen?'),
(49337, 4, 0, 'Machen wir uns auf den Weg, $n. Saltain sagte, dass wir hier oben ein paar Leichen finden können.'),
(49337, 5, 0, 'Ich glaube, ich sehe da vorn einige Leichen. Los geht''s, $n! Ihr sucht und kämpft, ich kümmere mich um das Tragen.');
