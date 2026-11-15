-- Misha Tor'kren (3193), Tor'kren Farm, Durotar: two spawns 3 yards apart.
-- The MoP 5.4.8 world DB (alexkulya) and Cata TDB 434 both hold one spawn, at
-- 728.602 -4256.38 20.03; guid 147339 matches it. Remove the extra one.
DELETE FROM `creature_addon` WHERE `guid`=141260;
DELETE FROM `creature` WHERE `guid`=141260 AND `id`=3193;
