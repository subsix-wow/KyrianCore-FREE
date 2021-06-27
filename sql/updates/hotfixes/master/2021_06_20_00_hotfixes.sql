INSERT INTO `area_table` (`ID`, `ZoneName`, `AreaName`, `ContinentID`, `ParentAreaID`, `AreaBit`, `SoundProviderPref`, `SoundProviderPrefUnderwater`, `AmbienceID`, `UwAmbience`, `ZoneMusic`, `UwZoneMusic`, `IntroSound`, `UwIntroSound`, `FactionGroupMask`, `AmbientMultiplier`, `MountFlags`, `PvpCombatWorldStateID`, `WildBattlePetLevelMin`, `WildBattlePetLevelMax`, `WindSettingsID`, `ContentTuningID`, `Flags1`, `Flags2`, `LiquidTypeID1`, `LiquidTypeID2`, `LiquidTypeID3`, `LiquidTypeID4`, `VerifiedBuild`) VALUES
(6852, 'ProvingGrounds', 'Terreno de Pruebas', 1148, 0, 5480, 0, 0, 0, 0, 1393, 0, 0, 0, 0, 0.15, 15, -1, 0, 0, 0, 632, 4160, 0, 0, 0, 0, 0, 38134);

INSERT INTO `area_group_member` (`ID`, `AreaID`, `AreaGroupID`, `VerifiedBuild`) VALUES
(20683, 3576, 7347, 38134),
(20684, 222, 7347, 38134),
(20685, 159, 7347, 38134),
(20686, 186, 7347, 38134),
(20687, 3665, 7347, 38134),
(20688, 87, 7347, 38134),
(20689, 131, 7347, 38134),
(20690, 362, 7347, 38134),
(20691, 223, 7347, 38134);

INSERT INTO `criteria` (`ID`, `Type`, `Asset`, `ModifierTreeId`, `StartEvent`, `StartAsset`, `StartTimer`, `FailEvent`, `FailAsset`, `Flags`, `EligibilityWorldStateID`, `EligibilityWorldStateValue`, `VerifiedBuild`) VALUES
(51687, 74, 0, 164849, 0, 0, 0, 0, 0, 0, 0, 0, 38134),
(51688, 8, 14960, 0, 0, 0, 0, 0, 0, 0, 0, 0, 38134);

INSERT INTO `phase` (`ID`, `Flags`, `VerifiedBuild`) VALUES
(15146, 20, 38134);

