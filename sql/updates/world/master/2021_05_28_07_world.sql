INSERT INTO `creature_template` (entry, name, subname, minlevel, maxlevel, faction, family, type) VALUES (177193, 'Kiku', 'Keeper of Histories', '50', '50', '14', '0', '7') ON DUPLICATE KEY UPDATE name = VALUES(name), subname = VALUES(subname), minlevel = VALUES(minlevel), maxlevel = VALUES(maxlevel), faction = VALUES(faction), family = VALUES(family), type = VALUES(type);
INSERT INTO `creature_model_info` (DisplayID, BoundingRadius, CombatReach, DisplayID_Other_Gender) VALUES (100727, '1', '1', '0') ON DUPLICATE KEY UPDATE BoundingRadius = VALUES(BoundingRadius), CombatReach = VALUES(CombatReach), DisplayID_Other_Gender = VALUES(DisplayID_Other_Gender);
INSERT INTO `creature_template_model` (CreatureID, Idx, CreatureDisplayID, DisplayScale, Probability) VALUES (174170, '0', '100727', '1', '1') ON DUPLICATE KEY UPDATE Idx = VALUES(Idx), CreatureDisplayID = VALUES(CreatureDisplayID), DisplayScale = VALUES(DisplayScale), Probability = VALUES(Probability);

DELETE FROM creature_addon   WHERE guid NOT IN (SELECT guid FROM creature);
DELETE FROM gameobject_addon WHERE guid NOT IN (SELECT guid FROM gameobject);

DELETE FROM creature_loot_template WHERE item IN (11098, 158078, 28558, 60405, 60406, 3393, 11098, 56054, 56055, 62770, 4882, 62770, 22541, 22548, 37331, 37330, 9293, 3393, 9299, 13140, 62528, 13140, 62370);

