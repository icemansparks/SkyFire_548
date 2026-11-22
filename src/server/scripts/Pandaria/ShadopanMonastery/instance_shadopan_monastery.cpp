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
#include "Containers.h"
#include "shadopan_monastery.h"

static DoorData const doorData[] =
{
    { GO_CLOUDSTRIKE_ENTRANCE, DATA_GU_CLOUDSTRIKE, DOOR_TYPE_ROOM,    BOUNDARY_NONE },
    { GO_CLOUDSTRIKE_EXIT,     DATA_GU_CLOUDSTRIKE, DOOR_TYPE_PASSAGE, BOUNDARY_NONE },
    { GO_SNOWDRIFT_DOJO_DOOR,  DATA_MASTER_SNOWDRIFT, DOOR_TYPE_ROOM,    BOUNDARY_NONE },
    { GO_SNOWDRIFT_EXIT,       DATA_MASTER_SNOWDRIFT, DOOR_TYPE_PASSAGE, BOUNDARY_NONE },
    { GO_SHA_ENTRANCE,         DATA_SHA_VIOLENCE,   DOOR_TYPE_ROOM,    BOUNDARY_NONE },
    { GO_SHA_EXIT,             DATA_SHA_VIOLENCE,   DOOR_TYPE_PASSAGE, BOUNDARY_NONE },
    { 0,                       0,                   DOOR_TYPE_ROOM,    BOUNDARY_NONE }
};

