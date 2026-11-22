-- Shado-Pan Monastery (map 959): the adds of the Master Snowdrift event
-- Script names are those of alexkulya's 5.4.8 DB. Idempotent: plain UPDATEs and DELETEs. The replacement spawns are summoned by the scripts at runtime.

UPDATE `creature_template` SET `ScriptName`='npc_spm_pandaren_referee' WHERE `entry`=56505;
UPDATE `creature_template` SET `ScriptName`='npc_spm_jade_staff' WHERE `entry`=56678;
UPDATE `creature_template` SET `ScriptName`='npc_snowdrift_novice' WHERE `entry`=56395;
UPDATE `creature_template` SET `ScriptName`='npc_snowdrift_miniboss' WHERE `entry` IN (56472, 56473);

-- The Jade Staff boards Fragrant Lotus (the script calls EnterVehicle), so she needs her vehicle.
UPDATE `creature_template` SET `VehicleId`=1890 WHERE `entry`=56472;

-- Novices, Fragrant Lotus and Flying Snow are summoned by the referee; alexkulya has no static spawn of them, so existing static spawn rows (13 on the checked world DB) would double them and are removed.
DELETE FROM `creature_addon` WHERE `guid` IN (SELECT `guid` FROM `creature` WHERE `map`=959 AND `id` IN (56395, 56472, 56473));
DELETE FROM `creature` WHERE `map`=959 AND `id` IN (56395, 56472, 56473);
