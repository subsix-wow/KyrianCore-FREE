DELETE FROM `creature` WHERE `guid`=60000000000025301;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnDifficulties`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES (60000000000025301, 170624, 2364, 0, 0, '0', 0, 0, 0, -1, 0, 1, 4158.67, 7869.11, 4969, 5.90197, 300, 0, 0, 7477, 1807, 0, 0, 0, 0, 0, 0, '', 0);

DELETE FROM `creature` WHERE  `guid`=4000000000057950;
DELETE FROM `creature` WHERE  `guid`=4000000000059090;
DELETE FROM `creature` WHERE  `guid`=4000000000099031;
DELETE FROM `creature` WHERE  `guid`=4000000000099063;
DELETE FROM `creature` WHERE  `guid`=4000000000134370;
DELETE FROM `creature` WHERE  `guid`=4000000000136713;
DELETE FROM `creature` WHERE  `guid`=4000000000136761;

DELETE FROM `quest_objectives` WHERE  `ID`=397621;


REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('312916', 'spell_class_mecagnomo_emergency');
REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('313010', 'spell_class_mecagnomo_emergency3');
REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('313015', 'spell_class_mecagnomo_emergency2');


UPDATE `gameobject_template` SET `type`=1,`data3`=0,`ScriptName`='' WHERE `entry`=179879;
replace into `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `event_param_string`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) values
('179879','1','0','0','64','0','100','0','0','0','0','0','0','','62','469','0','0','0','0','0','7','0','0','0','-7672.32','-1107.05','396.651','0.59','Teleport player to Blackwing Lair');

UPDATE `creature_template` SET `gossip_menu_id`=21334 WHERE `entry`=13020;

UPDATE `creature_template` SET `unit_flags`=64 WHERE `entry`=14020;

REPLACE INTO `scenarios` VALUES (1768,12,1570,1573);

UPDATE `creature_template` SET `movementid`=140 WHERE `entry` IN (60941);

DELETE FROM `disables` WHERE `entry`=312916;

UPDATE `creature_template` SET `unit_flags`=0,`unit_flags2`=0,`faction`=35 WHERE `entry`=36283;

UPDATE `quest_template` SET `QuestType`='0' WHERE  `ID`=59755;

UPDATE `quest_template` SET `QuestType`='0' WHERE  `ID`=59756;

DELETE FROM `creature` WHERE `guid`=60000000000025305;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnDifficulties`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES (60000000000025305, 166723, 2364, 0, 0, '0', 0, 0, 0, -1, 0, 0, 4752.92, 7659.9, 4772.66, 4.17087, 300, 0, 0, 682, 1500, 0, 0, 0, 0, 0, 0, '', 0);

UPDATE `creature_template` SET `npcflag`='2' WHERE  `entry`=166723;

UPDATE `gameobject_template` SET `Data1`='59760' WHERE  `entry`=351722;
UPDATE `gameobject_template` SET `ScriptName`='go_mawforged_lock' WHERE  `entry`=351722;

DELETE FROM `gameobject_template` WHERE `entry`=351761;
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `Data24`, `Data25`, `Data26`, `Data27`, `Data28`, `Data29`, `Data30`, `Data31`, `Data32`, `Data33`, `ContentTuningId`, `AIName`, `ScriptName`, `VerifiedBuild`) VALUES (351761, 10, 66897, 'Mawsteel Cage', 'questinteract', 'Unlocking', '', 1, 3221, 0, 0, 3000, 0, 0, 0, 0, 0, 0, 326649, 0, 0, 0, 24815, 0, 0, 0, 0, 0, 1, 0, 83245, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, '', '', 37474);

DELETE FROM `gameobject_template_addon` WHERE `entry`=351761;
INSERT INTO `gameobject_template_addon` (`entry`, `faction`, `flags`, `mingold`, `maxgold`, `WorldEffectID`, `AIAnimKitID`) VALUES (351761, 0, 262144, 0, 0, 0, 10311);



UPDATE `gameobject_template` SET `ScriptName`='go_mawsteel_cage' WHERE  `entry`=351761;

UPDATE `quest_template` SET `QuestType`='0' WHERE  `ID`= 59761;

UPDATE `quest_template` SET `QuestType`='0' WHERE  `ID`= 59776;

UPDATE `quest_template` SET `QuestType`='0' WHERE  `ID`= 59762;


DELETE FROM `creature` WHERE `guid`=60000000000025308;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnDifficulties`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES (60000000000025308, 167827, 2364, 0, 0, '0', 0, 0, 0, -1, 0, 0, 4522.25, 7442.47, 4793.95, 5.01864, 300, 0, 0, 1365, 60, 0, 0, 0, 0, 0, 0, '', 0);

UPDATE `creature_template` SET `npcflag`='2' WHERE  `entry`=167827;

UPDATE `creature_template` SET `npcflag`='2' WHERE  `entry`=168162;

