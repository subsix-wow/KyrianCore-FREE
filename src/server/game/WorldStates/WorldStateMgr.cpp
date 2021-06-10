#include "Map.h"
#include "WorldStateMgr.h"
#include "Player.h"
#include "GameObject.h"
#include "GridNotifiers.h"
#include "CellImpl.h"
#include "BattlegroundMgr.h"
#include "ScriptMgr.h"
#include "WorldStatePackets.h"
#include "MapManager.h"

void AddToWorldStateSet(WorldStateSet** worldStateSet, WorldState const* state)
{
    if (!*worldStateSet)
        *worldStateSet = new WorldStateSet;

    (*worldStateSet)->add(state);
}

WorldStateMgr::WorldStateMgr()
{
}

WorldStateMgr::~WorldStateMgr()
{
    _delayedOperations.KillAllFunctions();
}

WorldStateMgr& WorldStateMgr::Instance()
{
    static WorldStateMgr instance;
    return instance;
}

void WorldStateMgr::Update(uint32 diff)
{
    _delayedOperations.Update(diff);

    if (_worldState.empty())
        return;

    for (auto itr = _worldState.begin(); itr != _worldState.end(); ++itr)
    {
        auto state = &itr->second;
        if (!state || state->HasFlag(WorldStatesData::Flags::Deleted))
            continue;

        if (state->RenewTime < time(nullptr) - time_t(WEEK))
        {
            if (state->HasFlag(WorldStatesData::Flags::NotExpireable))
                continue;

            if (state->HasFlag(WorldStatesData::Flags::InitialState))
            {
                state->Initialize();
                continue;
            }

            state->RemoveFlag(WorldStatesData::Flags::Active);
            state->AddFlag(WorldStatesData::Flags::Deleted);

            sScriptMgr->OnWorldStateDelete(state->VariableID, state->Type);
        }
    }

    SaveToDB();

    for (auto itr = _worldState.begin(); itr != _worldState.end(); ++itr)
        if (itr->second.HasFlag(WorldStatesData::Flags::Deleted))
            _worldState.erase(itr++);
}

void WorldStateMgr::Initialize()
{
    _worldStateTemplates.clear();

    LoadTemplatesFromDB();
    LoadFromDB();

    CreateWorldStatesIfNeed();
}

WorldStateTemplate const* WorldStateMgr::FindTemplate(uint32 variableID, uint32 type, uint32 /*conditionID*/, uint32 linkedID)
{
    if (type == WorldStatesData::Types::Max && /*conditionID == 0 &&*/ linkedID == 0 && static_cast<int>(_worldStateTemplates.count(variableID)) > 1)
        return nullptr;

    auto bounds = _worldStateTemplates.equal_range(variableID);
    if (bounds.first == bounds.second)
        return nullptr;

    for (auto iter = bounds.first; iter != bounds.second; ++iter)
        if (iter->second.VariableID == variableID && (type == WorldStatesData::Types::Max || iter->second.VariableType == type) && /*(conditionID == 0 || iter->second.ConditionID == conditionID) &&*/ (linkedID == 0 || iter->second.LinkedID == linkedID))
            return &iter->second;

    return nullptr;
}

void WorldStateMgr::AddTemplate(uint32 ID, WorldStateTemplate& ws)
{
    _worldStateTemplates.insert({ID, ws});
}

