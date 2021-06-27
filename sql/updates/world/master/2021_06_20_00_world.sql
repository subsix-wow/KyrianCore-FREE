INSERT INTO `areatrigger_template` (`Id`, `Type`, `Flags`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `VerifiedBuild`) VALUES
(23556, 0, 0, 5, 5, 0, 0, 0, 0, 38134),
(22171, 4, 0, 3, 3, 6, 6, 0.300000011920928955, 0.300000011920928955, 38134);

UPDATE `areatrigger_template` SET `Data2`=8, `Data3`=8, `Data4`=3, `Data5`=3, `VerifiedBuild`=38134 WHERE `Id`=4744;
UPDATE `areatrigger_template` SET `Flags`=16, `Data0`=8, `Data1`=8, `Data2`=15, `Data3`=15, `Data4`=-3, `Data5`=-3, `VerifiedBuild`=38134 WHERE `Id`=11393;

INSERT INTO `spell_areatrigger` (`SpellMiscId`, `AreaTriggerId`, `MoveCurveId`, `ScaleCurveId`, `MorphCurveId`, `FacingCurveId`, `AnimId`, `AnimKitId`, `DecalPropertiesId`, `TimeToTarget`, `TimeToTargetScale`, `VerifiedBuild`) VALUES
(4488, 9228, 0, 0, 0, 0, 0, 0, 0, 0, 12000, 38134), -- SpellId : 26573
(510, 3678, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 38134); -- SpellId : 132950

UPDATE `spell_areatrigger` SET `DecalPropertiesId`=0, `TimeToTarget`=0, `TimeToTargetScale`=8000 WHERE (`SpellMiscId`=6887 AND `AreaTriggerId`=11393); -- SpellId : 0
UPDATE `spell_areatrigger` SET `DecalPropertiesId`=0, `TimeToTarget`=326, `TimeToTargetScale`=8000 WHERE (`SpellMiscId`=6887 AND `AreaTriggerId`=11393); -- SpellId : 0
UPDATE `spell_areatrigger` SET `DecalPropertiesId`=0, `TimeToTarget`=6000, `TimeToTargetScale`=6000 WHERE (`SpellMiscId`=18034 AND `AreaTriggerId`=22171); -- SpellId : 0

INSERT INTO `scene_template` (`SceneId`, `Flags`, `ScriptPackageID`, `Encrypted`) VALUES
(2498, 17, 2908, 0),
(2491, 17, 2775, 0),
(2497, 17, 2933, 0),
(2489, 17, 2896, 0),
(2636, 26, 3063, 0),
(2487, 16, 2708, 0),
(2496, 16, 2932, 0),
(2634, 26, 2972, 0),
(2416, 20, 2819, 0);

INSERT INTO `spell_target_position` (`ID`, `EffectIndex`, `MapID`, `PositionX`, `PositionY`, `PositionZ`, `VerifiedBuild`) VALUES
(346952, 0, 1, 1361.949951171875, -4375.25, 26.09000015258789062, 38134), -- Spell: 346952 (Pandaren Horde Teleport) Efffect: 11 (SPELL_EFFECT_BIND)
(325401, 0, 2175, 233.5500030517578125, -2332.52001953125, 80.839996337890625, 38134); -- Spell: 325401 (Rough Landing (DNT)) Efffect: 252 (SPELL_EFFECT_TELEPORT_UNITS)

UPDATE `spell_target_position` SET `PositionX`=1445.2099609375, `PositionY`=-4499.56005859375, `PositionZ`=18.30999946594238281, `VerifiedBuild`=38134 WHERE (`ID`=121852 AND `EffectIndex`=0);
UPDATE `spell_target_position` SET `VerifiedBuild`=38134 WHERE (`ID`=130696 AND `EffectIndex`=0);
UPDATE `spell_target_position` SET `PositionX`=3092.580078125, `PositionY`=3644.719970703125, `VerifiedBuild`=38134 WHERE (`ID`=36890 AND `EffectIndex`=0);
UPDATE `spell_target_position` SET `VerifiedBuild`=38134 WHERE (`ID`=305891 AND `EffectIndex`=0);
UPDATE `spell_target_position` SET `VerifiedBuild`=38134 WHERE (`ID`=319030 AND `EffectIndex`=0);
UPDATE `spell_target_position` SET `VerifiedBuild`=38134 WHERE (`ID`=312548 AND `EffectIndex`=0);

UPDATE `trainer_spell` SET `MoneyCost`=950 WHERE (`TrainerId`=386 AND `SpellId`=33389);

DELETE FROM `npc_vendor` WHERE (`entry`=167213 AND `item`=178120 AND `ExtendedCost`=0 AND `type`=1) OR (`entry`=167213 AND `item`=117 AND `ExtendedCost`=0 AND `type`=1);
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `ExtendedCost`, `type`, `PlayerConditionID`, `IgnoreFiltering`, `VerifiedBuild`) VALUES
(167213, 2, 178120, 0, 0, 1, 0, 0, 38134), -- Horde Tabard
(167213, 1, 117, 0, 0, 1, 0, 0, 38134); -- Tough Jerky

