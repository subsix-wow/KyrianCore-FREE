/*
this will fix shadowlands mining nodes with the correct loot chance
-- as well as add the loot to the nodes --
-- you may contact me with any questions on discord or email --
-- krillanna#5944           rswhiteclear@outllok.com    --
*/

DELETE FROM `gameobject_loot_template` WHERE `Entry`=349900;
DELETE FROM `gameobject_loot_template` WHERE `Entry`=350082;
DELETE FROM `gameobject_loot_template` WHERE `Entry`=350085;
DELETE FROM `gameobject_loot_template` WHERE `Entry`=349981;
DELETE FROM `gameobject_loot_template` WHERE `Entry`=349982;
DELETE FROM `gameobject_loot_template` WHERE `Entry`=355508;
DELETE FROM `gameobject_loot_template` WHERE `Entry`=350087;
DELETE FROM `gameobject_loot_template` WHERE `Entry`=355507;
DELETE FROM `gameobject_loot_template` WHERE `Entry`=350084;
DELETE FROM `gameobject_loot_template` WHERE `Entry`=349983;
DELETE FROM `gameobject_loot_template` WHERE `Entry`=350086;
DELETE FROM `gameobject_loot_template` WHERE `Entry`=349980;
DELETE FROM `gameobject_loot_template` WHERE `Entry`=349899;
DELETE FROM `gameobject_loot_template` WHERE `Entry`=349898;
DELETE FROM `gameobject_loot_template` WHERE `Entry`=336686;
DELETE FROM `gameobject_loot_template` WHERE `Entry`=351469;
DELETE FROM `gameobject_loot_template` WHERE `Entry`=351470;
DELETE FROM `gameobject_loot_template` WHERE `Entry`=351471;
DELETE FROM `gameobject_loot_template` WHERE `Entry`=336689;
DELETE FROM `gameobject_loot_template` WHERE `Entry`=336690;
DELETE FROM `gameobject_loot_template` WHERE `Entry`=336688;
DELETE FROM `gameobject_loot_template` WHERE `Entry`=336433;
DELETE FROM `gameobject_loot_template` WHERE `Entry`=336601;
DELETE FROM `gameobject_loot_template` WHERE `Entry`=356537;

