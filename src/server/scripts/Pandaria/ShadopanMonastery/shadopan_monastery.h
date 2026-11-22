/*
* This file is part of the Pandaria 5.4.8 Project. See THANKS file for Copyright information
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

// Ported from the Pandaria 5.4.8 Project (alexkulya) and adapted to SkyFire's script API.
// Boss data, doors and challenge mode come with the encounter PRs.

#ifndef SHADOPAN_MONASTERY_H_
#define SHADOPAN_MONASTERY_H_

#include "ScriptedCreature.h"

#define ShadopanMonasteryScriptName "instance_shadopan_monastery"

uint32 const EncounterCount = 4;

#define MAX_NOVICE              24

enum DataTypes
{
    DATA_GU_CLOUDSTRIKE         = 0,
    DATA_MASTER_SNOWDRIFT       = 1,
    DATA_SHA_VIOLENCE           = 2,
    DATA_TARAN_ZHU              = 3,
    DATA_ARCHERY                = 4,
    DATA_FIRE_ARCHERY           = 5,
    DATA_AZURE_SERPENT          = 6,

    // Master Snowdrift
    DATA_SNOWDRIFT_STATE        = 7,
    DATA_RANDOM_FIRST_POS       = 8,
    DATA_RANDOM_SECOND_POS      = 9,
    DATA_RANDOM_MINIBOSS_POS    = 10,
    DATA_DEFEATED_NOVICE        = 11,
    DATA_DEFEATED_MINIBOSS      = 12,
    DATA_PANDAREN_REFEREE       = 13,
    DATA_POSSESSIONS            = 14,
};

enum CreaturesIds
{
    NPC_GU_CLOUDSTRIKE          = 56747,
    NPC_MASTER_SNOWDRIFT        = 56541,
    NPC_SHA_VIOLENCE            = 56719,
    NPC_TARAN_ZHU               = 56884,

    // Gu Cloudstrike
    NPC_AZURE_SERPENT           = 56754,
    NPC_GUARDIAN                = 59741,

    // Master Snowdrift
    NPC_NOVICE                  = 56395,
    NPC_SNOWDRIFT_POSITION      = 56397,
    NPC_FLAGRANT_LOTUS          = 56472,
    NPC_FLYING_SNOW             = 56473,
    NPC_SNOWDRIFT_CLONE         = 56713,
    NPC_PANDAREEN_REFEREE       = 56505,
    NPC_LOTUS_STAFF             = 56678,

    // Trash
    NPC_ARCHERY_FIRST           = 64549,
    NPC_ARCHERY_SECOND          = 56767,
    NPC_ARCHERY_TARGET          = 64550,
    NPC_RESIDUAL_OF_HATRED      = 58803,
    NPC_VESTIGE_OF_HATRED       = 58807,
    NPC_FRAGMENT_OF_HATRED      = 58810,
    NPC_SHADO_PAN_AMBUSHER      = 59752,
    NPC_UNSTABLE_ENERGY         = 59811,
    NPC_TRAINING_TARGET         = 60162,
    NPC_HATEFUL_ESSENCE         = 58812,
};

enum GameObjectIds
{
    GO_CLOUDSTRIKE_ENTRANCE     = 210863,
    GO_CLOUDSTRIKE_EXIT         = 210864,
    GO_SNOWDRIFT_DOJO_DOOR      = 210800,
    GO_SNOWDRIFT_EXIT           = 210862,
    GO_SNOWDRIFT_ENTRANCE       = 213194,
    GO_SNOWDRIFT_FIRE_WALL      = 212908,
    GO_SNOWDRIFT_POSSESSIONS    = 214519,
    GO_SNOWDRIFT_POSSESSIONS_H  = 214518,
};

enum SharedSpells
{
    SPELL_RESPECT               = 118918,
};

Position const IceArchersPosition[3] =
{
    { 3670.379f, 2974.310f, 874.172f, 4.673f },
    { 3657.360f, 2996.389f, 863.440f, 4.952f },
    { 3673.500f, 2998.620f, 863.341f, 4.877f }
};

Position const FireArchersPosition[3] =
{
    { 3978.959f, 2977.770f, 832.538f, 1.84f },
    { 3984.889f, 2979.379f, 832.414f, 1.84f },
    { 3971.669f, 2976.040f, 832.411f, 1.84f }
};

// Jump points of the novices and minibosses in the dojo.
Position const NovicePositions[7] =
{
    { 3657.27f, 3039.42f, 804.30f, 4.06f },
    { 3659.71f, 3037.72f, 804.30f, 4.06f },
    { 3662.04f, 3036.03f, 804.30f, 4.06f },
    { 3675.17f, 3026.88f, 804.30f, 4.08f },
    { 3677.58f, 3025.08f, 804.30f, 4.08f },
    { 3680.14f, 3023.08f, 804.30f, 4.06f },
    { 3682.78f, 3021.06f, 804.30f, 4.06f }
};

#endif // SHADOPAN_MONASTERY_H_
