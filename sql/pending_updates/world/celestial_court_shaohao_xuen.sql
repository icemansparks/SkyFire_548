-- Timeless Isle Celestial Court: binds Emperor Shaohao (73303), who calls the celestials one
-- after the other, and Xuen (71953), the first celestial script. Follows alexkulya/pandaria_5.4.8
-- zone_timeless_isle.cpp and boss_xuen.cpp. Health_mod and unit_flags as in that world DB: Xuen had
-- Health_mod 1 and the immune-to-players flag (256), so he could not be fought.
UPDATE `creature_template` SET `ScriptName` = 'npc_emperor_shaohao' WHERE `entry` = 73303;
UPDATE `creature_template` SET `ScriptName` = 'boss_xuen', `Health_mod` = 206.25, `unit_flags` = 0 WHERE `entry` = 71953;
