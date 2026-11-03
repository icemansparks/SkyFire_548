-- Quest 29548 "The Mission" (Alliance start to Pandaria): Sky Admiral Rogers
-- (66292) on The Skyfire over Stormwind had no gossip, so players could not
-- talk to him. His SFDB smart script already waits for option 0 of menu 14971
-- (quest credit 131057 "The Mission Scene - JF", then teleport 130321 "The
-- Mission: Teleport Player"), and spell_target_position for 130321 exists,
-- but SFDB has no gossip_menu, gossip_menu_option or condition row for 14971.
-- Text, option and condition from the alexkulya 5.4.8 world DB (2026_08_16),
-- where the same rows sit on its own menu id 51000; moved to SFDB's 14971.
-- npc_text 66292 is alexkulya's id, free in SFDB. The text goes into both
-- text0_0 and text0_1: the NPC text query sends only text0_0. Cross-checked against the
-- 5.4.8 DBC: 131057 gives kill credit 66292 (the quest objective) and phase
-- 1740, 130321 teleports to map 870. The Cata 4.3.4 data has no Skyfire rows.
-- alexkulya's second option ("You really have it in for the Horde, don't
-- you?") leads nowhere there either and is left out.

-- SFDB update 2020_02_22_04 added this option with the old column names; the
-- current SFDB release (26.002) no longer has it.
-- Values before this update, queried on TEST mopworld (SFDB 26.002) on
-- 2026-11-03: no npc_text 66292, no gossip_menu, gossip_menu_option,
-- gossip_menu_option_locale or conditions (type 15) rows for menu 14971.
DELETE FROM `npc_text` WHERE `ID` = 66292;
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`, `lang0`, `prob0`, `em0_0`, `em0_1`, `em0_2`, `WDBVerified`) VALUES
(66292, 'The Skyfire has had a complete overhaul since our encounter with Deathwing! We now carry a full complement of both fixed-wing and gyro aircraft as well as a battery of 18-inch smoothbore shore bombardment cannons.$b$bThe Horde won\'t be able to hide from her reach. Not on land, not at sea, not in the air.', 'The Skyfire has had a complete overhaul since our encounter with Deathwing! We now carry a full complement of both fixed-wing and gyro aircraft as well as a battery of 18-inch smoothbore shore bombardment cannons.$b$bThe Horde won\'t be able to hide from her reach. Not on land, not at sea, not in the air.', 0, 100, 0, 1, 0, 18019);

DELETE FROM `locales_npc_text` WHERE `ID` = 66292;
INSERT INTO `locales_npc_text` (`ID`, `Text0_0_loc1`, `Text0_0_loc2`, `Text0_0_loc3`, `Text0_0_loc4`, `Text0_0_loc5`, `Text0_0_loc6`, `Text0_0_loc7`, `Text0_0_loc8`, `Text0_1_loc1`, `Text0_1_loc2`, `Text0_1_loc3`, `Text0_1_loc4`, `Text0_1_loc5`, `Text0_1_loc6`, `Text0_1_loc7`, `Text0_1_loc8`) VALUES
(66292, '하늘불꽃호가 데스윙과의 전투 이후 전면 개조를 거쳐 새롭게 태어났네! 이제 고정익 비행기, 자이로 비행기 편대와 더불어 구경 45센티미터의 해안 활강포를 여러 문 갖춘 완벽한 공격함으로 거듭났지.$b$b우리 사거리 내에서 호드가 숨을 수 있는 곳은 없네. 땅에서도, 바다에서도, 공중에서도 적을 전멸시킬 수 있으니까.', 'Le Brûleciel a été complètement remis à neuf depuis notre combat avec Aile de Mort ! Nous transportons désormais tout un bataillon d’engins volants, aussi bien à ailes fixes qu’à hélices, ainsi qu’une batterie de canons de bombardement à canon lisse de près de cinquante centimètres de long.$b$bLa Horde ne pourra pas nous échapper. Ni sur les terres, ni sur les mers, ni dans les airs !', 'Die Himmelsfeuer wurde nach der Begegnung mit Todesschwinge komplett überholt! Wir verfügen jetzt über ein vollständiges Kontingent an Starrflügel- und Gyro-Flugzeugen so wie eine Batterie an 18-Zoll-Glattlaufschusswaffen-Kanonen.$b$bVor ihnen wird sich die Horde nicht mehr verstecken können. Weder an Land, noch zur See, noch in der Luft.', '在遭遇死亡之翼后，我们已经对天火号进行了全面改装！我们现在不但有了舰载固定翼战机和旋翼机，还给它装上了18英寸口径的滑膛式海岸轰击炮组。$b$b密集的炮火一定会让部落无处遁形，随便他们上天入地，翻山过海，都会被我们给炸出来。', '在我們那場與死亡之翼的戰鬥之後，天火號進行過一場完整的大翻修!我們現在載有完整的固定機翼與旋翼戰機，及一組18吋的滑膛岸轟大砲。$B$B部落將沒有辦法逃離她的攻擊範圍。在地上不行，海上不行，在空中也不行。', '¡Hemos renovado el Abrasacielos por completo desde nuestro encuentro con Alamuerte! Ahora llevamos un destacamento completo de aviones y girocópteros, así como una batería de cañones aire-tierra de ánima lisa de dieciocho pulgadas.$b$bLa Horda no puede huir de esta nave. Ni en la tierra, ni en el mar, ni en el aire.', '¡Hemos renovado el Abrasacielos por completo desde nuestro encuentro con Alamuerte! Ahora llevamos un destacamento completo de aviones y girocópteros, así como una batería de cañones aire-tierra de ánima lisa de dieciocho pulgadas.$b$bLa Horda no puede huir de esta nave. Ni en la tierra, ni en el mar, ni en el aire.', 'После сражения со Смертокрылом "Небесный огонь" был полностью переоснащен! Теперь он укомплектован воздушными аппаратами с фиксированными крыльями и гирокоптерами, а также 50-миллиметровыми пушками гладкоствольными пушками для берегового обстрела.$b$bОрде не укрыться от нашего гнева. Ни на земле, ни на воде, ни в воздухе.', '하늘불꽃호가 데스윙과의 전투 이후 전면 개조를 거쳐 새롭게 태어났네! 이제 고정익 비행기, 자이로 비행기 편대와 더불어 구경 45센티미터의 해안 활강포를 여러 문 갖춘 완벽한 공격함으로 거듭났지.$b$b우리 사거리 내에서 호드가 숨을 수 있는 곳은 없네. 땅에서도, 바다에서도, 공중에서도 적을 전멸시킬 수 있으니까.', 'Le Brûleciel a été complètement remis à neuf depuis notre combat avec Aile de Mort ! Nous transportons désormais tout un bataillon d’engins volants, aussi bien à ailes fixes qu’à hélices, ainsi qu’une batterie de canons de bombardement à canon lisse de près de cinquante centimètres de long.$b$bLa Horde ne pourra pas nous échapper. Ni sur les terres, ni sur les mers, ni dans les airs !', 'Die Himmelsfeuer wurde nach der Begegnung mit Todesschwinge komplett überholt! Wir verfügen jetzt über ein vollständiges Kontingent an Starrflügel- und Gyro-Flugzeugen so wie eine Batterie an 18-Zoll-Glattlaufschusswaffen-Kanonen.$b$bVor ihnen wird sich die Horde nicht mehr verstecken können. Weder an Land, noch zur See, noch in der Luft.', '在遭遇死亡之翼后，我们已经对天火号进行了全面改装！我们现在不但有了舰载固定翼战机和旋翼机，还给它装上了18英寸口径的滑膛式海岸轰击炮组。$b$b密集的炮火一定会让部落无处遁形，随便他们上天入地，翻山过海，都会被我们给炸出来。', '在我們那場與死亡之翼的戰鬥之後，天火號進行過一場完整的大翻修!我們現在載有完整的固定機翼與旋翼戰機，及一組18吋的滑膛岸轟大砲。$B$B部落將沒有辦法逃離她的攻擊範圍。在地上不行，海上不行，在空中也不行。', '¡Hemos renovado el Abrasacielos por completo desde nuestro encuentro con Alamuerte! Ahora llevamos un destacamento completo de aviones y girocópteros, así como una batería de cañones aire-tierra de ánima lisa de dieciocho pulgadas.$b$bLa Horda no puede huir de esta nave. Ni en la tierra, ni en el mar, ni en el aire.', '¡Hemos renovado el Abrasacielos por completo desde nuestro encuentro con Alamuerte! Ahora llevamos un destacamento completo de aviones y girocópteros, así como una batería de cañones aire-tierra de ánima lisa de dieciocho pulgadas.$b$bLa Horda no puede huir de esta nave. Ni en la tierra, ni en el mar, ni en el aire.', 'После сражения со Смертокрылом "Небесный огонь" был полностью переоснащен! Теперь он укомплектован воздушными аппаратами с фиксированными крыльями и гирокоптерами, а также 50-миллиметровыми пушками гладкоствольными пушками для берегового обстрела.$b$bОрде не укрыться от нашего гнева. Ни на земле, ни на воде, ни в воздухе.');

DELETE FROM `gossip_menu` WHERE `MenuID` = 14971;
INSERT INTO `gossip_menu` (`MenuID`, `TextID`) VALUES
(14971, 66292);

DELETE FROM `gossip_menu_option` WHERE `MenuID` = 14971;
INSERT INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcflag`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextID`) VALUES
(14971, 0, 0, 'I am ready to depart.', 0, 1, 1, 0, 0, 0, 0, '', 0);

DELETE FROM `gossip_menu_option_locale` WHERE `MenuID` = 14971;
INSERT INTO `gossip_menu_option_locale` (`MenuID`, `OptionID`, `Locale`, `OptionText`, `BoxText`) VALUES
(14971, 0, 'koKR', '출발할 준비가 됐습니다.', ''),
(14971, 0, 'frFR', 'Je suis prêt à partir.', ''),
(14971, 0, 'deDE', 'Ich bin bereit zum Aufbruch.', ''),
(14971, 0, 'zhCN', '我准备好出发了。', ''),
(14971, 0, 'zhTW', '我準備好要離開了。', ''),
(14971, 0, 'esES', 'Estoy listo para partir.', ''),
(14971, 0, 'esMX', 'Estoy listo para partir.', ''),
(14971, 0, 'ruRU', 'Можно отправляться.', '');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 14971;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 14971, 0, 0, 0, 9, 0, 29548, 0, 0, 0, 0, 0, '', 'Sky Admiral Rogers - Show gossip option if quest The Mission is taken');
