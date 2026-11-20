-- Personal loot of world bosses, raid finder and flex bosses (5.4.8), one roll per boss per week.
-- Names and columns follow alexkulya's 5.4.8 tables; rows key the base creature entry.
-- money_bag: gold bag item given when the roll fails or no item fits the player's specialization;
-- money_bag_flex: the flex bag (0 = money_bag). quest: weekly tracking quest marked done on receipt.
-- legendary_quest_loot is kept for the column layout; the core does not read it.
-- Rows: the next update file, from alexkulya/pandaria_5.4.8.
CREATE TABLE IF NOT EXISTS `personal_loot_template` (
  `entry` int(10) unsigned NOT NULL,
  `money_bag` int(10) unsigned NOT NULL,
  `money_bag_flex` int(10) unsigned NOT NULL DEFAULT 0,
  `quest` int(10) unsigned NOT NULL DEFAULT 0,
  `legendary_quest_loot` set('MOGU_SEALS','SECRETS_OF_THE_EMPIRE','TITAN_RUNESTONE','CHIMERA_OF_FEAR','HEART_OF_THUNDER_KING') DEFAULT NULL,
  `comment` text,
  PRIMARY KEY (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Items a boss can give; the core keeps those fitting the player's loot specialization and,
-- by the item's "Flexible" or other name description, the kill's difficulty.
CREATE TABLE IF NOT EXISTS `personal_loot_item` (
  `entry` int(10) unsigned NOT NULL,
  `item` int(10) unsigned NOT NULL,
  PRIMARY KEY (`entry`, `item`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
