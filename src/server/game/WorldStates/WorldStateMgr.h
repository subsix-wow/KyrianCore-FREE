
#ifndef WORLD_STATE_MGR_H
#define WORLD_STATE_MGR_H

#include "Common.h"
#include "WorldState.h"
#include "FunctionProcessor.h"

class Player;

inline void AddToWorldStateSet(WorldStateSet** worldStateSet, WorldState const* state);

class WorldStateMgr
{
public:
    WorldStateMgr();
    ~WorldStateMgr();

    void Update(uint32 diff);

    static WorldStateMgr& Instance();
    void Initialize();

    void LoadTemplatesFromDB();
    void LoadFromDB();

    void SaveToDB();

    void SendWorldStateUpdate(Player* target, uint32 variableID, uint32 value, bool hidden);

    void CreateWorldStatesIfNeed();
    void CreateZoneAreaStateIfNeed(Player* player, uint32 zoneID, uint32 areaID);

    void CreateInstanceState(uint32 mapID, uint32 instanceID);
    void DeleteInstanceState(uint32 mapID, uint32 instanceID);

    void MapUpdate(Map* map);

    WorldState const* CreateWorldState(WorldStateTemplate const* stateTemplate, uint32 instanceID, uint32 value = std::numeric_limits<uint32>().max());
    WorldState const* CreateWorldState(uint32 variableID, uint32 instanceID, uint32 value = std::numeric_limits<uint32>().max());
    WorldState const* GetWorldState(uint32 variableID, uint32 instanceID, WorldStatesData::Types type, uint32 conditionID);
    WorldState const* GetWorldState(uint32 variableID, uint32 instanceID, Player* player = nullptr);
    WorldState const* GetWorldState(WorldStateTemplate const* stateTemplate, uint32 instanceID);
    void DeleteWorldState(WorldState* state);
    WorldStateTemplate const* FindTemplate(uint32 variableID, uint32 type = WorldStatesData::Types::Max, uint32 conditionID = 0, uint32 linkedID = 0);
    void AddTemplate(uint32 ID, WorldStateTemplate& ws);

    uint32 GetWorldStateValue(uint32 variableID);
    uint32 GetWorldStateValueFor(Player* player, uint32 variableID);
    uint32 GetWorldStateValueFor(Map* map, uint32 variableID);
    uint32 GetWorldStateValueFor(uint32 zoneID, uint32 variableID);
    uint32 GetWorldStateValueFor(uint32 mapID, uint32 instanceID, uint32 zoneID, uint32 areaID, uint32 variableID);
    uint32 GetWorldStateValueFor(WorldObject* object, uint32 variableID);

    void SetWorldStateValueFor(Player* player, uint32 variableID, uint32 value, bool hidden = false);
    void SetWorldStateValueFor(Map* map, uint32 variableID, uint32 value, bool hidden = false);
    void SetWorldStateValueFor(WorldObject* object, uint32 variableID, uint32 value, bool hidden = false);
    void SetWorldStateValueFor(uint32 zoneID, uint32 variableID, uint32 value, bool hidden = false);

    WorldStateSet* GetWorldStatesFor(Player* player, WorldStatesData::Flags flag);
    WorldStateSet* GetWorldStatesFor(Player* player, uint32 flags = std::numeric_limits<uint32>().max());

    WorldStateSet* GetUpdatedWorldStatesFor(Player* player, time_t lastUpdateTime = 0);

    WorldStateSet* GetInstanceStates(Map* map, uint32 flags = 0, bool full = false);
    WorldStateSet* GetInstanceStates(uint32 mapID, uint32 instanceID, uint32 flags = 0, bool full = false);
    WorldStateSet* GetInitWorldStates(uint32 mapID, uint32 instanceID, uint32 zoneID, uint32 areaID);

    static bool IsFitToCondition(Player* player, WorldState const* state);
    bool IsFitToCondition(Map* map, WorldState const* state) const;
    static bool IsFitToCondition(uint32 mapID, uint32 instanceID, uint32 zoneID, uint32 areaID, WorldState const* state);

    void AddWorldStateFor(Player* player, uint32 variableID, uint32 instanceID);
    void RemoveWorldStateFor(Player* player, uint32 variableID, uint32 instanceID);

    WorldStateSet* GetDownLinkedWorldStates(WorldState const* state);

    static bool CheckWorldState(uint32 variableID);

    static uint32 GetMapIdByZoneId(uint32 zoneID);

private:
    WorldStateTemplateMap _worldStateTemplates;
    WorldStateMap _worldState;
    FunctionProcessor _delayedOperations;
};

#define sWorldStateMgr WorldStateMgr::Instance()

#endif
