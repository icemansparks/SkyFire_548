-- Weekly loot lockouts per character: personal loot (type 0) and bonus rolls (type 1).
-- Name and columns follow alexkulya's 5.4.8 table. entry is the base creature entry,
-- difficulty the DifficultyID of the map the boss died on. Emptied at the weekly reset.
CREATE TABLE IF NOT EXISTS `character_loot_lockout` (
  `guid` int unsigned NOT NULL,
  `entry` int unsigned NOT NULL,
  `difficulty` tinyint unsigned NOT NULL,
  `type` tinyint unsigned NOT NULL,
  PRIMARY KEY (`guid`,`entry`,`difficulty`,`type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