void WorldStateMgr::LoadTemplatesFromDB()
{
    //                                         0             1       2              3        4               5
    auto result = WorldDatabase.Query("SELECT `VariableID`, `Type`, `ConditionID`, `Flags`, `DefaultValue`, `LinkedID` FROM `worldstate_template`");
    if (!result)
        return;

    do
    {
        auto fields = result->Fetch();
        auto variableID = fields[0].GetUInt32();
        _worldStateTemplates.insert({ variableID, WorldStateTemplate(variableID, fields[1].GetUInt32(), fields[2].GetUInt32(), fields[3].GetUInt32(), fields[4].GetUInt32(), fields[5].GetUInt32()) });

    } while (result->NextRow());

    for (auto itr = _worldStateTemplates.begin(); itr != _worldStateTemplates.end(); ++itr)
        if (itr->second.LinkedID != 0)
            if (auto stateTemplate = FindTemplate(itr->second.LinkedID, itr->second.VariableType, itr->second.ConditionID))
                const_cast<WorldStateTemplate*>(stateTemplate)->LinkedList.insert(itr->second.VariableID);

    //sLog->"outInfo"("LOG_FILTER_GENERAL", "%s >> Loaded %u templates", __FUNCTION__, _worldStateTemplates.size());
}

void WorldStateMgr::LoadFromDB()
{
    _worldState.clear();

    //                                             0             1             2       3              4        5        6
    auto result = CharacterDatabase.Query("SELECT `VariableID`, `InstanceID`, `Type`, `ConditionID`, `Flags`, `Value`, `RenewTime` FROM `worldstate_data`");
    if (!result)
        return;

    do
    {
        auto fields = result->Fetch();

        auto variableID = fields[0].GetUInt32();
        auto instanceID = fields[1].GetUInt32();
        auto type = fields[2].GetUInt32();
        auto conditionID = fields[3].GetUInt32();
        auto flags = fields[4].GetUInt32();
        auto value = fields[5].GetUInt32();
        auto renewtime = time_t(fields[6].GetUInt64());

        if (auto stateTemplate = FindTemplate(variableID, type, conditionID))
        {
            if (auto state = GetWorldState(stateTemplate, instanceID))
            {
                if (state->Value != value)
                    const_cast<WorldState*>(state)->SetValue(value, false);
            }
            else
                _worldState.insert({ variableID, WorldState(stateTemplate, instanceID, flags, value, renewtime) });
        }
        else if (type == WorldStatesData::Types::Custom)
            _worldState.insert({ variableID, WorldState(variableID, instanceID, flags, value, renewtime) });

    } while (result->NextRow());
}

void WorldStateMgr::CreateWorldStatesIfNeed()
{
    for (auto itr = _worldStateTemplates.begin(); itr != _worldStateTemplates.end(); ++itr)
        if (itr->second.HasFlag(WorldStatesData::Flags::InitialState) && (itr->second.VariableType == WorldStatesData::Types::World || itr->second.VariableType == WorldStatesData::Types::Weekly))
            if (!GetWorldState(&itr->second, 0))
                CreateWorldState(&itr->second, 0);
}

void WorldStateMgr::SaveToDB()
{
    CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();

    for (auto iter = _worldState.begin(); iter != _worldState.end(); ++iter)
        if (!iter->second.HasFlag(WorldStatesData::Flags::Saved))
        {
            auto state = &iter->second;

            auto stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_WORLD_STATE_BY_STATE_INSTANCE);
            stmt->setUInt32(0, state->VariableID);
            stmt->setUInt32(1, state->InstanceID);
            trans->Append(stmt);

            if (!state->HasFlag(WorldStatesData::Flags::Deleted))
            {
                const_cast<WorldState*>(state)->AddFlag(WorldStatesData::Flags::Saved);

                stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_WORLD_STAT);
                stmt->setUInt32(0, state->VariableID);
                stmt->setUInt32(1, state->InstanceID);
                stmt->setUInt32(2, state->Type);
                stmt->setUInt32(3, state->ConditionID);
                stmt->setUInt32(4, state->Flags);
                stmt->setUInt32(5, state->Value);
                stmt->setUInt64(6, state->RenewTime);
                trans->Append(stmt);
            }
        }

    CharacterDatabase.CommitTransaction(trans);
}

