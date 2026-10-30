-- Fledgling Braves at Camp Narache spar with the Bristleback Invaders
-- next to them instead of standing idle; a player or pet attacking an
-- invader takes it over, and a brave never kills an invader.
-- Behaviour in npc_fledgling_brave / npc_bristleback_invader (zone_mulgore.cpp).
-- Neither SFDB, TDB 434.22011 (Cata 4.3.4) nor the alexkulya 5.4.8 world
-- DB pair them up: all three only give them combat spells (SmartAI).
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_fledgling_brave' WHERE `entry` = 36942;
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_bristleback_invader' WHERE `entry` = 36943;
