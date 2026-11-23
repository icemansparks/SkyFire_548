-- Shado-Pan Monastery (map 959): Gu Cloudstrike encounter
-- Script names are those of alexkulya's 5.4.8 DB. Idempotent: plain UPDATEs and DELETE-then-INSERT.
-- Boss text groups 9 (phase 3) and 10 (serpent call) and all Azure Serpent lines have no row in alexkulya's DB, so the script's Talk on them stays silent.

UPDATE `creature_template` SET `ScriptName`='boss_gu_cloudstrike' WHERE `entry`=56747;
UPDATE `creature_template` SET `ScriptName`='npc_azure_serpent' WHERE `entry`=56754;
UPDATE `creature_template` SET `ScriptName`='npc_spm_static_field' WHERE `entry`=56803;

-- The serpent is summoned by the boss script; alexkulya has no static spawn, so a spawn row (guid 10033) would double it.
DELETE FROM `creature` WHERE `guid`=10033 AND `id`=56754;
INSERT IGNORE INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(56754, 0, 0, 0, 0, 0, '');
UPDATE `creature_template_addon` SET `bytes1`=50331648 WHERE `entry`=56754;

DELETE FROM `areatrigger_scripts` WHERE `entry`=7656;
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(7656, 'at_gu_intro');

DELETE FROM `spell_script_names` WHERE `spell_id` IN (106941, 107140, 110945, 111205, 114927);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(106941, 'spell_spm_static_field'),
(107140, 'spell_spm_magnetic_shroud'),
(110945, 'spell_spm_charging_soul'),
(111205, 'spell_spm_arc_lightning'),
(114927, 'spell_kill_guardians');

DELETE FROM `creature_text` WHERE `entry`=56747;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(56747, 0, 0, 'Let me show you my power.', 12, 0, 100, 0, 0, 28317, 'Gu Cloudstrike'),
(56747, 1, 0, 'Come to me, serpent!  Together we will destroy these intruders.', 12, 0, 100, 0, 0, 28318, 'Gu Cloudstrike'),
(56747, 2, 0, 'Even together... we failed...', 14, 0, 100, 0, 0, 28319, 'Gu Cloudstrike'),
(56747, 3, 0, 'Their energy is strong!', 14, 0, 100, 0, 0, 28320, 'Gu Cloudstrike'),
(56747, 4, 0, 'No, no! Do not fail me!', 14, 0, 100, 0, 0, 28321, 'Gu Cloudstrike'),
(56747, 5, 0, 'Weaklings... let me make room for our guests.', 14, 0, 100, 0, 0, 28322, 'Gu Cloudstrike'),
(56747, 6, 0, 'Approach... your death awaits.', 14, 0, 100, 0, 0, 28323, 'Gu Cloudstrike'),
(56747, 7, 0, 'Serpent, charge me with your power!', 12, 0, 100, 0, 0, 28324, 'Gu Cloudstrike'),
(56747, 8, 0, 'Weakling!', 14, 0, 100, 0, 0, 28325, 'Gu Cloudstrike'),
(56747, 8, 1, 'Lightning has no mercy!', 14, 0, 100, 0, 0, 28326, 'Gu Cloudstrike');