void WorldStateMgr::SendWorldStateUpdate(Player* target, uint32 variableID, uint32 value, bool hidden)
{
    WorldPackets::WorldState::UpdateWorldState packet;
    packet.VariableID = static_cast<WorldStates>(variableID);
    packet.Value = value;
    packet.Hidden = hidden;
    target->SendDirectMessage(packet.Write());
}

void WorldStateMgr::DeleteWorldState(WorldState* state)
{
    if (!state)
        return;

    auto trans = CharacterDatabase.BeginTransaction();
    auto stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_WORLD_STATE_BY_STATE_INSTANCE);
    stmt->setUInt32(0, state->VariableID);
    stmt->setUInt32(1, state->InstanceID);
    trans->Append(stmt);
    CharacterDatabase.CommitTransaction(trans);

    for (auto iter = _worldState.begin(); iter != _worldState.end(); ++iter)
    {
        if (&iter->second != state)
            continue;

        iter->second.RemoveFlag(WorldStatesData::Flags::Active);
        iter->second.AddFlag(WorldStatesData::Flags::Deleted);
        break;
    }
}

void WorldStateMgr::MapUpdate(Map* map)
{
    for (auto itr = _worldState.begin(); itr != _worldState.end(); ++itr)
    {
        auto state = &itr->second;
        if (!state || state->HasFlag(WorldStatesData::Flags::Deleted) || !IsFitToCondition(map, state))
            continue;

        switch (state->Type)
        {
            case WorldStatesData::Types::Custom:
            case WorldStatesData::Types::World:
            case WorldStatesData::Types::Weekly:
            case WorldStatesData::Types::Event:
            case WorldStatesData::Types::Map:
            case WorldStatesData::Types::Zone:
            case WorldStatesData::Types::Area:
            case WorldStatesData::Types::Battlegound:
            case WorldStatesData::Types::CapturePoint:
            case WorldStatesData::Types::DestructibleObject:
                break;
            default:
                break;
        }
    }
}

WorldStateSet* WorldStateMgr::GetWorldStatesFor(Player* player, uint32 flags)
{
    WorldStateSet* stateSet = nullptr;

    auto bFull = player ? !player->IsInWorld() : true;

    for (auto itr = _worldState.begin(); itr != _worldState.end(); ++itr)
    {
        auto state = &itr->second;
        if (!state || state->HasFlag(WorldStatesData::Flags::Deleted))
            continue;

        if (state->Flags & flags)
            if (bFull || IsFitToCondition(player, state))
                AddToWorldStateSet(&stateSet, state);
    }

    return stateSet;
}

WorldStateSet* WorldStateMgr::GetWorldStatesFor(Player* player, WorldStatesData::Flags flag)
{
    return GetWorldStatesFor(player, 1 << flag);
}

WorldStateSet* WorldStateMgr::GetUpdatedWorldStatesFor(Player* player, time_t updateTime)
{
    WorldStateSet* stateSet = nullptr;

    for (auto itr = _worldState.begin(); itr != _worldState.end(); ++itr)
    {
        auto state = &itr->second;
        if (!state || state->HasFlag(WorldStatesData::Flags::Deleted))
            continue;

        if (state->HasFlag(WorldStatesData::Flags::Active) && state->RenewTime >= updateTime && state->RenewTime != time(nullptr) && IsFitToCondition(player, state))
        {
            if (state->GetTemplate() && state->GetTemplate()->LinkedID)
                if (auto stateTemplate = FindTemplate(state->GetTemplate()->LinkedID, state->Type, state->ConditionID))
                    if (auto state1 = GetWorldState(stateTemplate, state->InstanceID))
                        AddToWorldStateSet(&stateSet, state1);

            AddToWorldStateSet(&stateSet, state);
        }
    }

    return stateSet;
}

