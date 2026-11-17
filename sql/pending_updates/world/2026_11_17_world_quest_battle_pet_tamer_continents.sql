-- Battle Pet Tamers: Outland, Northrend, Cataclysm and Pandaria were offered
-- by Audrey Burnhep and Varzok from the start, next to Kalimdor and Eastern
-- Kingdoms. They now follow each other in the order of the MoP 5.4.8 quest
-- storylines (mop.warcraftdb.com), where each continent's breadcrumb (The
-- Returning Champion, Exceeding Expectations, A Brief Reprieve, The
-- Triumphant Return) leads on to the next one:
--   Kalimdor and Eastern Kingdoms > Outland > Northrend > Cataclysm > Pandaria
--   Alliance: 31889 + 31902 > 31919 > 31927 > 31966 > 31930
--   Horde:    31891 + 31903 > 31921 > 31929 > 31967 > 31952
-- Outland needs both earlier quests. A PrevQuestId holds only one, and a
-- negative ExclusiveGroup on Kalimdor and Eastern Kingdoms would also gate
-- their Grand Master quests, so Outland gets two quest accept conditions.
-- Those also hide the quest at the giver until both are rewarded.

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 19 AND `SourceEntry` IN (31919, 31921);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(19, 0, 31919, 0, 0, 8, 0, 31889, 0, 0, 0, 0, 0, '', 'Battle Pet Tamers: Outland (Alliance) - Battle Pet Tamers: Kalimdor must be rewarded'),
(19, 0, 31919, 0, 0, 8, 0, 31902, 0, 0, 0, 0, 0, '', 'Battle Pet Tamers: Outland (Alliance) - Battle Pet Tamers: Eastern Kingdoms must be rewarded'),
(19, 0, 31921, 0, 0, 8, 0, 31891, 0, 0, 0, 0, 0, '', 'Battle Pet Tamers: Outland (Horde) - Battle Pet Tamers: Kalimdor must be rewarded'),
(19, 0, 31921, 0, 0, 8, 0, 31903, 0, 0, 0, 0, 0, '', 'Battle Pet Tamers: Outland (Horde) - Battle Pet Tamers: Eastern Kingdoms must be rewarded');

UPDATE `quest_template` SET `PrevQuestId` = 31919 WHERE `Id` = 31927;
UPDATE `quest_template` SET `PrevQuestId` = 31927 WHERE `Id` = 31966;
UPDATE `quest_template` SET `PrevQuestId` = 31966 WHERE `Id` = 31930;

UPDATE `quest_template` SET `PrevQuestId` = 31921 WHERE `Id` = 31929;
UPDATE `quest_template` SET `PrevQuestId` = 31929 WHERE `Id` = 31967;
UPDATE `quest_template` SET `PrevQuestId` = 31967 WHERE `Id` = 31952;
