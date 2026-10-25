-- Quest 24959 "Fresh out of the Grave": no new Undead could start the quest
-- chain. Its giver, the Val'kyr Agatha (49044), had no spawn, no AI and no
-- invisibility aura, and nothing let new players see her.
-- Source: alexkulya 5.4.8 world DB (2026_08_16): the spawn above the
-- Deathknell graves, Generic Quest Invisibility 1 (49414) and hover bytes on
-- the template addon, flying inhabit type, the spell_area row that gives
-- Undead players Generic Quest Invisibility Detection 1 (49416) in Deathknell
-- (6454) until the quest is rewarded, and the smart script that casts
-- Val'kyr Resurrection (73524) and removes Rigor Mortis (73523) on accept.
-- The Cata TDB has the same spawn, addon and script; it shows her through
-- phase 169 instead of the detection aura.
-- alexkulya's guid 192768 is taken in SFDB_full_548_26.002, so the spawn uses
-- 372909, free in SFDB and in sql/updates.
-- 5.4.8 cross-check: 49044 starts 24959; 49414, 49416, 73523 and 73524 exist
-- in the 5.4.8 Spell DBC with invisibility and detection type 7.
-- Values before this update: no creature row for 49044, no template addon,
-- AIName '' and InhabitType 3 on 49044, no spell_area row for 49416.

UPDATE `creature_template` SET `AIName` = 'SmartAI', `InhabitType` = 4 WHERE `entry` = 49044;

DELETE FROM `creature_template_addon` WHERE `entry` = 49044;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(49044, 0, 0, 50397184, 4097, 0, '49414');

DELETE FROM `creature` WHERE `guid` = 372909;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(372909, 49044, 0, 1, 0, 0, 0, 0, 1700.2, 1703.36, 137.198, 1.67552, 300, 0, 0, 0, 0, 0, 0, 0, 0);

-- Status masks are 1 << QuestStatus (NONE 0, COMPLETE 1, INCOMPLETE 3, REWARDED 6).
-- With quest_start 0 only the end mask applies: 11 = NONE | COMPLETE | INCOMPLETE,
-- so Undead players (racemask 16) see Agatha until they turn the quest in.
DELETE FROM `spell_area` WHERE `spell` = 49416 AND `area` = 6454;
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(49416, 6454, 0, 24959, 0, 16, 2, 1, 74, 11);

DELETE FROM `smart_scripts` WHERE `entryorguid` = 49044 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(49044, 0, 0, 1, 19, 0, 100, 0, 24959, 0, 0, 0, 0, 11, 73524, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Agatha - On Quest \'Fresh out of the Grave\' Accepted - Cast \'Val\'kyr Resurrection\''),
(49044, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 28, 73523, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Agatha - On Quest \'Fresh out of the Grave\' Accepted - Remove Aura \'Rigor Mortis\'');
