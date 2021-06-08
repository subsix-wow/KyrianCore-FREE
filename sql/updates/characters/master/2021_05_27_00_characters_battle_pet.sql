/*Table structure for table `account_battlepet` */

DROP TABLE IF EXISTS `account_battlepet`;

CREATE TABLE `account_battlepet` (
  `id` BIGINT(20) UNSIGNED NOT NULL AUTO_INCREMENT,
  `account` INT(10) UNSIGNED NOT NULL DEFAULT '0',
  `slot` INT(11) NOT NULL DEFAULT '-1',
  `name` VARCHAR(50) NOT NULL,
  `nameTimeStamp` INT(10) UNSIGNED NOT NULL DEFAULT '0',
  `species` INT(10) UNSIGNED NOT NULL DEFAULT '0',
  `quality` INT(10) UNSIGNED NOT NULL DEFAULT '0',
  `breed` INT(10) UNSIGNED NOT NULL DEFAULT '0',
  `level` INT(10) UNSIGNED NOT NULL DEFAULT '0',
  `xp` INT(10) UNSIGNED NOT NULL DEFAULT '0',
  `display` INT(10) UNSIGNED NOT NULL DEFAULT '0',
  `health` INT(11) NOT NULL DEFAULT '0',
  `flags` INT(10) UNSIGNED NOT NULL DEFAULT '0',
  `infoPower` INT(11) NOT NULL DEFAULT '0',
  `infoMaxHealth` INT(11) NOT NULL DEFAULT '0',
  `infoSpeed` INT(11) NOT NULL DEFAULT '0',
  `infoGender` INT(11) NOT NULL DEFAULT '0',
  `declinedGenitive` VARCHAR(50) NOT NULL,
  `declinedNative` VARCHAR(50) NOT NULL,
  `declinedAccusative` VARCHAR(50) NOT NULL,
  `declinedInstrumental` VARCHAR(50) NOT NULL,
  `declinedPrepositional` VARCHAR(50) NOT NULL,
  PRIMARY KEY (`id`) USING BTREE,
  KEY `account` (`account`) USING BTREE
) ENGINE=INNODB DEFAULT CHARSET=latin1 ROW_FORMAT=COMPACT;

/*Data for the table `account_battlepet` */