INSERT INTO `gameobject_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`)
VALUES
(349900, 171933, 0, 100, 0, 1, 0, 1, 3, 'zzz ores and herb fix'),
(349900, 171841, 0, 47, 0, 1, 0, 1, 3, 'zzz ores and herb fix'),
(350082, 171833, 0, 100, 0, 1, 0, 1, 3, 'zzz ores and herb fix'),
(350082, 171841, 0, 71, 0, 1, 0, 1, 3, 'zzz ores and herb fix'),
(350085, 171830, 0, 100, 0, 1, 0, 1, 7, 'zzz ores and herb fix'),
(350085, 171840, 0, 34, 0, 1, 0, 1, 3, 'zzz ores and herb fix'),
(350085, 171841, 0, 18, 0, 1, 0, 1, 3, 'zzz ores and herb fix'),
(350085, 171833, 0, 9, 0, 1, 0, 1, 3, 'zzz ores and herb fix'),
(349981, 171830, 0, 100, 0, 1, 0, 1, 7, 'zzz ores and herb fix'),
(349981, 171840, 0, 34, 0, 1, 0, 1, 3, 'zzz ores and herb fix'),
(349981, 171841, 0, 10, 0, 1, 0, 1, 3, 'zzz ores and herb fix'),
(349981, 171833, 0, 5, 0, 1, 0, 1, 3, 'zzz ores and herb fix'),
(349982, 171831, 0, 100, 0, 1, 0, 1, 3, 'zzz ores and herb fix'),
(349982, 177061, 0, 21, 0, 1, 0, 1, 3, 'zzz ores and herb fix'),
(349982, 171833, 0, 5, 0, 1, 0, 1, 3, 'zzz ores and herb fix'),
(355508, 171831, 0, 100, 0, 1, 0, 1, 7, 'zzz ores and herb fix'),
(355508, 171061, 0, 21, 0, 1, 0, 1, 3, 'zzz ores and herb fix'),
(355508, 171833, 0, 5, 0, 1, 0, 1, 3, 'zzz ores and herb fix'),
(350087, 171831, 0, 100, 0, 1, 0, 1, 7, 'zzz ores and herb fix'),
(350087, 177061, 0, 36, 0, 1, 0, 1, 3, 'zzz ores and herb fix'),
(350087, 171833, 0, 10, 0, 1, 0, 1, 3, 'zzz ores and herb fix'),
(355507, 171831, 0, 100, 0, 1, 0, 1, 7, 'zzz ores and herb fix'),
(355507, 177061, 0, 38, 0, 1, 0, 1, 3, 'zzz ores and herb fix'),
(355507, 171833, 0, 9, 0, 1, 0, 1, 3, 'zzz ores and herb fix'),
(350084, 171832, 0, 100, 0, 1, 0, 1, 7, 'zzz ores and herb fix'),
(350084, 171840, 0, 55, 0, 1, 0, 1, 3, 'zzz ores and herb fix'),
(350084, 171841, 0, 19, 0, 1, 0, 1, 3, 'zzz ores and herb fix'),
(350084, 171833, 0, 9, 0, 1, 0, 1, 3, 'zzz ores and herb fix'),
(349983, 171832, 0, 100, 0, 1, 0, 1, 7, 'zzz ores and herb fix'),
(349983, 171840, 0, 34, 0, 1, 0, 1, 3, 'zzz ores and herb fix'),
(349983, 171841, 0, 10, 0, 1, 0, 1, 3, 'zzz ores and herb fix'),
(349983, 171833, 0, 5, 0, 1, 0, 1, 3, 'zzz ores and herb fix'),
(350086, 171829, 0, 100, 0, 1, 0, 1, 7, 'zzz ores and herb fix'),
(350086, 171840, 0, 54, 0, 1, 0, 1, 3, 'zzz ores and herb fix'),
(350086, 171841, 0, 18, 0, 1, 0, 1, 3, 'zzz ores and herb fix'),
(350086, 171833, 0, 9, 0, 1, 0, 1, 3, 'zzz ores and herb fix'),
(349980, 171829, 0, 100, 0, 1, 0, 1, 7, 'zzz ores and herb fix'),
(349980, 171840, 0, 34, 0, 1, 0, 1, 3, 'zzz ores and herb fix'),
(349980, 171841, 0, 10, 0, 1, 0, 1, 3, 'zzz ores and herb fix'),
(349980, 171833, 0, 5, 0, 1, 0, 1, 3, 'zzz ores and herb fix'),
(349899, 171828, 0, 100, 0, 1, 0, 1, 7, 'zzz ores and herb fix'),
(349899, 171840, 0, 55, 0, 1, 0, 1, 3, 'zzz ores and herb fix'),
(349899, 171833, 0, 9, 0, 1, 0, 1, 3, 'zzz ores and herb fix'),
(349898, 171828, 0, 100, 0, 1, 0, 1, 7, 'zzz ores and herb fix'),
(349898, 171840, 0, 34, 0, 1, 0, 1, 3, 'zzz ores and herb fix'),
(349898, 171833, 0, 5, 0, 1, 0, 1, 3, 'zzz ores and herb fix'),
(336686, 169701, 0, 100, 0, 1, 0, 1, 5, 'zzz ores and herb fix'),
(351469, 169701, 0, 100, 0, 1, 0, 1, 5, 'zzz ores and herb fix'),
(351470, 169701, 0, 100, 0, 1, 0, 1, 5, 'zzz ores and herb fix'),
(351471, 169701, 0, 100, 0, 1, 0, 1, 5, 'zzz ores and herb fix'),
(336689, 168589, 0, 100, 0, 1, 0, 1, 5, 'zzz ores and herb fix'),
(336690, 168586, 0, 100, 0, 1, 0, 1, 12, 'zzz ores and herb fix'),
(336688, 170554, 0, 100, 0, 1, 0, 1, 5, 'zzz ores and herb fix'),
(336433, 168583, 0, 100, 0, 1, 0, 1, 5, 'zzz ores and herb fix'),
(336601, 171315, 0, 100, 0, 1, 0, 1, 3, 'zzz ores and herb fix'),
(336601, 169697, 0, 0.3, 0, 1, 0, 1, 1, 'zzz ores and herb fix'),
(356537, 171315, 0, 100, 0, 1, 0, 1, 3, 'zzz ores and herb fix');