bool WorldStateMgr::IsFitToCondition(Player* player, WorldState const* state)
{
    if (!player || !state)
        return false;

    if (state->HasFlag(WorldStatesData::Flags::Deleted))
        return false;

    switch (state->Type)
    {
        case WorldStatesData::Types::World:
        case WorldStatesData::Types::Event:
        case WorldStatesData::Types::Battlegound:
            return true;
        case WorldStatesData::Types::Weekly:
            return player->IsPvP();
        case WorldStatesData::Types::Map:
            return player->GetMapId() == state->ConditionID && player->GetInstanceId() == state->InstanceID;
        case WorldStatesData::Types::Zone:
            return player->GetInstanceId() == state->InstanceID;
        case WorldStatesData::Types::Area:
            return player->GetCurrentAreaID() == state->ConditionID && player->GetInstanceId() == state->InstanceID;
        case WorldStatesData::Types::CapturePoint:
            if (state->HasClient(player))
                return true;
            if (auto point = player->GetMap()->GetGameObject(state->LinkedGuid))
            {
                auto goInfo = point->GetGOInfo();
                if (!goInfo || goInfo->type != GAMEOBJECT_TYPE_CAPTURE_POINT)
                    return false;

                if (player->IsWithinDistInMap(point, goInfo->controlZone.radius, true))
                    return true;
            }
            return false;
        case WorldStatesData::Types::DestructibleObject:
            break;
        case WorldStatesData::Types::Custom:
            if (!state->ConditionID)
                return true;
            return (player->GetMapId() == state->ConditionID || player->GetCurrentAreaID() == state->ConditionID) && player->GetInstanceId() == state->InstanceID;
        default:
            break;
    }
    return false;
}

bool WorldStateMgr::IsFitToCondition(Map* map, WorldState const* state) const
{
    if (!map || !state)
        return false;

    return IsFitToCondition(map->GetId(), map->GetInstanceId(), 0, 0, state);
}

bool WorldStateMgr::IsFitToCondition(uint32 mapID, uint32 instanceID, uint32 zoneID, uint32 areaID, WorldState const* state)
{
    if (!state)
        return false;

    if (state->HasFlag(WorldStatesData::Flags::Deleted))
        return false;

    switch (state->Type)
    {
        case WorldStatesData::Types::World:
        case WorldStatesData::Types::Event:
        case WorldStatesData::Types::Weekly:
        case WorldStatesData::Types::Battlegound:
            return true;
        case WorldStatesData::Types::Map:
            return mapID == state->ConditionID && instanceID == state->InstanceID;
        case WorldStatesData::Types::Zone:
            if (zoneID == 0)
                return false;
            return zoneID == state->ConditionID && instanceID == state->InstanceID;
        case WorldStatesData::Types::Area:
            if (areaID == 0)
                return false;
            return areaID == state->ConditionID && instanceID == state->InstanceID;
        case WorldStatesData::Types::CapturePoint:
            return instanceID == state->InstanceID;
        case  WorldStatesData::Types::DestructibleObject:
            if (instanceID)
                return instanceID == state->InstanceID;
            return zoneID == state->ConditionID;
        case WorldStatesData::Types::Custom:
            if (!state->ConditionID)
                return true;
            return mapID == state->ConditionID && instanceID == state->InstanceID;
        default:
            break;
    }
    return false;
}

uint32 WorldStateMgr::GetWorldStateValue(uint32 variableID)
{
    auto bounds = _worldState.equal_range(variableID);
    if (bounds.first == bounds.second)
        return std::numeric_limits<uint32>().max();

    return bounds.first->second.Value;
}

uint32 WorldStateMgr::GetWorldStateValueFor(Player* player, uint32 variableID)
{
    if (!player)
        return std::numeric_limits<uint32>().max();

    auto bounds = _worldState.equal_range(variableID);
    if (bounds.first == bounds.second)
        return std::numeric_limits<uint32>().max();

    for (auto iter = bounds.first; iter != bounds.second; ++iter)
        if (IsFitToCondition(player, &iter->second))
            return iter->second.Value;

    return std::numeric_limits<uint32>().max();
}

