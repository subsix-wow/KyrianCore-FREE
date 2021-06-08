
#ifndef WORLD_STATE_H
#define WORLD_STATE_H

#include "Common.h"

namespace WorldStatesData
{
    enum Limits : uint16
    {
        Begin = 1000,
        End = 20000
    };

    enum Types : uint8
    {
        Custom = 0,
        World = 1,
        Weekly = 2,
        Event = 3,
        Map = 4,
        Zone = 5,
        Area = 6,
        Battlegound = 7,
        CapturePoint = 8,
        DestructibleObject = 9,
        Max
    };

    enum Flags : uint8
    {
        Initialized = 0,
        Active = 1,
        Saved = 2,
        Expired = 3,
        Updated = 4,
        Deleted = 7,
        Neutral = 8,
        Alliance = 9,
        Horde = 10,
        InitialState = 16,
        PassiaveAtCreate = 17,
        NotExpireable = 18,
        CustomFormat = 24,
        CustomGlobal = 25,
        CustomHidden = 26,
        CustomX = 27,
    };

    enum InitialValue : uint8
    {
        Remove = 0,
        Add = 1,
    };
}

struct WorldStateTemplate
{
    WorldStateTemplate(uint32 variableID, uint32 type, uint32 _condition, uint32 flags, uint32 defaultValue, uint32 linkedID);

    std::unordered_set<uint32> LinkedList;
    uint32 const VariableID;
    uint32 const VariableType;
    uint32 const ConditionID;
    uint32 const Flags;
    uint32 const DefaultValue;
    uint32 LinkedID;

    bool IsGlobal() const;
    bool HasFlag(WorldStatesData::Flags flag) const;
};

struct WorldState
{
    WorldState(WorldStateTemplate const* stateTemplate, uint32 instanceID);
    WorldState(WorldStateTemplate const* stateTemplate, uint32 instanceID, uint32 flags, uint32 value, time_t renewtime);
    WorldState(uint32 variableID, uint32 instanceID, uint32 flags, uint32 value, time_t renewtime);
    WorldState(uint32 variableID, uint32 instanceID, uint32 value);

    bool IsGlobal() const;

    void Initialize();

    void Reload();

    void AddClient(Player* player);
    void AddClient(ObjectGuid const& guid);

    bool HasClient(Player* player) const;
    bool HasClient(ObjectGuid const& guid) const;

    void RemoveClient(Player* player);
    void RemoveClient(ObjectGuid const& guid);

    bool IsExpired() const;

    WorldStateTemplate const* GetTemplate() const;

    bool HasDownLink() const;
    std::unordered_set<uint32> const* GetLinkedSet() const;

    void AddFlag(WorldStatesData::Flags flag);
    void RemoveFlag(WorldStatesData::Flags flag);
    bool HasFlag(WorldStatesData::Flags flag) const;

    void SetValue(uint32 value, bool hidden);

    WorldStateTemplate const* StateTemplate;
    GuidSet ClientGuids;
    ObjectGuid LinkedGuid;
    time_t RenewTime;
    uint32 const VariableID;
    uint32 const InstanceID;
    uint32 const Type;
    uint32 Flags;
    uint32 Value;
    uint32 ConditionID;
    bool Hidden;
};

class WorldStateSet
{
    WorldState* _worldStatesContainer[4084];

public:
    WorldStateSet();

    void add(WorldState const* state);

    WorldState* operator[](uint32 idx) const;

    uint32 Count;
};

typedef std::multimap<uint32 /*variableID*/, WorldStateTemplate> WorldStateTemplateMap;
typedef std::multimap<uint32 /*variableID*/, WorldState> WorldStateMap;

#endif
