-- Shado-Pan Monastery (map 959): Taran Zhu encounter
-- The boss (56884, ScriptName already bound), its texts, its loot and the Gripping Hatred template (59804, ScriptName already bound) are not touched here.
-- Added here: the spell script bindings, alexkulya's boss flags and immunities, the two stash chests and their loot.
-- Idempotent: plain UPDATEs and DELETE-then-INSERT.

UPDATE `creature_template` SET `flags_extra`=1, `mechanic_immune_mask`=770424831 WHERE `entry`=56884;
UPDATE `creature_template` SET `flags_extra`=0, `mechanic_immune_mask`=803979263, `InhabitType`=4 WHERE `entry`=59804;

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_taran_zhu_hate', 'spell_taran_zhu_meditation', 'spell_taran_zhu_ring_of_malice', 'spell_taran_zhu_grip_of_hate');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(107085, 'spell_taran_zhu_hate'),
(107200, 'spell_taran_zhu_meditation'),
(112933, 'spell_taran_zhu_ring_of_malice'),
(131522, 'spell_taran_zhu_ring_of_malice'),
(115010, 'spell_taran_zhu_grip_of_hate');

-- Taran Zhu's Personal Stash: both chest templates were empty stubs and only the heroic chest was spawned, open from the start. The templates and loot are alexkulya's;
-- each difficulty has its own chest, spawned inactive (negative respawn time) until the instance script respawns it when the boss is done.
-- Loot mode 1 is the default mode of chest loot in SkyFire. Each chest rolls its reference table twice, as in alexkulya's loot.
UPDATE `gameobject_template` SET `type`=3, `displayId`=10316, `name`='Taran Zhu''s Personal Stash', `size`=2, `data0`=1634, `data1`=41584, `data3`=1, `data10`=1, `data11`=1, `data12`=1, `data13`=1, `data15`=1, `data20`=89, `data25`=1306 WHERE `entry`=213888;
UPDATE `gameobject_template` SET `type`=3, `displayId`=10316, `name`='Taran Zhu''s Personal Stash', `size`=2, `data0`=1634, `data1`=43242, `data3`=1, `data10`=1, `data11`=1, `data12`=1, `data13`=1, `data15`=1, `data20`=90, `data25`=1306 WHERE `entry`=213889;

DELETE FROM `reference_loot_template` WHERE `entry` IN (80936, 81107);
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(80936, 80916, 17, 1, 1, 1, 1),
(80936, 80917, 17, 1, 1, 1, 1),
(80936, 80918, 17, 1, 1, 1, 1),
(80936, 80919, 17, 1, 1, 1, 1),
(80936, 80935, 17, 1, 1, 1, 1),
(80936, 80936, 15, 1, 1, 1, 1),
(81107, 81093, 10, 1, 1, 1, 1),
(81107, 81096, 10, 1, 1, 1, 1),
(81107, 81099, 10, 1, 1, 1, 1),
(81107, 81103, 10, 1, 1, 1, 1),
(81107, 81107, 10, 1, 1, 1, 1),
(81107, 81114, 10, 1, 1, 1, 1),
(81107, 81186, 10, 1, 1, 1, 1),
(81107, 81187, 10, 1, 1, 1, 1),
(81107, 81188, 10, 1, 1, 1, 1),
(81107, 81189, 10, 1, 1, 1, 1);

DELETE FROM `gameobject_loot_template` WHERE `entry` IN (41584, 43242);
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(41584, 80936, 100, 1, 0, -80936, 2),
(43242, 81107, 100, 1, 0, -81107, 2),
(43242, 87543, 1.5, 1, 0, 1, 1);

UPDATE `gameobject` SET `spawnMask`=4, `spawntimesecs`=-86400, `animprogress`=255, `state`=1 WHERE `guid`=582 AND `id`=213889;
DELETE FROM `gameobject` WHERE `guid`=1639919 AND `id`=213888;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(1639919, 213888, 959, 2, 0, 0, 3880.16, 2586.06, 757.201, 2.05339, 0, 0, 0, 1, -86400, 255, 1);