uint32 WorldStateMgr::GetWorldStateValueFor(Map* map, uint32 variableID)
{
    if (!map)
        return std::numeric_limits<uint32>().max();

    auto bounds = _worldState.equal_range(variableID);
    if (bounds.first == bounds.second)
        return std::numeric_limits<uint32>().max();

    for (auto iter = bounds.first; iter != bounds.second; ++iter)
        if (IsFitToCondition(map, &iter->second))
            return iter->second.Value;

    return std::numeric_limits<uint32>().max();
}

uint32 WorldStateMgr::GetWorldStateValueFor(uint32 mapID, uint32 instanceID, uint32 zoneID, uint32 areaID, uint32 variableID)
{
    auto bounds = _worldState.equal_range(variableID);
    if (bounds.first == bounds.second)
        return std::numeric_limits<uint32>().max();

    for (auto iter = bounds.first; iter != bounds.second; ++iter)
        if (IsFitToCondition(mapID, instanceID, zoneID, areaID, &iter->second))
            return iter->second.Value;

    return std::numeric_limits<uint32>().max();
}

uint32 WorldStateMgr::GetWorldStateValueFor(WorldObject* object, uint32 variableID)
{
    if (!object)
        return std::numeric_limits<uint32>().max();

    if (!object->GetGUID().IsGameObject())
        return object->GetGUID().IsPlayer() ? GetWorldStateValueFor(static_cast<Player*>(object), variableID) : GetWorldStateValueFor(object->GetMap(), variableID);

    auto goInfo = static_cast<GameObject*>(object)->GetGOInfo();
    if (!goInfo || goInfo->type != GAMEOBJECT_TYPE_CAPTURE_POINT)
        return GetWorldStateValueFor(object->GetMap(), variableID);

    auto bounds = _worldState.equal_range(variableID);

    if (bounds.first != bounds.second)
        for (auto iter = bounds.first; iter != bounds.second; ++iter)
            if (IsFitToCondition(object->GetMap()->GetId(), object->GetGUID().GetCounter(), 0, 0, &iter->second))
                return iter->second.Value;

    return std::numeric_limits<uint32>().max();
}

uint32 WorldStateMgr::GetWorldStateValueFor(uint32 zoneID, uint32 variableID)
{
    if (!zoneID || !CheckWorldState(variableID))
        return std::numeric_limits<uint32>().max();

    return GetWorldStateValueFor(GetMapIdByZoneId(zoneID), 0, zoneID, 0, variableID);
}

void WorldStateMgr::SetWorldStateValueFor(Player* player, uint32 variableID, uint32 value, bool hidden /*= false*/)
{
    if (!player)
        return;

    CreateWorldState(variableID, player->GetInstanceId(), value);

    auto bounds = _worldState.equal_range(variableID);
    if (bounds.first != bounds.second)
    {
        for (auto itr = bounds.first; itr != bounds.second; ++itr)
            if (IsFitToCondition(player, &itr->second))
            {
                if ((&itr->second)->Value != value)
                    const_cast<WorldState*>(&itr->second)->SetValue(value, hidden);
                return;
            }
    }
}

void WorldStateMgr::SetWorldStateValueFor(Map* map, uint32 variableID, uint32 value, bool hidden /*= false*/)
{
    if (!map)
        return;

    CreateWorldState(variableID, map->GetInstanceId(), value);

    auto bounds = _worldState.equal_range(variableID);
    if (bounds.first != bounds.second)
        for (auto itr = bounds.first; itr != bounds.second; ++itr)
            if (IsFitToCondition(map, &itr->second))
            {
                if ((&itr->second)->Value != value)
                    const_cast<WorldState*>(&itr->second)->SetValue(value, hidden);
                return;
            }
}

