-- Battle pet tamer breadcrumbs: The Returning Champion and The Triumphant Return
-- Three of the breadcrumbs that send a player from a continent's Grand Master
-- back to Audrey Burnhep or Varzok had no quest starter, so nobody offered them.
-- Neither this DB nor the MoP 5.4.8 world DB (alexkulya) has a starter row for
-- any of these breadcrumbs; the starters below come from the MoP 5.4.8 quest
-- text on mop.warcraftdb.com:
--   31975 The Returning Champion (Alliance): Stone Cold Trixxy (66466). Its
--         quest text is word for word the text of 31977, Trixxy's Horde copy,
--         with Audrey Burnhep in place of Varzok.
--   31985 The Triumphant Return (Alliance): Obalis (66824). Its completion text
--         reads "I take it you met Obalis?".
--   31986 The Triumphant Return (Horde): Obalis (66824). Same quest text as
--         31985 with Varzok in place of Audrey Burnhep; both lead to Battle Pet
--         Tamers: Pandaria, the step after Obalis, the Cataclysm Grand Master.
-- Races: Trixxy, Lydia Accoste and Obalis each start both faction copies, and the
-- DB had no race mask on any breadcrumb, so every character was offered both.
-- The masks are alexkulya's and match the side mop.warcraftdb.com gives.
-- 31979, a fifth The Returning Champion, stays untouched: it carries
-- QUEST_FLAGS_DEPRECATED (0x4000, not generically available), has no side, no
-- quest type and no completion text on mop.warcraftdb.com, and neither this DB
-- nor alexkulya gives it a starter or an ender. It is unused.

SET @HORDE    := 33555378; -- orc, undead, tauren, troll, goblin, blood elf, pandaren (horde)
SET @ALLIANCE := 18875469; -- human, dwarf, night elf, gnome, draenei, worgen, pandaren (alliance)

DELETE FROM `creature_queststarter` WHERE `quest` IN (31975, 31985, 31986);
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(66466, 31975), -- Stone Cold Trixxy: The Returning Champion (Alliance)
(66824, 31985), -- Obalis: The Triumphant Return (Alliance)
(66824, 31986); -- Obalis: The Triumphant Return (Horde)

UPDATE `quest_template` SET `RequiredRaces` = @ALLIANCE WHERE `Id` IN (31975, 31976, 31981, 31984, 31985);
UPDATE `quest_template` SET `RequiredRaces` = @HORDE    WHERE `Id` IN (31977, 31980, 31982, 31983, 31986);
