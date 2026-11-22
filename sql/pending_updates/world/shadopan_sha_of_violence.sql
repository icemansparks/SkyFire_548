-- Shado-Pan Monastery (map 959): Sha of Violence encounter
-- Script names are those of alexkulya's 5.4.8 DB, except 56766, 58201 and 213457: alexkulya's DB leaves them unbound although its code
-- has the scripts (npc_achiev_prisoned_sha, go_containment_override), so they are bound here. Idempotent: plain UPDATEs and DELETE-then-INSERT.
-- The boss (56719), its texts, the doors (210866, 210867) and all spawns are not touched here; creature_text 56719 needs no change.

UPDATE `creature_template` SET `ScriptName`='boss_sha_of_violence', `flags_extra`=1, `mechanic_immune_mask`=803979263 WHERE `entry`=56719;
UPDATE `creature_template` SET `ScriptName`='npc_achiev_prisoned_sha' WHERE `entry` IN (56766, 58201);
UPDATE `creature_template` SET `ScriptName`='npc_sv_lesser' WHERE `entry`=66652;

-- The Containment Override is a button (type 1) in alexkulya's DB; here it was a plain door with no data. The prisons keep their door type.
UPDATE `gameobject_template` SET `name`='Containment Override', `type`=1, `data1`=1690, `ScriptName`='go_containment_override' WHERE `entry`=213457;
UPDATE `gameobject_template` SET `name`='Sha Prison 1' WHERE `entry`=212062;
UPDATE `gameobject_template` SET `name`='Sha Prison 2' WHERE `entry`=212063;
UPDATE `gameobject_template` SET `name`='Sha Prison 3' WHERE `entry`=212064;

DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_sv_smoke_blades';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(106827, 'spell_sv_smoke_blades');
