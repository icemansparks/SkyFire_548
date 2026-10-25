-- Quest 24979 "A Scarlet Letter" starts from the item A Scarlet Letter
-- (52079), and nothing spawned dropped it: the only loot row was on Mature
-- Flame Hound (61658), which has no spawn. So the quest could never start.
-- Source: alexkulya 5.4.8 world DB (2026_08_16): Scarlet Warrior (1535, 33
-- spawns around Scarlet Palisade) drops it at 18.5758%. alexkulya also has
-- traces of 0.001% to 0.03% on other Scarlet and Tirisfal mobs; only the
-- Scarlet Warrior row is ported. The Cata TDB has no loot row for 52079.
-- 5.4.8 cross-check: 52079 starts 24979 in the 5.4.8 item data (startquest).
-- Values before this update: no loot row for 52079 on 1535.

DELETE FROM `creature_loot_template` WHERE `entry` = 1535 AND `item` = 52079;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(1535, 52079, 18.5758, 1, 0, 1, 1);
