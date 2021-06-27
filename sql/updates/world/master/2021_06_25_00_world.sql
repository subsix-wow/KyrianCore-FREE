UPDATE creature_template SET `AIName` = 'SmartAI' WHERE `AIName` = 'SmartAi';
UPDATE creature_template SET `AIName` = 'SmartAI' WHERE `AIName` = 'ScriptedAI';
UPDATE creature_template SET `AIName` = 'SmartAI' WHERE `AIName` = 'NullAI';
UPDATE creature_template SET `AIName` = 'SmartAI' WHERE `AIName` = 'AOEAI';

UPDATE creature SET spawnDifficulties = '0' WHERE (spawnDifficulties <> '0' AND map = '0');

DELETE FROM creature WHERE id IN (543533,324235,432425,877896,500526,500527,500528);

UPDATE gameobject_template SET displayId = '62173' WHERE entry = 351469;
UPDATE gameobject_template SET displayId = '67720' WHERE entry = 349980;


