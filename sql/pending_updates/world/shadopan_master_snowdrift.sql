-- Shado-Pan Monastery (map 959): Master Snowdrift encounter
-- Script names are those of alexkulya's 5.4.8 DB. Idempotent: plain UPDATEs and DELETE-then-INSERT.
-- Boss texts (creature_text 56541, 56473) and the spawns of the doors (210800, 210862), the entrance and the fire wall (213194, 212908) are not touched here.

UPDATE `creature_template` SET `ScriptName`='boss_master_snowdrift' WHERE `entry`=56541;
UPDATE `creature_template` SET `ScriptName`='npc_snowdrift_clone' WHERE `entry`=56713;
UPDATE `creature_template` SET `ScriptName`='npc_snowdrift_fireball' WHERE `entry`=59225;

DELETE FROM `areatrigger_scripts` WHERE `entry`=7088;
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(7088, 'AreaTrigger_at_before_snowdrift');

-- Snowdrift's Possessions: both chest templates were empty stubs and only the heroic chest was spawned, standing open from the start. The templates and loot are alexkulya's; each difficulty has its own chest, spawned inactive (negative respawn time) until the boss script respawns it.
-- Loot mode 1 is the default mode of chest loot in SkyFire, as in the Terrace chests.
UPDATE `gameobject_template` SET `type`=3, `displayId`=10316, `name`='Snowdrift''s Possessions', `size`=2, `data0`=1634, `data1`=43240, `data2`=0, `data3`=1, `data4`=0, `data5`=0, `data6`=0, `data7`=0, `data8`=0, `data9`=0, `data10`=0, `data11`=1, `data12`=0, `data13`=1, `data14`=0, `data15`=1, `data16`=0, `data17`=0, `data18`=0, `data19`=0, `data20`=90, `data21`=0, `data22`=0, `data23`=0, `data24`=0, `data25`=1304, `data26`=0, `data27`=0, `data28`=0, `data29`=0, `data30`=0, `data31`=0 WHERE `entry`=214518;
UPDATE `gameobject_template` SET `type`=3, `displayId`=10316, `name`='Snowdrift''s Possessions', `size`=2, `data0`=1634, `data1`=41582, `data2`=0, `data3`=1, `data4`=0, `data5`=0, `data6`=0, `data7`=0, `data8`=0, `data9`=0, `data10`=0, `data11`=1, `data12`=0, `data13`=1, `data14`=0, `data15`=1, `data16`=0, `data17`=0, `data18`=0, `data19`=0, `data20`=89, `data21`=0, `data22`=0, `data23`=0, `data24`=0, `data25`=1304, `data26`=0, `data27`=0, `data28`=0, `data29`=0, `data30`=0, `data31`=0 WHERE `entry`=214519;

DELETE FROM `gameobject_loot_template` WHERE `entry` IN (41582, 43240);
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(41582, 80911, 0, 1, 1, 1, 1),
(41582, 80912, 0, 1, 1, 1, 1),
(41582, 80937, 0, 1, 1, 1, 1),
(43240, 81087, 0, 1, 1, 1, 1),
(43240, 81101, 0, 1, 1, 1, 1),
(43240, 81108, 0, 1, 1, 1, 1),
(43240, 81181, 0, 1, 1, 1, 1),
(43240, 81182, 0, 1, 1, 1, 1);

UPDATE `gameobject` SET `spawnMask`=4, `spawntimesecs`=-86400, `animprogress`=0, `state`=1 WHERE `guid`=565 AND `id`=214518;
DELETE FROM `gameobject` WHERE `guid`=1639918 AND `id`=214519;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(1639918, 214519, 959, 2, 0, 0, 3713.53, 3091.72, 817.319, 4.13521, 0, 0, 0.879108, -0.476622, -86400, 0, 1);
