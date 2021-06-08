DROP TABLE IF EXISTS `worldstate_data`;

CREATE TABLE `worldstate_data` (
  `VariableID` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'WorldState ID (can be 0)',
  `InstanceID` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'WorldState instance',
  `Type` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'WorldStatesData::Types',
  `ConditionID` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Condition (dependent from type)',
  `Flags` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Current flags see WorldStatesData::Falgs',
  `Value` int(10) unsigned NOT NULL DEFAULT '0',
  `RenewTime` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'Time of last renew of WorldState',
  PRIMARY KEY (`VariableID`,`InstanceID`,`Type`,`ConditionID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='WorldState data storage';