-- this is to fix the gameobject template so the loot tables are right--

/* This seems to be removed from Game - Subsix

DELETE FROM `gameobject_template` WHERE `entry`=356400;
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `Data24`, `Data25`, `Data26`, `Data27`, `Data28`, `Data29`, `Data30`, `Data31`, `Data32`, `Data33`, `ContentTuningId`, `AIName`, `ScriptName`, `VerifiedBuild`) 
VALUES (356400, 50, 64910, 'Ligneous Phaedrum Deposit', '', '', '', 1.5, 3209, 356400, 1, 0, 1, 1, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 807, '', '', 37474);
*/

DELETE FROM `gameobject_template` WHERE `entry`=355508;
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `Data24`, `Data25`, `Data26`, `Data27`, `Data28`, `Data29`, `Data30`, `Data31`, `Data32`, `Data33`, `ContentTuningId`, `AIName`, `ScriptName`, `VerifiedBuild`) 
VALUES (355508, 50, 65888, 'Phaedrum Deposit', '', '', '', 1, 3209, 355508, 0, 0, 120, 175, 30, 0, 0, 0, 0, 0, 292, 1, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 807, '', '', 37474);

DELETE FROM `gameobject_template` WHERE `entry`=355507;
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `Data24`, `Data25`, `Data26`, `Data27`, `Data28`, `Data29`, `Data30`, `Data31`, `Data32`, `Data33`, `ContentTuningId`, `AIName`, `ScriptName`, `VerifiedBuild`) 
VALUES (355507, 50, 65887, 'Rich Phaedrum Deposit', '', '', '', 1, 3209, 355507, 0, 0, 135, 175, 30, 0, 0, 0, 0, 0, 292, 1, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 807, '', '', 37474);

DELETE FROM `gameobject_template` WHERE `entry`=350087;
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `Data24`, `Data25`, `Data26`, `Data27`, `Data28`, `Data29`, `Data30`, `Data31`, `Data32`, `Data33`, `ContentTuningId`, `AIName`, `ScriptName`, `VerifiedBuild`) 
VALUES (350087, 50, 65889, 'Rich Phaedrum Deposit', '', '', '', 1, 3209, 350087, 0, 0, 135, 175, 30, 0, 0, 0, 0, 0, 292, 1, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 807, '', '', 38556);

DELETE FROM `gameobject_template` WHERE `entry`=350086;
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `Data24`, `Data25`, `Data26`, `Data27`, `Data28`, `Data29`, `Data30`, `Data31`, `Data32`, `Data33`, `ContentTuningId`, `AIName`, `ScriptName`, `VerifiedBuild`) 
VALUES (350086, 50, 67721, 'Rich Solenium Deposit', '', '', '', 1, 3209, 350086, 0, 0, 135, 175, 30, 0, 0, 0, 0, 0, 292, 1, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 807, '', '', 38556);

DELETE FROM `gameobject_template` WHERE `entry`=350085;
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `Data24`, `Data25`, `Data26`, `Data27`, `Data28`, `Data29`, `Data30`, `Data31`, `Data32`, `Data33`, `ContentTuningId`, `AIName`, `ScriptName`, `VerifiedBuild`) 
VALUES (350085, 50, 65774, 'Rich Oxxein Deposit', '', '', '', 1, 3209, 350085, 0, 0, 135, 175, 30, 0, 0, 0, 0, 0, 292, 1, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 807, '', '', 38556);

DELETE FROM `gameobject_template` WHERE `entry`=350084;
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `Data24`, `Data25`, `Data26`, `Data27`, `Data28`, `Data29`, `Data30`, `Data31`, `Data32`, `Data33`, `ContentTuningId`, `AIName`, `ScriptName`, `VerifiedBuild`) 
VALUES (350084, 50, 65772, 'Rich Sinvyr Deposit', '', '', '', 1, 3209, 350084, 0, 0, 135, 175, 30, 0, 0, 0, 0, 0, 292, 1, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 807, '', '', 38556);

