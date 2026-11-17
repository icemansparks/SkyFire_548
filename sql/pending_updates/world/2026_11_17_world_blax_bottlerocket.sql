-- Blax Bottlerocket (52809), Toys and Novelties vendor in Orgrimmar
-- Talking to him opened his greeting but never the vendor window: the menu's
-- only vendor-flagged option (12759, option 3) had option type 0 instead of
-- GOSSIP_OPTION_VENDOR, so PrepareGossipMenu dropped it as unknown. Option 0
-- carries the text but no npcflag. The MoP 5.4.8 world DB (alexkulya) has a
-- single option 0 of vendor type with npcflag 128; option 0 becomes that and the
-- duplicate option 3 goes, with its locale row.
--
-- He also stood still. He walks the Valley of Strength with his Toy Cart: the
-- 39-point loop below is TDB 434.22011 path 3708780 (Cataclysm 4.3.4 sniff). The
-- MoP 5.4.8 DB has a shorter recording of the same route whose points match
-- points 15 and 16 here within 0.3 yards. The Toy Cart (52812) and its Toy Cart
-- Bunny (52810) come as vehicle accessories (vehicle_template_accessory), so the
-- separately spawned cart and bunny would stand left behind; the 4.3.4 sniff
-- spawns only Blax.

-- Vendor option
UPDATE `gossip_menu_option` SET `OptionType`=3, `OptionNpcflag`=128 WHERE `MenuID`=12759 AND `OptionID`=0;
DELETE FROM `gossip_menu_option` WHERE `MenuID`=12759 AND `OptionID`=3;
DELETE FROM `gossip_menu_option_locale` WHERE `MenuID`=12759 AND `OptionID`=3;

-- Standalone Toy Cart and Toy Cart Bunny (the vehicle seats its own)
DELETE FROM `creature_addon` WHERE `guid` IN (162011, 162012);
DELETE FROM `creature` WHERE `guid` IN (162011, 162012) AND `id` IN (52812, 52810);

-- Waypoint patrol
UPDATE `creature` SET `position_x`=1814.53, `position_y`=-4524.29, `position_z`=23.4143, `orientation`=5.71909, `MovementType`=2, `spawndist`=0 WHERE `guid`=162013 AND `id`=52809;
DELETE FROM `creature_addon` WHERE `guid`=162013;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(162013, 1620130, 0, 0, 257, 0, NULL);

DELETE FROM `waypoint_data` WHERE `id`=1620130;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) VALUES
(1620130, 1, 1814.53, -4524.29, 23.4143, 0, 0, 0, 0, 100, 0),
(1620130, 2, 1792.4, -4527.09, 23.8666, 0, 0, 0, 0, 100, 0),
(1620130, 3, 1784.65, -4527.59, 24.3666, 0, 0, 0, 0, 100, 0),
(1620130, 4, 1775.96, -4528.34, 25.0537, 0, 0, 0, 0, 100, 0),
(1620130, 5, 1771.71, -4525.84, 25.5537, 0, 0, 0, 0, 100, 0),
(1620130, 6, 1766.96, -4522.84, 26.3037, 0, 0, 0, 0, 100, 0),
(1620130, 7, 1763.05, -4520.71, 26.7008, 0, 0, 0, 0, 100, 0),
(1620130, 8, 1757.34, -4520.24, 27.2902, 0, 0, 0, 0, 100, 0),
(1620130, 9, 1760.34, -4522.99, 26.7902, 0, 0, 0, 0, 100, 0),
(1620130, 10, 1763.84, -4525.99, 26.2902, 0, 0, 0, 0, 100, 0),
(1620130, 11, 1766.14, -4527.97, 25.8735, 0, 0, 0, 0, 100, 0),
(1620130, 12, 1769.64, -4529.72, 25.1235, 0, 0, 0, 0, 100, 0),
(1620130, 13, 1775.89, -4532.88, 24.5539, 0, 0, 0, 0, 100, 0),
(1620130, 14, 1782.64, -4533.38, 24.0539, 0, 0, 0, 0, 100, 0),
(1620130, 15, 1791.43, -4534.1, 23.451, 0, 0, 0, 0, 100, 0),
(1620130, 16, 1811.44, -4530.4, 23.6962, 0, 0, 0, 0, 100, 0),
(1620130, 17, 1834.69, -4525.65, 23.9462, 0, 0, 0, 0, 100, 0),
(1620130, 18, 1846.41, -4522.95, 24.2594, 0, 0, 0, 0, 100, 0),
(1620130, 19, 1858.34, -4524.24, 24.9354, 0, 0, 0, 0, 100, 0),
(1620130, 20, 1863.34, -4527.49, 25.4354, 0, 0, 0, 0, 100, 0),
(1620130, 21, 1868.34, -4530.74, 26.1854, 0, 0, 0, 0, 100, 0),
(1620130, 22, 1872.59, -4533.24, 26.6854, 0, 0, 0, 0, 100, 0),
(1620130, 23, 1876.59, -4535.74, 27.4354, 0, 0, 0, 0, 100, 0),
(1620130, 24, 1880.76, -4538.2, 28.1798, 0, 0, 0, 0, 100, 0),
(1620130, 25, 1890.26, -4536.7, 28.6798, 0, 0, 0, 0, 100, 0),
(1620130, 26, 1894.44, -4535.52, 28.9536, 0, 0, 0, 0, 100, 0),
(1620130, 27, 1894.69, -4531.52, 28.2036, 0, 0, 0, 0, 100, 0),
(1620130, 28, 1894.94, -4525.52, 27.7036, 0, 0, 0, 0, 100, 0),
(1620130, 29, 1895.19, -4519.77, 26.9536, 0, 0, 0, 0, 100, 0),
(1620130, 30, 1895.44, -4514.77, 26.4536, 0, 0, 0, 0, 100, 0),
(1620130, 31, 1895.69, -4510.77, 25.9536, 0, 0, 0, 0, 100, 0),
(1620130, 32, 1895.72, -4505.72, 25.0598, 0, 0, 0, 0, 100, 0),
(1620130, 33, 1892.22, -4502.22, 24.5598, 0, 0, 0, 0, 100, 0),
(1620130, 34, 1889.47, -4499.72, 24.0598, 0, 0, 0, 0, 100, 0),
(1620130, 35, 1883.43, -4493.99, 23.5952, 0, 0, 0, 0, 100, 0),
(1620130, 36, 1863.91, -4510.19, 23.8035, 0, 0, 0, 0, 100, 0),
(1620130, 37, 1851.02, -4517.42, 24.1831, 0, 0, 0, 0, 100, 0),
(1620130, 38, 1838.68, -4521.32, 24.1263, 0, 0, 0, 0, 100, 0),
(1620130, 39, 1822.93, -4523.07, 23.6263, 0, 0, 0, 0, 100, 0);
