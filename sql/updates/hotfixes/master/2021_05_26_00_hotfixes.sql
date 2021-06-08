DELETE FROM hotfix_blob where Tablehash = 0x383B4C27;

DROP TABLE IF EXISTS `battle_pet_ability`;

CREATE TABLE `battle_pet_ability` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text COLLATE utf8mb4_unicode_ci,
  `Description` text COLLATE utf8mb4_unicode_ci,
  `IconFileDataID` int(11) NOT NULL DEFAULT '0',
  `PetTypeEnum` tinyint(4) NOT NULL DEFAULT '0',
  `Cooldown` int(10) unsigned NOT NULL DEFAULT '0',
  `BattlePetVisualID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Flags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`VerifiedBuild`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

/*Data for the table `battle_pet_ability` */

/*Table structure for table `battle_pet_ability_effect` */

DROP TABLE IF EXISTS `battle_pet_ability_effect`;

CREATE TABLE `battle_pet_ability_effect` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `BattlePetAbilityTurnID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `OrderIndex` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `BattlePetEffectPropertiesID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `AuraBattlePetAbilityID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `BattlePetVisualID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Param1` smallint(6) NOT NULL DEFAULT '0',
  `Param2` smallint(6) NOT NULL DEFAULT '0',
  `Param3` smallint(6) NOT NULL DEFAULT '0',
  `Param4` smallint(6) NOT NULL DEFAULT '0',
  `Param5` smallint(6) NOT NULL DEFAULT '0',
  `Param6` smallint(6) NOT NULL DEFAULT '0',
  `VerifiedBuild` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`VerifiedBuild`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

/*Data for the table `battle_pet_ability_effect` */

/*Table structure for table `battle_pet_ability_locale` */

DROP TABLE IF EXISTS `battle_pet_ability_locale`;

CREATE TABLE `battle_pet_ability_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) COLLATE utf8mb4_unicode_ci NOT NULL,
  `Name_lang` text COLLATE utf8mb4_unicode_ci,
  `Description_lang` text COLLATE utf8mb4_unicode_ci,
  `VerifiedBuild` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`,`VerifiedBuild`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci
/*!50500 PARTITION BY LIST  COLUMNS(locale)
(PARTITION deDE VALUES IN ('deDE') ENGINE = InnoDB,
 PARTITION esES VALUES IN ('esES') ENGINE = InnoDB,
 PARTITION esMX VALUES IN ('esMX') ENGINE = InnoDB,
 PARTITION frFR VALUES IN ('frFR') ENGINE = InnoDB,
 PARTITION itIT VALUES IN ('itIT') ENGINE = InnoDB,
 PARTITION koKR VALUES IN ('koKR') ENGINE = InnoDB,
 PARTITION ptBR VALUES IN ('ptBR') ENGINE = InnoDB,
 PARTITION ruRU VALUES IN ('ruRU') ENGINE = InnoDB,
 PARTITION zhCN VALUES IN ('zhCN') ENGINE = InnoDB,
 PARTITION zhTW VALUES IN ('zhTW') ENGINE = InnoDB) */;

/*Data for the table `battle_pet_ability_locale` */

/*Table structure for table `battle_pet_ability_state` */

DROP TABLE IF EXISTS `battle_pet_ability_state`;

CREATE TABLE `battle_pet_ability_state` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `BattlePetStateID` int(10) unsigned NOT NULL DEFAULT '0',
  `Value` int(11) NOT NULL DEFAULT '0',
  `BattlePetAbilityID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`VerifiedBuild`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

/*Data for the table `battle_pet_ability_state` */

/*Table structure for table `battle_pet_ability_turn` */

DROP TABLE IF EXISTS `battle_pet_ability_turn`;

CREATE TABLE `battle_pet_ability_turn` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `BattlePetAbilityID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `OrderIndex` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `TurnTypeEnum` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `EventTypeEnum` tinyint(4) NOT NULL DEFAULT '0',
  `BattlePetVisualID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`VerifiedBuild`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

/*Data for the table `battle_pet_ability_turn` */

/*Table structure for table `battle_pet_breed_quality` */

DROP TABLE IF EXISTS `battle_pet_breed_quality`;

CREATE TABLE `battle_pet_breed_quality` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `StateMultiplier` float NOT NULL DEFAULT '0',
  `QualityEnum` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`VerifiedBuild`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

/*Data for the table `battle_pet_breed_quality` */

/*Table structure for table `battle_pet_breed_state` */

DROP TABLE IF EXISTS `battle_pet_breed_state`;

CREATE TABLE `battle_pet_breed_state` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `BattlePetStateID` int(11) NOT NULL DEFAULT '0',
  `Value` smallint(5) unsigned NOT NULL DEFAULT '0',
  `BattlePetBreedID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`VerifiedBuild`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

/*Data for the table `battle_pet_breed_state` */

/*Table structure for table `battle_pet_species` */

DROP TABLE IF EXISTS `battle_pet_species`;

