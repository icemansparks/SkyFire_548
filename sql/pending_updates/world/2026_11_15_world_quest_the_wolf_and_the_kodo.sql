-- Quest 25205 "The Wolf and The Kodo": Shin Stonepillar (39380) showed no dialog,
-- so "Can you tell me your fable, Shin?" and the objective credit (39365) were
-- out of reach. His template had no gossip menu, gossip flag or AI, and the
-- option of menu 11209 had OptionType 0 and OptionNpcflag 0, so the core hid it
-- even with the menu set.
-- Menu 11209, gossip flag and the two script rows (credit on the option, then
-- close the menu) from the 5.4.8 data (alexkulya pandaria_5.4.8); the 4.3.4 data
-- (TDB 434.22011) has the same menu and npcflag 3. The option gets type 1 and
-- npcflag 1 like the other menus upstream repaired, matching the 5.4.8 row.
-- The scripted story behind the option (Summon The Wolf, Dash, Hunting, the
-- quest-invisible Kodos) has no script in any source, so the credit is given
-- on the option, as in the 5.4.8 data.

UPDATE `creature_template` SET `gossip_menu_id` = 11209, `npcflag` = `npcflag` | 1, `AIName` = 'SmartAI' WHERE `entry` = 39380 AND `ScriptName` = '';

UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 11209 AND `OptionID` = 0;

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 39380;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(39380, 0, 0, 1, 62, 0, 100, 0, 11209, 0, 0, 0, 0, 33, 39365, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Shin Stonepillar - On Gossip Option 0 Selected - Quest Credit The Wolf and The Kodo'),
(39380, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Shin Stonepillar - Linked To Id 0 - Close Gossip');
