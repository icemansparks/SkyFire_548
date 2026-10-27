-- Account achievements and their criteria progress, shared by every character of the account.
CREATE TABLE IF NOT EXISTS `account_achievement` (
  `account` int unsigned NOT NULL DEFAULT '0',
  `achievement` smallint unsigned NOT NULL DEFAULT '0',
  `date` int unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`account`,`achievement`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `account_achievement_progress` (
  `account` int unsigned NOT NULL DEFAULT '0',
  `criteria` smallint unsigned NOT NULL DEFAULT '0',
  `counter` bigint unsigned NOT NULL DEFAULT '0',
  `date` int unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`account`,`criteria`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
