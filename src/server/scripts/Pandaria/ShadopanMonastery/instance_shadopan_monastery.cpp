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

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "CreatureTextMgr.h"
#include "shadopan_monastery.h"

static DoorData const doorData[] =
{
    { GO_CLOUDSTRIKE_ENTRANCE, DATA_GU_CLOUDSTRIKE, DOOR_TYPE_ROOM,    BOUNDARY_NONE },
    { GO_CLOUDSTRIKE_EXIT,     DATA_GU_CLOUDSTRIKE, DOOR_TYPE_PASSAGE, BOUNDARY_NONE },
    { 0,                       0,                   DOOR_TYPE_ROOM,    BOUNDARY_NONE }
};

class instance_shadopan_monastery : public InstanceMapScript
{
    public:
        instance_shadopan_monastery() : InstanceMapScript(ShadopanMonasteryScriptName, 959) { }

        struct instance_shadopan_monastery_InstanceScript : public InstanceScript
        {
            instance_shadopan_monastery_InstanceScript(Map* map) : InstanceScript(map)
            {
                SetBossNumber(EncounterCount);
                LoadDoorData(doorData);

                iceArchersState  = NOT_STARTED;
                fireArchersState = NOT_STARTED;
                guCloudstrikeGUID    = 0;
                masterSnowdriftGUID  = 0;
                shaViolenceGUID      = 0;
                taranZhuGUID         = 0;
                azureSerpentGUID     = 0;
            }

            void OnCreatureCreate(Creature* creature) OVERRIDE
            {
                switch (creature->GetEntry())
                {
                    case NPC_GU_CLOUDSTRIKE:
                        guCloudstrikeGUID = creature->GetGUID();
                        break;
                    case NPC_MASTER_SNOWDRIFT:
                        masterSnowdriftGUID = creature->GetGUID();
                        break;
                    case NPC_SHA_VIOLENCE:
                        shaViolenceGUID = creature->GetGUID();
                        break;
                    case NPC_TARAN_ZHU:
                        taranZhuGUID = creature->GetGUID();
                        break;
                    case NPC_AZURE_SERPENT:
                        azureSerpentGUID = creature->GetGUID();
                        break;
                    case NPC_HATEFUL_ESSENCE:
                        creature->SetDisplayId(41106);
                        break;
                    default:
                        break;
                }
            }

            void OnGameObjectCreate(GameObject* go) OVERRIDE
            {
                switch (go->GetEntry())
                {
                    case GO_CLOUDSTRIKE_ENTRANCE:
                    case GO_CLOUDSTRIKE_EXIT:
                        AddDoor(go, true);
                        break;
                    default:
                        break;
                }
            }

            void SetData(uint32 type, uint32 data) OVERRIDE
            {
                switch (type)
                {
                    case DATA_ARCHERY:
                        iceArchersState = data;
                        HandleArchers(data, NPC_ARCHERY_FIRST, IceArchersPosition, firstArcherySet);
                        break;
                    case DATA_FIRE_ARCHERY:
                        fireArchersState = data;
                        HandleArchers(data, NPC_ARCHERY_SECOND, FireArchersPosition, secondArcherySet);
                        break;
                    default:
                        break;
                }
            }

            uint32 GetData(uint32 type) const OVERRIDE
            {
                switch (type)
                {
                    case DATA_ARCHERY:
                        return iceArchersState;
                    case DATA_FIRE_ARCHERY:
                        return fireArchersState;
                    default:
                        break;
                }
                return 0;
            }

            uint64 GetData64(uint32 type) const OVERRIDE
            {
                switch (type)
                {
                    case DATA_GU_CLOUDSTRIKE:
                        return guCloudstrikeGUID;
                    case DATA_MASTER_SNOWDRIFT:
                        return masterSnowdriftGUID;
                    case DATA_SHA_VIOLENCE:
                        return shaViolenceGUID;
                    case DATA_TARAN_ZHU:
                        return taranZhuGUID;
                    case DATA_AZURE_SERPENT:
                        return azureSerpentGUID;
                    default:
                        break;
                }
                return 0;
            }

            std::string GetSaveData() OVERRIDE
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "S P M " << GetBossSaveData() << iceArchersState << ' ' << fireArchersState;

                OUT_SAVE_INST_DATA_COMPLETE;
                return saveStream.str();
            }

            void Load(char const* str) OVERRIDE
            {
                if (!str)
                {
                    OUT_LOAD_INST_DATA_FAIL;
                    return;
                }

                OUT_LOAD_INST_DATA(str);

                char dataHead1, dataHead2, dataHead3;

                std::istringstream loadStream(str);
                loadStream >> dataHead1 >> dataHead2 >> dataHead3;

                if (dataHead1 == 'S' && dataHead2 == 'P' && dataHead3 == 'M')
                {
                    for (uint32 i = 0; i < EncounterCount; ++i)
                    {
                        uint32 tmpState;
                        loadStream >> tmpState;
                        if (tmpState == IN_PROGRESS || tmpState > SPECIAL)
                            tmpState = NOT_STARTED;
                        SetBossState(i, EncounterState(tmpState));
                    }

                    uint32 temp = 0;
                    loadStream >> temp;
                    iceArchersState = temp == DONE ? DONE : NOT_STARTED;

                    loadStream >> temp;
                    fireArchersState = temp == DONE ? DONE : NOT_STARTED;
                }
                else
                    OUT_LOAD_INST_DATA_FAIL;

                OUT_LOAD_INST_DATA_COMPLETE;
            }

        private:
            // Summons the three archers on IN_PROGRESS, the last one speaks the start and end lines, all leave on DONE.
            void HandleArchers(uint32 state, uint32 entry, Position const* positions, std::list<uint64>& set)
            {
                if (state == IN_PROGRESS)
                {
                    for (uint8 i = 0; i < 3; ++i)
                    {
                        if (Creature* archer = instance->SummonCreature(entry, positions[i]))
                        {
                            set.push_back(archer->GetGUID());
                            if (i == 2)
                                sCreatureTextMgr->SendChat(archer, 0, NULL, ChatMsg::CHAT_MSG_ADDON, Language::LANG_ADDON, TEXT_RANGE_ZONE);
                        }
                    }
                }
                else if (state == DONE)
                {
                    if (!set.empty())
                        if (Creature* archer = instance->GetCreature(set.back()))
                            sCreatureTextMgr->SendChat(archer, 1, NULL, ChatMsg::CHAT_MSG_ADDON, Language::LANG_ADDON, TEXT_RANGE_ZONE);

                    for (std::list<uint64>::const_iterator itr = set.begin(); itr != set.end(); ++itr)
                        if (Creature* archer = instance->GetCreature(*itr))
                            archer->DespawnOrUnsummon();

                    set.clear();
                }
            }

            std::list<uint64> firstArcherySet;
            std::list<uint64> secondArcherySet;

            uint32 iceArchersState;
            uint32 fireArchersState;
            uint64 guCloudstrikeGUID;
            uint64 masterSnowdriftGUID;
            uint64 shaViolenceGUID;
            uint64 taranZhuGUID;
            uint64 azureSerpentGUID;
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const OVERRIDE
        {
            return new instance_shadopan_monastery_InstanceScript(map);
        }
};

void AddSC_instance_shadopan_monastery()
{
    new instance_shadopan_monastery();
}
