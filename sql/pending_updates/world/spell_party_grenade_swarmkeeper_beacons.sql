-- Party G.R.E.N.A.D.E. (item 38577, dropped by Blingtron 4000 Gift Package 86623, a reward
-- of quest 31752), Swarmkeeper's Medallion (item 86526, chest 213971 on map 870) and the
-- Kirin Tor and Sunreaver Beacons (items 95567 and 95568, rewards of quests 32680 and 32681).
-- All four had no script binding: the grenade never started the dance emote, the medallion
-- gave no speed when equipped inside its zone, and the beacons cast nothing.
-- Before this update there were no spell_script_names rows for 51508, 51510, 128887,
-- 140295 and 140300.
DELETE FROM `spell_script_names` WHERE `spell_id` IN (51508, 51510, 128887, 140295, 140300);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(51508, 'spell_item_party_grenade_eff'),
(51510, 'spell_item_party_grenade'),
(128887, 'spell_item_swarmkeepers_speed'),
(140295, 'spell_gen_isle_of_thunder_beacon'),
(140300, 'spell_gen_isle_of_thunder_beacon');
