-- Battle pet trainer quest chains: Learning the Ropes, On The Mend, Level Up!, Got one!
-- Twelve chains, one per battle pet trainer. Each trainer starts and ends its own
-- four quests (Wowhead MoP Classic quick facts, all 48 quests). The DB had starters
-- for only some quests, no chain links and no race mask, so any character could take
-- every chain, Alliance could take Horde chains, and Got one! was open before
-- Learning the Ropes.
-- Chain order and links (PrevQuestId, NextQuestIdChain) from the MoP 5.4.8 world DB
-- (alexkulya); mop.warcraftdb.com lists the same order.
-- Races: a start-zone trainer serves the races that start in its zone, a capital
-- trainer (Varzok, Matty, Audrey Burnhep) any race of its faction. Goblins and
-- pandaren start at the capital trainers. The faction masks are alexkulya's.
-- The quests carry QUEST_FLAGS2_ACCOUNT_LEVEL_QUEST, which this core does not
-- enforce yet; that is a separate core change.

SET @HORDE    := 33555378; -- orc, undead, tauren, troll, goblin, blood elf, pandaren (horde)
SET @ALLIANCE := 18875469; -- human, dwarf, night elf, gnome, draenei, worgen, pandaren (alliance)

DROP TEMPORARY TABLE IF EXISTS `tmp_pet_trainer_chain`;
CREATE TEMPORARY TABLE `tmp_pet_trainer_chain` (
  `npc` INT UNSIGNED NOT NULL,
  `races` INT UNSIGNED NOT NULL,
  `q1` INT UNSIGNED NOT NULL, -- Learning the Ropes
  `q2` INT UNSIGNED NOT NULL, -- On The Mend
  `q3` INT UNSIGNED NOT NULL, -- Level Up!
  `q4` INT UNSIGNED NOT NULL  -- Got one!
);
INSERT INTO `tmp_pet_trainer_chain` VALUES
-- Horde
(63061, 2|128,     31571, 31572, 31830, 31570), -- Narzak, Razor Hill: orc, troll
(63067, 32,        31573, 31574, 31831, 31575), -- Naleen, Bloodhoof Village: tauren
(63073, 16,        31576, 31577, 31823, 31578), -- Ansel Fincap, Brill: undead
(63080, 512,       31579, 31580, 31824, 31581), -- Jarson Everlong, Falconwing Square: blood elf
(63086, @HORDE,    31585, 31586, 31828, 31587), -- Matty, Orgrimmar
(63626, @HORDE,    31588, 31589, 31827, 31590), -- Varzok, Orgrimmar
-- Alliance
(63014, 1,         31308, 31309, 31785, 31550), -- Marcus Jensen, Goldshire: human
(63075, 4|64,      31548, 31549, 31822, 31551), -- Grady Bannson, Kharanos: dwarf, gnome
(63070, 8,         31552, 31553, 31826, 31555), -- Valeena, Dolanaar: night elf
(63077, 1024,      31556, 31568, 31825, 31569), -- Lehna, Azure Watch: draenei
(63083, 2097152,   31582, 31583, 31832, 31584), -- Will Larsons, Lor'danel: worgen
(63596, @ALLIANCE, 31591, 31592, 31821, 31593); -- Audrey Burnhep, Stormwind

DROP TEMPORARY TABLE IF EXISTS `tmp_pet_trainer_quest`;
CREATE TEMPORARY TABLE `tmp_pet_trainer_quest` (
  `quest` INT UNSIGNED NOT NULL PRIMARY KEY,
  `npc` INT UNSIGNED NOT NULL,
  `races` INT UNSIGNED NOT NULL,
  `prev` INT UNSIGNED NOT NULL,
  `next` INT UNSIGNED NOT NULL
);
INSERT INTO `tmp_pet_trainer_quest` SELECT `q1`, `npc`, `races`, 0,    `q2` FROM `tmp_pet_trainer_chain`;
INSERT INTO `tmp_pet_trainer_quest` SELECT `q2`, `npc`, `races`, `q1`, `q3` FROM `tmp_pet_trainer_chain`;
INSERT INTO `tmp_pet_trainer_quest` SELECT `q3`, `npc`, `races`, `q2`, `q4` FROM `tmp_pet_trainer_chain`;
INSERT INTO `tmp_pet_trainer_quest` SELECT `q4`, `npc`, `races`, `q3`, 0    FROM `tmp_pet_trainer_chain`;

UPDATE `quest_template` `q` JOIN `tmp_pet_trainer_quest` `t` ON `t`.`quest` = `q`.`Id`
SET `q`.`RequiredRaces` = `t`.`races`, `q`.`PrevQuestId` = `t`.`prev`, `q`.`NextQuestIdChain` = `t`.`next`;

DELETE `s` FROM `creature_queststarter` `s` JOIN `tmp_pet_trainer_quest` `t` ON `t`.`quest` = `s`.`quest`;
INSERT INTO `creature_queststarter` (`id`, `quest`) SELECT `npc`, `quest` FROM `tmp_pet_trainer_quest`;

DELETE `e` FROM `creature_questender` `e` JOIN `tmp_pet_trainer_quest` `t` ON `t`.`quest` = `e`.`quest`;
INSERT INTO `creature_questender` (`id`, `quest`) SELECT `npc`, `quest` FROM `tmp_pet_trainer_quest`;

DROP TEMPORARY TABLE `tmp_pet_trainer_quest`;
DROP TEMPORARY TABLE `tmp_pet_trainer_chain`;