void WorldStateMgr::SetWorldStateValueFor(uint32 zoneID, uint32 variableID, uint32 value, bool hidden /*= false*/)
{
    if (!zoneID || !CheckWorldState(variableID))
        return;

    auto mapID = GetMapIdByZoneId(zoneID);

    auto bounds = _worldState.equal_range(variableID);
    if (bounds.first != bounds.second)
    {
        for (auto itr = bounds.first; itr != bounds.second; ++itr)
        {
            if (IsFitToCondition(mapID, 0, zoneID, 0, &itr->second))
            {
                if ((&itr->second)->Value != value)
                    const_cast<WorldState*>(&itr->second)->SetValue(value, hidden);
                return;
            }
        }
    }

    CreateWorldState(variableID, 0, value);
}

void WorldStateMgr::SetWorldStateValueFor(WorldObject* object, uint32 variableID, uint32 value, bool hidden /*= false*/)
{
    if (!object)
        return;

    if (!object->GetGUID().IsGameObject())
    {
        object->GetGUID().IsPlayer() ? SetWorldStateValueFor(static_cast<Player*>(object), variableID, value) : SetWorldStateValueFor(object->GetMap(), variableID, value);
        return;
    }

    auto goInfo = static_cast<GameObject*>(object)->GetGOInfo();
    if (!goInfo || goInfo->type != GAMEOBJECT_TYPE_CAPTURE_POINT && goInfo->type != GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING)
    {
        SetWorldStateValueFor(object->GetMap(), variableID, value);
        return;
    }

    auto bounds = _worldState.equal_range(variableID);
    if (bounds.first != bounds.second)
    {
        for (auto itr = bounds.first; itr != bounds.second; ++itr)
        {
            auto stateTemplate = &itr->second;
            if (!stateTemplate)
                continue;

            if (IsFitToCondition(object->GetMap()->GetId(), object->GetGUID().GetCounter(), 0, 0, stateTemplate))
            {
                if (stateTemplate->Value != value)
                    const_cast<WorldState*>(stateTemplate)->SetValue(value, hidden);

                return;
            }
        }
    }

    CreateWorldState(variableID, object->GetGUID().GetCounter(), value);
}

WorldState const* WorldStateMgr::CreateWorldState(uint32 variableID, uint32 instanceID, uint32 value)
{
    if (variableID == 0)
        return nullptr;

    return CreateWorldState(FindTemplate(variableID), instanceID, value);
}

WorldState const* WorldStateMgr::CreateWorldState(WorldStateTemplate const* stateTemplate, uint32 instanceID, uint32 value)
{
    if (!stateTemplate)
        return nullptr;

    if (stateTemplate->IsGlobal() && instanceID > 0)
        return nullptr;

    sScriptMgr->OnWorldStateCreate(stateTemplate->VariableID, value, stateTemplate->VariableType);

    if (auto stateTemplateData = GetWorldState(stateTemplate, instanceID))
    {
        const_cast<WorldState*>(stateTemplateData)->SetValue(value, false);
        return stateTemplateData;
    }

    _worldState.insert({ stateTemplate->VariableID, WorldState(stateTemplate, instanceID) });

    auto stateTemplateData = const_cast<WorldState*>(GetWorldState(stateTemplate, instanceID));
    if (!stateTemplateData)
        return nullptr;

    if (value != std::numeric_limits<uint32>().max())
        stateTemplateData->SetValue(value, false);
    else
        stateTemplateData->RemoveFlag(WorldStatesData::Flags::Saved);

    if (!stateTemplateData->HasFlag(WorldStatesData::Flags::PassiaveAtCreate))
        stateTemplateData->AddFlag(WorldStatesData::Flags::Active);

    return stateTemplateData;
}

WorldState const* WorldStateMgr::GetWorldState(uint32 variableID, uint32 instanceID, WorldStatesData::Types type, uint32 conditionID)
{
    auto bounds = _worldState.equal_range(variableID);
    if (bounds.first == bounds.second)
        return nullptr;

    for (auto iter = bounds.first; iter != bounds.second; ++iter)
        if (iter->second.InstanceID == instanceID && type == iter->second.Type && conditionID == iter->second.ConditionID)
            return &iter->second;

    return nullptr;
}

