-- Quest 33354 "Den Mother's Demise" starts from Moonfang's Pelt (105891),
-- dropped by Moonfang (71992) on Darkmoon Island, but no NPC ended it, so the
-- quest could be accepted and never turned in.
-- Source: 5.4.8 quest data (mop.warcraftdb.com, quest 33354): the log text
-- names Yebb Neblegear (14829) as the turn-in, and the progress and completion
-- texts below are the 5.4.8 texts. Neither the alexkulya 5.4.8 world DB nor the
-- Cata TDB has an ender row; the quest is 5.4 content. Yebb already ends the
-- sibling item quest 29444 "An Exotic Egg".
-- Values before this update: no creature_questender row for 33354,
-- RequestItemsText and OfferRewardText NULL.

DELETE FROM `creature_questender` WHERE `quest` = 33354;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(14829, 33354);

UPDATE `quest_template` SET
`RequestItemsText` = 'Is that what I think it is?',
`OfferRewardText` = 'At last, that vicious beast has been slain! Thank you.'
WHERE `Id` = 33354;
