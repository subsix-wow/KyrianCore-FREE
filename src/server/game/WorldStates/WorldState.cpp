#include "WorldState.h"

WorldStateTemplate::WorldStateTemplate(uint32 variableID, uint32 type, uint32 _condition, uint32 flags, uint32 defaultValue, uint32 linkedID) : VariableID(variableID), VariableType(type), ConditionID(_condition), Flags(flags), DefaultValue(defaultValue), LinkedID(linkedID)
{
}

bool WorldStateTemplate::IsGlobal() const
{
    return VariableType == WorldStatesData::Types::World || VariableType == WorldStatesData::Types::Weekly || VariableType == WorldStatesData::Types::Event;
}

bool WorldStateTemplate::HasFlag(WorldStatesData::Flags flag) const
{
    return Flags & 1 << flag;
}

bool WorldState::IsExpired() const
{
    return time(nullptr) > time_t(RenewTime + WEEK);
}

WorldStateTemplate const* WorldState::GetTemplate() const
{
    return StateTemplate;
}

bool WorldState::HasDownLink() const
{
    if (auto x = GetTemplate())
        return !x->LinkedList.empty();

    return false;
}

std::unordered_set<uint32> const* WorldState::GetLinkedSet() const
{
    if (auto x = GetTemplate())
        return &x->LinkedList;
    
    return nullptr;
}

void WorldState::AddFlag(WorldStatesData::Flags flag)
{
    Flags |= 1 << flag;
}

void WorldState::RemoveFlag(WorldStatesData::Flags flag)
{
    Flags &= ~(1 << flag);
}

bool WorldState::HasFlag(WorldStatesData::Flags flag) const
{
    return bool(Flags & 1 << flag);
}

WorldState::WorldState(WorldStateTemplate const* stateTemplate, uint32 instanceID) : StateTemplate(stateTemplate), VariableID(StateTemplate->VariableID), InstanceID(instanceID), Type(StateTemplate->VariableType), Hidden{false}
{
    Initialize();
}

WorldState::WorldState(WorldStateTemplate const* stateTemplate, uint32 instanceID, uint32 flags, uint32 value, time_t renewtime) : StateTemplate(stateTemplate), RenewTime(renewtime), VariableID(StateTemplate->VariableID), InstanceID(instanceID), Type(StateTemplate->VariableType), Flags(flags), Value(value), Hidden{ false }
{
    LinkedGuid.Clear();
    ClientGuids.clear();
    Reload();
}

WorldState::WorldState(uint32 variableID, uint32 instanceID, uint32 flags, uint32 value, time_t renewtime) : StateTemplate(nullptr), RenewTime(renewtime), VariableID(variableID), InstanceID(instanceID), Type(WorldStatesData::Types::Custom), Flags(flags), Value(value), Hidden{ false }
{
    Initialize();
}

WorldState::WorldState(uint32 variableID, uint32 instanceID, uint32 value) : StateTemplate(nullptr), VariableID(variableID), InstanceID(instanceID), Type(WorldStatesData::Types::Custom), Value(value), Hidden{ false }
{
    Initialize();
}

bool WorldState::IsGlobal() const
{
    return Type == WorldStatesData::Types::World || Type == WorldStatesData::Types::Weekly || Type == WorldStatesData::Types::Event || Type == WorldStatesData::Types::Custom &&
        HasFlag(WorldStatesData::Flags::CustomX) && HasFlag(WorldStatesData::Flags::CustomGlobal);
}

void WorldState::Initialize()
{
    LinkedGuid.Clear();
    ClientGuids.clear();

    if (StateTemplate)
    {
        ConditionID = StateTemplate->ConditionID;
        Flags = StateTemplate->Flags;
        Value = StateTemplate->DefaultValue;
    }
    else
    {
        ConditionID = 0;
        Flags = 0;
        Value = 0;
        AddFlag(WorldStatesData::Flags::CustomX);
    }

    AddFlag(WorldStatesData::Flags::Updated);
    RenewTime = time(nullptr);
}

void WorldState::Reload()
{
    if (!StateTemplate)
    {
        AddFlag(WorldStatesData::Flags::CustomX);
        return;
    }

    ConditionID = StateTemplate->ConditionID;
    Flags = StateTemplate->Flags;
    Value = StateTemplate->DefaultValue;
}

void WorldState::AddClient(Player* player)
{
    if (player)
        AddClient(player->GetGUID());
}

void WorldState::AddClient(ObjectGuid const& guid)
{
    if (guid.IsPlayer())
        ClientGuids.insert(guid);
}

bool WorldState::HasClient(Player* player) const
{
    if (player)
        return HasClient(player->GetGUID());

    return false;
}

bool WorldState::HasClient(ObjectGuid const& guid) const
{
    return ClientGuids.find(guid) != ClientGuids.end();
}

void WorldState::RemoveClient(Player* player)
{
    if (player)
        RemoveClient(player->GetGUID());
}

void WorldState::RemoveClient(ObjectGuid const& guid)
{
    if (guid.IsPlayer())
        ClientGuids.erase(guid);
}

void WorldState::SetValue(uint32 value, bool hidden)
{
    Value = value;
    Hidden = hidden;
    RenewTime = time(nullptr);

    RemoveFlag(WorldStatesData::Flags::Saved);
    AddFlag(WorldStatesData::Flags::Updated);
}

WorldStateSet::WorldStateSet() : _worldStatesContainer{}, Count(0)
{
}

void WorldStateSet::add(WorldState const* state)
{
    _worldStatesContainer[Count++] = const_cast<WorldState*>(state);
}

WorldState* WorldStateSet::operator[](uint32 idx) const
{
    return _worldStatesContainer[idx];
}