CREATE TABLE `battle_pet_species` (
  `Description` text COLLATE utf8mb4_unicode_ci,
  `SourceText` text COLLATE utf8mb4_unicode_ci,
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `CreatureID` int(11) NOT NULL DEFAULT '0',
  `SummonSpellID` int(11) NOT NULL DEFAULT '0',
  `IconFileDataID` int(11) NOT NULL DEFAULT '0',
  `PetTypeEnum` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Flags` smallint(5) unsigned NOT NULL DEFAULT '0',
  `SourceTypeEnum` tinyint(4) NOT NULL DEFAULT '0',
  `CardUIModelSceneID` int(11) NOT NULL DEFAULT '0',
  `LoadoutUIModelSceneID` int(11) NOT NULL DEFAULT '0',
  `CovenantID` int(11) NOT NULL DEFAULT '0',
  `VerifiedBuild` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`VerifiedBuild`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

/*Data for the table `battle_pet_species` */

insert  into `battle_pet_species`(`Description`,`SourceText`,`ID`,`CreatureID`,`SummonSpellID`,`IconFileDataID`,`PetTypeEnum`,`Flags`,`SourceTypeEnum`,`CardUIModelSceneID`,`LoadoutUIModelSceneID`,`CovenantID`,`VerifiedBuild`) values 
('What vile subtance is jiggling around inside of this creature?','|cFFFFD200Treasure:|r Strange Growth|n|cFFFFD200Zone:|r Maldraxxus|n',3013,173534,339677,3860396,4,2,0,6,7,0,37474);

/*Table structure for table `battle_pet_species_locale` */

DROP TABLE IF EXISTS `battle_pet_species_locale`;

CREATE TABLE `battle_pet_species_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) COLLATE utf8mb4_unicode_ci NOT NULL,
  `Description_lang` text COLLATE utf8mb4_unicode_ci,
  `SourceText_lang` text COLLATE utf8mb4_unicode_ci,
  `VerifiedBuild` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`,`VerifiedBuild`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci
/*!50500 PARTITION BY LIST  COLUMNS(locale)
(PARTITION deDE VALUES IN ('deDE') ENGINE = InnoDB,
 PARTITION esES VALUES IN ('esES') ENGINE = InnoDB,
 PARTITION esMX VALUES IN ('esMX') ENGINE = InnoDB,
 PARTITION frFR VALUES IN ('frFR') ENGINE = InnoDB,
 PARTITION itIT VALUES IN ('itIT') ENGINE = InnoDB,
 PARTITION koKR VALUES IN ('koKR') ENGINE = InnoDB,
 PARTITION ptBR VALUES IN ('ptBR') ENGINE = InnoDB,
 PARTITION ruRU VALUES IN ('ruRU') ENGINE = InnoDB,
 PARTITION zhCN VALUES IN ('zhCN') ENGINE = InnoDB,
 PARTITION zhTW VALUES IN ('zhTW') ENGINE = InnoDB) */;

/*Data for the table `battle_pet_species_locale` */

insert  into `battle_pet_species_locale`(`ID`,`locale`,`Description_lang`,`SourceText_lang`,`VerifiedBuild`) values 
(3013,'deDE','Welche widerliche Substanz wabbelt in dieser Kreatur herum?','|cFFFFD200Schatz: |rSeltsamer Auswuchs|n|cFFFFD200Zone: |rMaldraxxus|n',37474),
(3013,'esES','Â¿QuÃ© sustancia vil se agita en el interior de esta criatura?','|cFFFFD200Tesoro:|r Crecimiento raro|n|cFFFFD200Zona:|r Maldraxxus|n',37474),
(3013,'esMX','Â¿QuÃ© sustancia vil se mueve por el interior de esta criatura?','|cFFFFD200Tesoro:|r Crecimiento anormal|n|cFFFFD200Zona:|r Maldraxxus|n',37474),
(3013,'frFR','De quelle vile substance cette crÃ©ature est-elle constituÃ©eÂ ?','|cFFFFD200TrÃ©sorÂ : |rÃ‰trange excroissance|n|cFFFFD200RÃ©gionÂ : |rMaldraxxus|n',37474),
(3013,'itIT','Quale vile sostanza si agita dentro questa creatura?','|cFFFFD200Tesoro: |rCrescita Strana|n|cFFFFD200Zona: |rMaldraxxus',37474),
(3013,'koKR','ì´ ìƒë¬¼ì˜ ë°° ì†ì—ì„œëŠ” ì–´ë–¤ ë”ì°í•œ ë¬¼ì§ˆì´ ì°°ëž‘ê±°ë¦¬ê³  ìžˆëŠ” ê±¸ê¹Œìš”?','|cFFFFD200ë³´ë¬¼: |rì´ìƒí•œ ì¦ì‹ë¬¼|n|cFFFFD200ì§€ì—­: |rë§ë“œë½ì„œìŠ¤|n',37474),
(3013,'ptBR','Que substÃ¢ncia pavorosa Ã© essa que sacoleja dentro desta coisa?','|cFFFFD200Tesouro: |rBrotaÃ§Ã£o Estranha|n|cFFFFD200Ãrea: |rMaldraxxus',37474),
(3013,'ruRU','Ð§Ñ‚Ð¾ Ð·Ð° Ð¾Ð¼ÐµÑ€Ð·Ð¸Ñ‚ÐµÐ»ÑŒÐ½Ð°Ñ ÑÑƒÐ±ÑÑ‚Ð°Ð½Ñ†Ð¸Ñ Ð±ÑƒÐ»Ñ‚Ñ‹Ñ…Ð°ÐµÑ‚ÑÑ Ð² ÑÑ‚Ð¾Ð¼ ÑÑƒÑ‰ÐµÑÑ‚Ð²Ðµ?','|cFFFFD200Ð¡Ð¾ÐºÑ€Ð¾Ð²Ð¸Ñ‰Ðµ: |rÑÑ‚Ñ€Ð°Ð½Ð½Ñ‹Ð¹ Ð½Ð°Ñ€Ð¾ÑÑ‚|n|cFFFFD200Ð—Ð¾Ð½Ð°: |rÐœÐ°Ð»Ð´Ñ€Ð°ÐºÑÑƒÑ|n',37474),
(3013,'zhCN','è¿™ä¸œè¥¿çš„è‚šå­æ¶ŒåŠ¨ç€ä»€ä¹ˆé‚ªæ¶çš„ä¸œè¥¿ï¼Ÿ','|cFFFFD200å®è—ï¼š|rå¥‡å¼‚å¢žç”Ÿ|n|cFFFFD200åœ°åŒºï¼š|rçŽ›å“å…‹è¨æ–¯|n',37474),
(3013,'zhTW','é€™ç”Ÿç‰©é«”å…§æµå‹•çš„ï¼Œåˆ°åº•æ˜¯ä»€éº¼é‚ªæƒ¡çš„ç‰©è³ªï¼Ÿ','|cFFFFD200å¯¶è—ï¼š |ræ€ªç•°çš„ç‰©é«”|n|cFFFFD200å€åŸŸï¼š|rç‘ªå“è–©æ–¯|n',37474);

/*Table structure for table `battle_pet_species_state` */

DROP TABLE IF EXISTS `battle_pet_species_state`;

CREATE TABLE `battle_pet_species_state` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `BattlePetStateID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Value` int(11) NOT NULL DEFAULT '0',
  `BattlePetSpeciesID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`VerifiedBuild`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

