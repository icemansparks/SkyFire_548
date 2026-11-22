-- Timeless Isle rare chests: bind go_timeless_chest (zone_timeless_isle.cpp, ported from
-- alexkulya/pandaria_5.4.8 zone_timeless_isle.cpp). The script limits each chest to one pickup per
-- character through its tracking quest. The quest's own Weekly flag decides the reset: six chests
-- are weekly, the other 43 are one-time. A repeat attempt is refused before the chest opens, the
-- first one rewards the quest.
-- ScriptName was empty for all 49 templates. The Crane Platform and Crane Cloud (220907, 220906)
-- stay unscripted: they share the Mist-Covered chest tracking quest and would use up its reward
-- before the chest is reached.
UPDATE `gameobject_template` SET `ScriptName` = 'go_timeless_chest' WHERE `entry` IN (
    220832, 220901, 220902, 220908, 220986, 221036,
    221617, 221671, 223116, 223117, 223118, 221672, 223119, 221673,
    221670, 223084, 223085, 223086, 223087, 223088, 223089, 223090,
    223091, 223092, 223093, 223094, 223095, 223096, 223097, 223098,
    223099, 223100, 223101, 223102, 223103, 223104, 223105, 223106,
    223107, 223108, 223109, 223110, 223111, 223112, 223113, 223114,
    223115, 223204, 220807
);
