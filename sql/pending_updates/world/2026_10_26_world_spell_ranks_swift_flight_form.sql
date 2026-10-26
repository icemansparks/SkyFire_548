-- Druid: Swift Flight Form (40120) did not replace Flight Form (33943).
-- Both stayed in the spellbook and on the shapeshift bar, because
-- spell_ranks had no chain for them. In the 5.4.8 SkillLineAbility.dbc
-- (row 25070, skill 798) Swift Flight Form supersedes Flight Form, the
-- same link the riding chain 33388 -> 90265 uses.
-- Existing characters with both spells keep only Swift Flight Form
-- active at their next login.
DELETE FROM `spell_ranks` WHERE `first_spell_id` IN (33943, 40120) OR `spell_id` IN (33943, 40120);
INSERT INTO `spell_ranks` (`first_spell_id`, `spell_id`, `spell_rank`) VALUES
(33943, 33943, 1),
(33943, 40120, 2);
