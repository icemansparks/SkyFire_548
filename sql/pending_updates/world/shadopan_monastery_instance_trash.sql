-- Shado-Pan Monastery (map 959): instance script and trash scripts
-- Script names are those of alexkulya's 5.4.8 DB. Idempotent: plain UPDATEs and DELETE-then-INSERT.

UPDATE `instance_template` SET `script`='instance_shadopan_monastery' WHERE `map`=959;

UPDATE `creature_template` SET `ScriptName`='npc_shadopan_ambusher' WHERE `entry`=59752;
UPDATE `creature_template` SET `ScriptName`='npc_shadopan_warden' WHERE `entry`=59751;
UPDATE `creature_template` SET `ScriptName`='npc_shadopan_stormbringer' WHERE `entry`=59808;
UPDATE `creature_template` SET `ScriptName`='npc_shadopan_disciple' WHERE `entry`=58198;
UPDATE `creature_template` SET `ScriptName`='npc_shadopan_unstable_energy' WHERE `entry`=59811;
UPDATE `creature_template` SET `ScriptName`='npc_ethereal_sha' WHERE `entry`=65414;
UPDATE `creature_template` SET `ScriptName`='npc_spm_void_sha' WHERE `entry`=65415;
UPDATE `creature_template` SET `ScriptName`='npc_consuming_sha' WHERE `entry`=56764;
UPDATE `creature_template` SET `ScriptName`='npc_destroying_sha' WHERE `entry`=56765;
UPDATE `creature_template` SET `ScriptName`='npc_shadopan_novice' WHERE `entry`=65407;
UPDATE `creature_template` SET `ScriptName`='npc_shadopan_archery' WHERE `entry` IN (56767, 64549);
UPDATE `creature_template` SET `ScriptName`='npc_residual_hatred' WHERE `entry`=58803;
UPDATE `creature_template` SET `ScriptName`='npc_vistige_of_hatred' WHERE `entry`=58807;
UPDATE `creature_template` SET `ScriptName`='npc_fragment_of_hatred' WHERE `entry`=58810;
UPDATE `creature_template` SET `ScriptName`='npc_hateful_essence' WHERE `entry`=58812;

DELETE FROM `areatrigger_scripts` WHERE `entry` IN (7121, 7126, 8271, 8272);
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(7121, 'AreaTrigger_at_shadopan_archery'),
(7126, 'AreaTrigger_at_shadopan_archery'),
(8271, 'AreaTrigger_at_shadopan_archery'),
(8272, 'AreaTrigger_at_shadopan_archery');

-- The archers are summoned by the instance script when a player crosses the begin trigger; a permanent spawn would shoot from the moment the instance loads and double the summoned ones.
DELETE FROM `creature_addon` WHERE `guid` IN (SELECT `guid` FROM `creature` WHERE `map`=959 AND `id` IN (56767, 64549));
DELETE FROM `creature` WHERE `map`=959 AND `id` IN (56767, 64549);
