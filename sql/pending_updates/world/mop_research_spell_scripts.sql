-- Spell scripts for the MoP research spells that teach a recipe, from the world DB of
-- alexkulya/pandaria_5.4.8 release 2026_08_16.
-- 131593, 131686, 131688, 131690, 131691, 131695: the six jewelcrafting gem research spells.
-- 112996: Scroll of Wisdom.
-- The discovery rows of 131690 (Vermilion Onyx research) come along so the script has a list to roll.
-- Rerunnable: each row is deleted by its key and written again.
DELETE FROM `spell_script_names` WHERE `spell_id` IN (131593, 131686, 131688, 131690, 131691, 131695, 112996)
    AND `ScriptName` IN ('spell_gen_jewelcrafting_research', 'spell_gen_scroll_of_wisdom');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(131593, 'spell_gen_jewelcrafting_research'),
(131686, 'spell_gen_jewelcrafting_research'),
(131688, 'spell_gen_jewelcrafting_research'),
(131690, 'spell_gen_jewelcrafting_research'),
(131691, 'spell_gen_jewelcrafting_research'),
(131695, 'spell_gen_jewelcrafting_research'),
(112996, 'spell_gen_scroll_of_wisdom');

DELETE FROM `skill_discovery_template` WHERE `reqSpell` = 131690;
INSERT INTO `skill_discovery_template` (`spellId`, `reqSpell`, `reqSkillValue`, `chance`) VALUES
(107715, 131690, 0, 100),
(107716, 131690, 0, 100),
(107717, 131690, 0, 100),
(107718, 131690, 0, 100),
(107719, 131690, 0, 100),
(107720, 131690, 0, 100),
(107721, 131690, 0, 100),
(107722, 131690, 0, 100),
(107723, 131690, 0, 100),
(107724, 131690, 0, 100),
(107725, 131690, 0, 100),
(107726, 131690, 0, 100),
(107727, 131690, 0, 100),
(107728, 131690, 0, 100),
(107729, 131690, 0, 100),
(107730, 131690, 0, 100),
(107731, 131690, 0, 100),
(107732, 131690, 0, 100),
(107733, 131690, 0, 100),
(107734, 131690, 0, 100),
(107735, 131690, 0, 100),
(107736, 131690, 0, 100);