WorldState const* WorldStateMgr::GetWorldState(uint32 variableID, uint32 instanceID, Player* player)
{
    auto bounds = _worldState.equal_range(variableID);
    if (bounds.first == bounds.second)
        return nullptr;

    for (auto iter = bounds.first; iter != bounds.second; ++iter)
        if (iter->second.InstanceID == instanceID && (!player || IsFitToCondition(player, &iter->second)))
            return &iter->second;

    return nullptr;
}

WorldState const* WorldStateMgr::GetWorldState(WorldStateTemplate const* stateTemplate, uint32 instanceID)
{
    if (!stateTemplate)
        return nullptr;

    auto bounds = _worldState.equal_range(stateTemplate->VariableID);
    if (bounds.first == bounds.second)
        return nullptr;

    for (auto iter = bounds.first; iter != bounds.second; ++iter)
        if (iter->second.InstanceID == instanceID && iter->second.GetTemplate() == stateTemplate)
            return &iter->second;

    return nullptr;
}

void WorldStateMgr::AddWorldStateFor(Player* player, uint32 variableID, uint32 instanceID)
{
    if (!player)
        return;

    auto state = GetWorldState(variableID, instanceID, player);
    if (!state || !state->HasClient(player))
        return;

    const_cast<WorldState*>(state)->AddClient(player);

    if (!player->IsInWorld())
        return;

    if (state->HasDownLink())
    {
        if (auto stateSet = GetDownLinkedWorldStates(state))
        {
            for (uint32 i = 0; i < stateSet->Count; ++i)
            {
                auto ws = (*stateSet)[i];
                ws->AddClient(player);
                SendWorldStateUpdate(player, ws->VariableID, ws->Value, false);
            }
        }
    }

    SendWorldStateUpdate(player, variableID, WorldStatesData::InitialValue::Add, false);
}

void WorldStateMgr::RemoveWorldStateFor(Player* player, uint32 variableID, uint32 instanceID)
{
    if (!player)
        return;

    auto state = GetWorldState(variableID, instanceID, player);
    if (!state || !state->HasClient(player))
        return;

    if (state->HasDownLink())
        if (auto stateSet = GetDownLinkedWorldStates(state))
            for (uint32 i = 0; i < stateSet->Count; ++i)
                (*stateSet)[i]->RemoveClient(player);

    const_cast<WorldState*>(state)->RemoveClient(player);
    SendWorldStateUpdate(player, variableID, WorldStatesData::InitialValue::Remove, false);
}

void WorldStateMgr::CreateInstanceState(uint32 mapID, uint32 instanceID)
{
    for (auto itr = _worldStateTemplates.begin(); itr != _worldStateTemplates.end(); ++itr)
        if (itr->second.HasFlag(WorldStatesData::Flags::InitialState))
        {
            if (itr->second.VariableType == WorldStatesData::Types::Map && itr->second.ConditionID != mapID)
                continue;

            CreateWorldState(&itr->second, instanceID);
        }
}

void WorldStateMgr::CreateZoneAreaStateIfNeed(Player* player, uint32 zoneID, uint32 areaID)
{
    if (!player)
        return;

    if (player->GetInstanceId() != 0)
        return;

    for (auto itr = _worldStateTemplates.begin(); itr != _worldStateTemplates.end(); ++itr)
    {
        if (itr->second.HasFlag(WorldStatesData::Flags::Active) && !GetWorldState(&itr->second, player->GetInstanceId()))
        {
            if (itr->second.VariableType == WorldStatesData::Types::Zone && itr->second.ConditionID == zoneID)
                CreateWorldState(&itr->second, player->GetInstanceId());
            else if (itr->second.VariableType == WorldStatesData::Types::Area && itr->second.ConditionID == areaID)
                CreateWorldState(&itr->second, player->GetInstanceId());
        }
    }
}