DELETE FROM `gameobject_template` WHERE `entry`=350082;
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `Data24`, `Data25`, `Data26`, `Data27`, `Data28`, `Data29`, `Data30`, `Data31`, `Data32`, `Data33`, `ContentTuningId`, `AIName`, `ScriptName`, `VerifiedBuild`) 
VALUES (350082, 50, 66084, 'Rich Elethium Deposit', '', '', '', 1, 3209, 350082, 0, 0, 150, 175, 30, 0, 0, 0, 0, 0, 292, 1, 348183, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 807, '', '', 38556);

DELETE FROM `gameobject_template` WHERE `entry`=349983;
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `Data24`, `Data25`, `Data26`, `Data27`, `Data28`, `Data29`, `Data30`, `Data31`, `Data32`, `Data33`, `ContentTuningId`, `AIName`, `ScriptName`, `VerifiedBuild`) 
VALUES (349983, 50, 65773, 'Sinvyr Deposit', '', '', '', 1, 3209, 349983, 0, 0, 120, 175, 30, 0, 0, 0, 0, 0, 292, 1, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 807, '', '', 38556);

DELETE FROM `gameobject_template` WHERE `entry`=349982;
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `Data24`, `Data25`, `Data26`, `Data27`, `Data28`, `Data29`, `Data30`, `Data31`, `Data32`, `Data33`, `ContentTuningId`, `AIName`, `ScriptName`, `VerifiedBuild`) 
VALUES (349982, 50, 64910, 'Phaedrum Deposit', '', '', '', 1, 3209, 349982, 0, 0, 120, 175, 30, 0, 0, 0, 0, 0, 292, 1, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 807, '', '', 38556);

DELETE FROM `gameobject_template` WHERE `entry`=349981;
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `Data24`, `Data25`, `Data26`, `Data27`, `Data28`, `Data29`, `Data30`, `Data31`, `Data32`, `Data33`, `ContentTuningId`, `AIName`, `ScriptName`, `VerifiedBuild`) 
VALUES (349981, 50, 65775, 'Oxxein Deposit', '', '', '', 1, 3209, 349981, 0, 0, 120, 175, 30, 0, 0, 0, 0, 0, 292, 1, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 807, '', '', 38556);

DELETE FROM `gameobject_template` WHERE `entry`=349980;
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `Data24`, `Data25`, `Data26`, `Data27`, `Data28`, `Data29`, `Data30`, `Data31`, `Data32`, `Data33`, `ContentTuningId`, `AIName`, `ScriptName`, `VerifiedBuild`) 
VALUES (349980, 50, 67720, 'Solenium Deposit', '', '', '', 1, 3209, 349980, 0, 0, 120, 175, 30, 0, 0, 0, 0, 0, 292, 1, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 807, '', '', 38556);

DELETE FROM `gameobject_template` WHERE `entry`=349900;
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `Data24`, `Data25`, `Data26`, `Data27`, `Data28`, `Data29`, `Data30`, `Data31`, `Data32`, `Data33`, `ContentTuningId`, `AIName`, `ScriptName`, `VerifiedBuild`) 
VALUES (349900, 50, 66085, 'Elethium Deposit', '', '', '', 1, 3209, 349900, 0, 0, 150, 175, 30, 0, 0, 0, 0, 0, 292, 1, 348182, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 807, '', '', 38556);

DELETE FROM `gameobject_template` WHERE `entry`=349899;
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `Data24`, `Data25`, `Data26`, `Data27`, `Data28`, `Data29`, `Data30`, `Data31`, `Data32`, `Data33`, `ContentTuningId`, `AIName`, `ScriptName`, `VerifiedBuild`) 
VALUES (349899, 50, 64846, 'Rich Laestrite Deposit', '', '', '', 1, 3209, 349899, 0, 0, 120, 175, 30, 0, 0, 0, 0, 0, 292, 1, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 807, '', '', 38556);

DELETE FROM `gameobject_template` WHERE `entry`=349898;
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `Data24`, `Data25`, `Data26`, `Data27`, `Data28`, `Data29`, `Data30`, `Data31`, `Data32`, `Data33`, `ContentTuningId`, `AIName`, `ScriptName`, `VerifiedBuild`) 
VALUES (349898, 50, 64909, 'Laestrite Deposit', '', '', '', 1, 3209, 349898, 0, 0, 100, 175, 30, 0, 0, 0, 0, 0, 292, 1, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 807, '', '', 38556);