static Position const snowdriftCenterPos = { 3659.08f, 3015.38f, 804.74f };

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
                snowdriftState       = NOT_STARTED;
                aliveNoviceCount     = MAX_NOVICE;
                aliveMinibossCount   = 2;
                snowdriftRefereeGUID = 0;
                snowdriftEntranceGUID  = 0;
                snowdriftFireWallGUID  = 0;
                snowdriftPossessionsGUID  = 0;
                snowdriftPossessionsHGUID = 0;
                shaPrison1GUID       = 0;
                shaPrison2GUID       = 0;
                shaPrison3GUID       = 0;
                containmentOverrideGUID = 0;
                snowdriftQuestenderGUID = 0;
                taranZhuCacheGUID       = 0;
                taranZhuCacheHGUID      = 0;
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
                    case NPC_SNOWDRIFT_QUESTENDER:
                        snowdriftQuestenderGUID = creature->GetGUID();
                        // Hidden until Taran Zhu is done; a saved lockout with him done leaves Snowdrift standing for the quest.
                        if (GetBossState(DATA_TARAN_ZHU) != DONE)
                        {
                            creature->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                            creature->SetVisible(false);
                        }
                        break;
                    case NPC_TARAN_ZHU:
                        taranZhuGUID = creature->GetGUID();
                        break;
                    case NPC_AZURE_SERPENT:
                        azureSerpentGUID = creature->GetGUID();
                        break;
                    case NPC_PANDAREEN_REFEREE:
                        snowdriftRefereeGUID = creature->GetGUID();
                        break;
                    case NPC_SNOWDRIFT_POSITION:
                    {
                        // The stalkers of the dojo sit in three rings around its centre: miniboss spots, first and second novice spots.
                        float dist = creature->GetDistance(snowdriftCenterPos);
                        if (dist >= 5.0f && dist < 15.0f)
                        {
                            minibossPositions.push_back(creature->GetGUID());
                            minibossPositionsSave.push_back(creature->GetGUID());
                        }
                        else if (dist >= 15.0f && dist < 18.5f)
                        {
                            firstNovicePositions.push_back(creature->GetGUID());
                            firstNovicePositionsSave.push_back(creature->GetGUID());
                        }
                        else if (dist >= 18.5f && dist <= 25.0f)
                        {
                            secondNovicePositions.push_back(creature->GetGUID());
                            secondNovicePositionsSave.push_back(creature->GetGUID());
                        }
                        break;
                    }
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
                    case GO_SNOWDRIFT_DOJO_DOOR:
                    case GO_SNOWDRIFT_EXIT:
                    case GO_SHA_ENTRANCE:
                    case GO_SHA_EXIT:
                        AddDoor(go, true);
                        break;
                    case GO_SHA_PRISON_1:
                        shaPrison1GUID = go->GetGUID();
                        break;
                    case GO_SHA_PRISON_2:
                        shaPrison2GUID = go->GetGUID();
                        break;
                    case GO_SHA_PRISON_3:
                        shaPrison3GUID = go->GetGUID();
                        break;
                    case GO_CONTAINMENT_OVERRIDE:
                        containmentOverrideGUID = go->GetGUID();
                        break;
                    case GO_TARAN_ZHU_CACHE:
                        taranZhuCacheGUID = go->GetGUID();
                        break;
                    case GO_TARAN_ZHU_CACHE_H:
                        taranZhuCacheHGUID = go->GetGUID();
                        break;
                    case GO_SNOWDRIFT_ENTRANCE:
                        snowdriftEntranceGUID = go->GetGUID();
                        break;
                    case GO_SNOWDRIFT_FIRE_WALL:
                        snowdriftFireWallGUID = go->GetGUID();
                        break;
                    case GO_SNOWDRIFT_POSSESSIONS:
                        snowdriftPossessionsGUID = go->GetGUID();
                        break;
                    case GO_SNOWDRIFT_POSSESSIONS_H:
                        snowdriftPossessionsHGUID = go->GetGUID();
                        break;
                    default:
                        break;
                }
            }

            bool SetBossState(uint32 type, EncounterState state) OVERRIDE
            {
                if (!InstanceScript::SetBossState(type, state))
                    return false;

                // Snowdrift leaves his possessions behind: the normal or the heroic chest, by difficulty.
                if (type == DATA_MASTER_SNOWDRIFT && state == DONE)
                    DoRespawnGameObject(instance->IsHeroic() ? snowdriftPossessionsHGUID : snowdriftPossessionsGUID, DAY);

                // Taran Zhu: the Hate aura rides on every player during the fight and leaves with it; his stash appears when he is done.
                if (type == DATA_TARAN_ZHU)
                {
                    if (state == IN_PROGRESS)
                        DoAddAuraOnPlayers(SPELL_HATE);
                    else
                    {
                        DoRemoveAurasDueToSpellOnPlayers(SPELL_HATE);
                        DoRemoveAurasDueToSpellOnPlayers(SPELL_HAZE_OF_HATE);
                        DoRemoveAurasDueToSpellOnPlayers(SPELL_HAZE_OF_HATE_VISUAL);

                        if (state == DONE)
                            DoRespawnGameObject(instance->IsHeroic() ? taranZhuCacheHGUID : taranZhuCacheGUID, DAY);
                    }
                }

                return true;
            }

            void SetData(uint32 type, uint32 data) OVERRIDE
            {
                switch (type)
                {
                    case DATA_DEFEATED_NOVICE:
                        if (aliveNoviceCount && !--aliveNoviceCount)
                            if (Creature* referee = instance->GetCreature(snowdriftRefereeGUID))
                                if (referee->IsAIEnabled)
                                    referee->AI()->DoAction(1); // ACTION_NOVICE_DONE
                        break;
                    case DATA_DEFEATED_MINIBOSS:
                        if (aliveMinibossCount && !--aliveMinibossCount)
                            if (Creature* referee = instance->GetCreature(snowdriftRefereeGUID))
                                if (referee->IsAIEnabled)
                                    referee->AI()->DoAction(2); // ACTION_MINIBOSS_DONE
                        break;
                    case DATA_SNOWDRIFT_STATE:
                        snowdriftState = data;
                        if (data == IN_PROGRESS || data == FAIL)
                        {
                            aliveNoviceCount = MAX_NOVICE;
                            aliveMinibossCount = 2;
                            minibossPositions = minibossPositionsSave;
                            firstNovicePositions = firstNovicePositionsSave;
                            secondNovicePositions = secondNovicePositionsSave;
                        }
                        switch (data)
                        {
                            case IN_PROGRESS:
                                HandleGameObject(snowdriftEntranceGUID, false);
                                break;
                            case DONE:
                                HandleGameObject(snowdriftFireWallGUID, true);
                                // no break: the entrance opens again as well
                            case FAIL:
                            case NOT_STARTED:
                                HandleGameObject(snowdriftEntranceGUID, true);
                                break;
                            default:
                                break;
                        }
                        break;
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
                    case DATA_SNOWDRIFT_STATE:
                        return snowdriftState;
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
                    case DATA_PANDAREN_REFEREE:
                        return snowdriftRefereeGUID;
                    case DATA_POSSESSIONS:
                        return instance->IsHeroic() ? snowdriftPossessionsHGUID : snowdriftPossessionsGUID;
                    case DATA_SHA_PRISON_1:
                        return shaPrison1GUID;
                    case DATA_SHA_PRISON_2:
                        return shaPrison2GUID;
                    case DATA_SHA_PRISON_3:
                        return shaPrison3GUID;
                    case DATA_CONTAINMENT_OVERRIDE:
                        return containmentOverrideGUID;
                    case DATA_SNOWDRIFT_QUESTENDER:
                        return snowdriftQuestenderGUID;
                    case DATA_TARAN_ZHU_CACHE:
                        return instance->IsHeroic() ? taranZhuCacheHGUID : taranZhuCacheGUID;
                    case DATA_RANDOM_FIRST_POS:
                        return TakeRandomGuid(firstNovicePositions);
                    case DATA_RANDOM_SECOND_POS:
                        return TakeRandomGuid(secondNovicePositions);
                    case DATA_RANDOM_MINIBOSS_POS:
                        return TakeRandomGuid(minibossPositions);
                    default:
                        break;
                }
                return 0;
            }

            std::string GetSaveData() OVERRIDE
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "S P M " << GetBossSaveData() << iceArchersState << ' ' << fireArchersState << ' ' << snowdriftState;

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

                    // Saves written before the Snowdrift event end here; the stream then leaves temp at 0.
                    temp = 0;
                    loadStream >> temp;
                    snowdriftState = temp == DONE ? DONE : NOT_STARTED;
                }
                else
                    OUT_LOAD_INST_DATA_FAIL;

                OUT_LOAD_INST_DATA_COMPLETE;
            }

        private:
            // Hands out one stalker guid and takes it off the list, so a position is used once per run.
            uint64 TakeRandomGuid(std::list<uint64>& guids) const
            {
                if (guids.empty())
                    return 0;

                uint64 guid = Skyfire::Containers::SelectRandomContainerElement(guids);
                guids.remove(guid);
                return guid;
            }

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

            // Mutable: GetData64 hands out each stalker once.
            mutable std::list<uint64> minibossPositions;
            mutable std::list<uint64> firstNovicePositions;
            mutable std::list<uint64> secondNovicePositions;
            std::list<uint64> minibossPositionsSave;
            std::list<uint64> firstNovicePositionsSave;
            std::list<uint64> secondNovicePositionsSave;

            uint32 snowdriftState;
            uint8 aliveNoviceCount;
            uint8 aliveMinibossCount;
            uint64 snowdriftRefereeGUID;
            uint64 snowdriftEntranceGUID;
            uint64 snowdriftFireWallGUID;
            uint64 snowdriftPossessionsGUID;
            uint64 snowdriftPossessionsHGUID;
            uint64 shaPrison1GUID;
            uint64 shaPrison2GUID;
            uint64 shaPrison3GUID;
            uint64 containmentOverrideGUID;
            uint64 snowdriftQuestenderGUID;
            uint64 taranZhuCacheGUID;
            uint64 taranZhuCacheHGUID;

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
