-- Account-wide mounts, companions and the battle pet unlock, shared by every character of the account.
-- Existing rows of such spells move here from `character_spell` at worldserver startup.
CREATE TABLE IF NOT EXISTS `account_spell` (
  `account` int unsigned NOT NULL DEFAULT '0',
  `spell` mediumint unsigned NOT NULL DEFAULT '0',
  `active` tinyint unsigned NOT NULL DEFAULT '1',
  `disabled` tinyint unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`account`,`spell`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