void WorldStateMgr::DeleteInstanceState(uint32 mapID, uint32 instanceID)
{
    if (instanceID == 0)
        return;

    auto targetMapEntry = sMapStore.LookupEntry(mapID);
    if (!targetMapEntry || targetMapEntry->IsContinent() || !targetMapEntry->Instanceable())
        return;

    if (auto stateSet = GetInstanceStates(mapID, instanceID))
        for (uint32 i = 0; i < stateSet->Count; ++i)
            DeleteWorldState((*stateSet)[i]);
}

WorldStateSet* WorldStateMgr::GetInstanceStates(Map* map, uint32 flags, bool full)
{
    if (!map)
        return nullptr;

    return GetInstanceStates(map->GetId(), map->GetInstanceId(), flags, full);
}

WorldStateSet* WorldStateMgr::GetInstanceStates(uint32 mapID, uint32 instanceID, uint32 flags, bool full)
{
    WorldStateSet* stateSet = nullptr;

    for (auto itr = _worldState.begin(); itr != _worldState.end(); ++itr)
    {
        auto state = &itr->second;
        if (!state || state->HasFlag(WorldStatesData::Flags::Deleted))
            continue;

        if (!flags || state->Flags & flags)
        {
            if (state->Type == WorldStatesData::Types::Map && state->ConditionID == mapID && state->InstanceID == instanceID)
                AddToWorldStateSet(&stateSet, state);
            else if (full && IsFitToCondition(sMapMgr->FindMap(mapID, instanceID), state))
                AddToWorldStateSet(&stateSet, state);
        }
    }
    return stateSet;
}

WorldStateSet* WorldStateMgr::GetInitWorldStates(uint32 mapID, uint32 instanceID, uint32 zoneID, uint32 areaID)
{
    WorldStateSet* stateSet = nullptr;

    for (auto itr = _worldState.begin(); itr != _worldState.end(); ++itr)
    {
        auto state = &itr->second;
        if (!state || state->HasFlag(WorldStatesData::Flags::Deleted))
            continue;

        if ((state->HasFlag(WorldStatesData::Flags::InitialState) || state->HasFlag(WorldStatesData::Flags::Active)) && IsFitToCondition(mapID, instanceID, zoneID, areaID, state))
        {
            if (state->HasDownLink())
                if (auto linkedStateSet = GetDownLinkedWorldStates(state))
                    for (uint32 i = 0; i < linkedStateSet->Count; ++i)
                        AddToWorldStateSet(&stateSet, (*linkedStateSet)[i]);

            AddToWorldStateSet(&stateSet, &itr->second);
        }
    }
    return stateSet;
}

WorldStateSet* WorldStateMgr::GetDownLinkedWorldStates(WorldState const* state)
{
    if (!state->HasDownLink())
        return nullptr;

    WorldStateSet* stateSet = nullptr;
    for (auto itr = state->GetLinkedSet()->begin(); itr != state->GetLinkedSet()->end(); ++itr)
    {
        auto stateTemplate = FindTemplate(*itr, state->Type, state->ConditionID, state->VariableID);
        if (!stateTemplate)
            continue;

        if (auto linkedState = GetWorldState(stateTemplate, state->InstanceID))
            AddToWorldStateSet(&stateSet, linkedState);
    }
    return stateSet;
}

bool WorldStateMgr::CheckWorldState(uint32 variableID)
{
    return variableID >= WorldStatesData::Limits::Begin && variableID <= WorldStatesData::Limits::End;
}

uint32 WorldStateMgr::GetMapIdByZoneId(uint32 zoneID)
{
    if (!zoneID)
        return 0;

    for (auto const& areaEntry : sAreaTableStore)
        if (areaEntry->ParentAreaID == zoneID)
            return areaEntry->ContinentID;

    return 0;
}
