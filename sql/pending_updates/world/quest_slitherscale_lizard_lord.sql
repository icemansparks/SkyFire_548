-- Quest 31194 "Slitherscale Suppression" (Jade Forest, level 90): kill
-- Slitherscale Lizard-Lord (63536; given and turned in at 58564). The world
-- DB had the template but no spawn, and alexkulya binds
-- npc_slitherscale_lizard_lord. That script, in combat: every 4.5-12 s a
-- random one of Mortal Strike (32736), Slam (130198) or Rend (13738) on the
-- victim (never the same twice in a row); every 10-15 s Spring (130475) on
-- itself or Sunder Armor (64978) on the victim, 50/50; once below 51% health
-- Avatar (130471) on itself. Here: the same with SmartAI. The no-repeat rule
-- is not reproduced (a random timed action list picks uniformly), so the same
-- attack can fire twice in a row. Plus alexkulya's one spawn (guid 504928,
-- default phase, wanders 7.786 yards). All six spells exist in the 5.4.8
-- Spell.dbc. Before this update: no creature of id 63536, no guid 504928, no
-- smart_scripts for 63536 or 6353600-6353612, AIName '' and ScriptName ''.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 63536;

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 63536;
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` BETWEEN 6353600 AND 6353612;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(63536, 0, 0, 0, 0, 0, 100, 0, 4500, 12000, 4500, 12000, 0, 87, 6353600, 6353601, 6353602, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Slitherscale Lizard-Lord - In combat - Run a random warrior attack'),
(63536, 0, 1, 0, 0, 0, 100, 0, 10000, 15000, 10000, 15000, 0, 87, 6353610, 6353611, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Slitherscale Lizard-Lord - In combat - Run Spring or Sunder Armor'),
(63536, 0, 2, 0, 2, 0, 100, 1, 0, 51, 0, 0, 0, 11, 130471, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Slitherscale Lizard-Lord - Below 51% health - Cast Avatar'),
(6353600, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 32736, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Slitherscale Lizard-Lord - Attack 1 - Cast Mortal Strike on victim'),
(6353601, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 130198, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Slitherscale Lizard-Lord - Attack 2 - Cast Slam on victim'),
(6353602, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 13738, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Slitherscale Lizard-Lord - Attack 3 - Cast Rend on victim'),
(6353610, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 130475, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Slitherscale Lizard-Lord - Second attack 1 - Cast Spring on self'),
(6353611, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 64978, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Slitherscale Lizard-Lord - Second attack 2 - Cast Sunder Armor on victim');

DELETE FROM `creature` WHERE `guid` = 504928;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(504928, 63536, 870, 1, 0, 0, 0, 0, 2195.14, -3416.31, 1.35479, 1.3964, 60, 7.786, 0, 0, 0, 1, 0, 0, 0);