/*Data for the table `battle_pet_species_state` */

insert  into `battle_pet_species_state`(`ID`,`BattlePetStateID`,`Value`,`BattlePetSpeciesID`,`VerifiedBuild`) values 
(10981,183,2428,2978,37474),
(11263,183,316,2978,37474);

/*Table structure for table `battle_pet_species_x_ability` */

DROP TABLE IF EXISTS `battle_pet_species_x_ability`;

CREATE TABLE `battle_pet_species_x_ability` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `BattlePetAbilityID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RequiredLevel` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `SlotEnum` tinyint(4) NOT NULL DEFAULT '0',
  `BattlePetSpeciesID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`VerifiedBuild`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

/*Data for the table `battle_pet_species_x_ability` */

/*Table structure for table `battle_pet_state` */

DROP TABLE IF EXISTS `battle_pet_state`;

CREATE TABLE `battle_pet_state` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `LuaName` text COLLATE utf8mb4_unicode_ci,
  `Flags` smallint(5) unsigned NOT NULL DEFAULT '0',
  `BattlePetVisualID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`VerifiedBuild`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

/*Data for the table `battle_pet_state` */

/*Table structure for table `battle_pet_state_locale` */

DROP TABLE IF EXISTS `battle_pet_state_locale`;

CREATE TABLE `battle_pet_state_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) COLLATE utf8mb4_unicode_ci NOT NULL,
  `LuaName_lang` text COLLATE utf8mb4_unicode_ci,
  `VerifiedBuild` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`,`VerifiedBuild`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci
/*!50500 PARTITION BY LIST  COLUMNS(locale)
(PARTITION deDE VALUES IN ('deDE') ENGINE = InnoDB,
 PARTITION esES VALUES IN ('esES') ENGINE = InnoDB,
 PARTITION esMX VALUES IN ('esMX') ENGINE = InnoDB,
 PARTITION frFR VALUES IN ('frFR') ENGINE = InnoDB,
 PARTITION itIT VALUES IN ('itIT') ENGINE = InnoDB,
 PARTITION koKR VALUES IN ('koKR') ENGINE = InnoDB,
 PARTITION ptBR VALUES IN ('ptBR') ENGINE = InnoDB,
 PARTITION ruRU VALUES IN ('ruRU') ENGINE = InnoDB,
 PARTITION zhCN VALUES IN ('zhCN') ENGINE = InnoDB,
 PARTITION zhTW VALUES IN ('zhTW') ENGINE = InnoDB) */;

/*Data for the table `battle_pet_state_locale` */


