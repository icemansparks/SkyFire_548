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

enum DataTypes
{
    DATA_GU_CLOUDSTRIKE         = 0,
    DATA_MASTER_SNOWDRIFT       = 1,
    DATA_SHA_VIOLENCE           = 2,
    DATA_TARAN_ZHU              = 3,
    DATA_ARCHERY                = 4,
    DATA_FIRE_ARCHERY           = 5,
};

enum CreaturesIds
{
    NPC_GU_CLOUDSTRIKE          = 56747,
    NPC_MASTER_SNOWDRIFT        = 56541,
    NPC_SHA_VIOLENCE            = 56719,
    NPC_TARAN_ZHU               = 56884,

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

#endif // SHADOPAN_MONASTERY_H_
