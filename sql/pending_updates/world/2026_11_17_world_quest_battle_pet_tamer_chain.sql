-- Battle Pet Tamers: Kalimdor (31889 Alliance, 31891 Horde) and
-- Battle Pet Tamers: Eastern Kingdoms (31902 Alliance, 31903 Horde) were
-- offered by Audrey Burnhep and Varzok from the start. No quest of the
-- starter tamer chain had a PrevQuestId, so every step could be taken out of
-- order and the continent quests needed none of it.
-- Chain order from the MoP 5.4.8 quest storylines (mop.warcraftdb.com) and the
-- NextQuestIdChain values already in quest_template. Each step now needs the
-- one before it, and both continent quests need A Tamer's Homecoming, which
-- ends the six starter tamers of the player's faction:
--   Alliance: Audrey Burnhep (32008) > Julia (31316) > Old MacDonald (31724)
--     > Lindsay (31725) > Eric Davidson (31726) > Steven Lisbane (31729)
--     > Bill Buckler (31728) > A Tamer's Homecoming (31917) > 31889, 31902
--   Horde: Varzok (32009) > Zunta (31812) > Dagra the Fierce (31813)
--     > Analynn (31814) > Zonya the Sadist (31815) > Merda Stronghoof (31817)
--     > Cassandra Kaboom (31870) > A Tamer's Homecoming (31918) > 31891, 31903

UPDATE `quest_template` SET `PrevQuestId` = 32008 WHERE `Id` = 31316;
UPDATE `quest_template` SET `PrevQuestId` = 31316 WHERE `Id` = 31724;
UPDATE `quest_template` SET `PrevQuestId` = 31724 WHERE `Id` = 31725;
UPDATE `quest_template` SET `PrevQuestId` = 31725 WHERE `Id` = 31726;
UPDATE `quest_template` SET `PrevQuestId` = 31726 WHERE `Id` = 31729;
UPDATE `quest_template` SET `PrevQuestId` = 31729 WHERE `Id` = 31728;
UPDATE `quest_template` SET `PrevQuestId` = 31728 WHERE `Id` = 31917;
UPDATE `quest_template` SET `PrevQuestId` = 31917 WHERE `Id` IN (31889, 31902);

UPDATE `quest_template` SET `PrevQuestId` = 32009 WHERE `Id` = 31812;
UPDATE `quest_template` SET `PrevQuestId` = 31812 WHERE `Id` = 31813;
UPDATE `quest_template` SET `PrevQuestId` = 31813 WHERE `Id` = 31814;
UPDATE `quest_template` SET `PrevQuestId` = 31814 WHERE `Id` = 31815;
UPDATE `quest_template` SET `PrevQuestId` = 31815 WHERE `Id` = 31817;
UPDATE `quest_template` SET `PrevQuestId` = 31817 WHERE `Id` = 31870;
UPDATE `quest_template` SET `PrevQuestId` = 31870 WHERE `Id` = 31918;
UPDATE `quest_template` SET `PrevQuestId` = 31918 WHERE `Id` IN (31891, 31903);
