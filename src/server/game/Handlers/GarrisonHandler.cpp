/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
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

#include "WorldSession.h"
#include "Garrison.h"
#include "GarrisonPackets.h"
#include "Player.h"

void WorldSession::HandleGetGarrisonInfo(WorldPackets::Garrison::GetGarrisonInfo& /*getGarrisonInfo*/)
{
    if (Garrison* garrison = _player->GetGarrison())
        garrison->SendInfo();
}

void WorldSession::HandleGarrisonPurchaseBuilding(WorldPackets::Garrison::GarrisonPurchaseBuilding& garrisonPurchaseBuilding)
{
    if (!_player->GetNPCIfCanInteractWith(garrisonPurchaseBuilding.NpcGUID, UNIT_NPC_FLAG_NONE, UNIT_NPC_FLAG_2_GARRISON_ARCHITECT))
        return;

    if (Garrison* garrison = _player->GetGarrison())
        garrison->PlaceBuilding(garrisonPurchaseBuilding.PlotInstanceID, garrisonPurchaseBuilding.BuildingID);
}

void WorldSession::HandleGarrisonCancelConstruction(WorldPackets::Garrison::GarrisonCancelConstruction& garrisonCancelConstruction)
{
    if (!_player->GetNPCIfCanInteractWith(garrisonCancelConstruction.NpcGUID, UNIT_NPC_FLAG_NONE, UNIT_NPC_FLAG_2_GARRISON_ARCHITECT))
        return;

    if (Garrison* garrison = _player->GetGarrison())
        garrison->CancelBuildingConstruction(garrisonCancelConstruction.PlotInstanceID);
}

void WorldSession::HandleGarrisonRequestBlueprintAndSpecializationData(WorldPackets::Garrison::GarrisonRequestBlueprintAndSpecializationData& /*garrisonRequestBlueprintAndSpecializationData*/)
{
    if (Garrison* garrison = _player->GetGarrison())
        garrison->SendBlueprintAndSpecializationData();
}

void WorldSession::HandleGarrisonGetMapData(WorldPackets::Garrison::GarrisonGetMapData& /*garrisonGetMapData*/)
{
    if (Garrison* garrison = _player->GetGarrison())
        garrison->SendMapData(_player);
}

void WorldSession::HandleCheckIsAdventureMapPoiValide(WorldPackets::Garrison::CheckIsAdventureMapPoiValide& packet)
{
    AdventureMapPOIEntry const* poiEntry = sAdventureMapPOIStore.LookupEntry(packet.ID);
    if (!poiEntry)
        return;

    bool active = true;
    if (poiEntry->PlayerConditionID)
        active = active && _player->MeetPlayerCondition(poiEntry->PlayerConditionID);

    switch (packet.ID)
    {
    case 40: // Zuldazar
        if (_player->GetTeam() == HORDE)
        {
            active = true;
        }
        break;
    case 41:
        if (_player->GetTeam() == HORDE)
        {
            active = true;
        }
        break;
    case 42:
        if (_player->GetTeam() == HORDE)
        {
            active = true;
        }
        break;
    case 148:
        if (_player->GetTeam() == HORDE)
        {
            active = true;
        }
        break;
    case 149:
        if (_player->GetTeam() == HORDE)
        {
            active = true;
        }
        break;
    case 150:
        if (_player->GetTeam() == HORDE)
        {
            active = true;
        }
        break;
    case 43: // Tiragarde Sound
        if (_player->GetTeam() == ALLIANCE)
        {
            active = true;
        }
        break;
    case 44:
        if (_player->GetTeam() == ALLIANCE)
        {
            active = true;
        }
        break;
    case 45:
        if (_player->GetTeam() == ALLIANCE)
        {
            active = true;
        }
        break;
    case 151:
        if (_player->GetTeam() == ALLIANCE)
        {
            active = true;
        }
        break;
    case 152:
        if (_player->GetTeam() == ALLIANCE)
        {
            active = true;
        }
        break;
    case 153:
        if (_player->GetTeam() == ALLIANCE)
        {
            active = true;
        }
        break;
    default:
        break;
    }

    if (poiEntry->QuestID)
        if (Quest const* quest = sObjectMgr->GetQuestTemplate(poiEntry->QuestID))
            active = active && _player->CanTakeQuest(quest, false);

    WorldPackets::Garrison::PlayerIsAdventureMapPoiValid result;
    result.ID = packet.ID;
    result.Active = active;
    SendPacket(result.Write());
}

void WorldSession::HandleGarrisonCreateShipmentOpcode(WorldPackets::Garrison::GarrisonCreateShipment& createShipment)
{
    printf("HandleGarrisonCreateShipmentOpcode npcId=%d \n", createShipment.NpcGUID.GetEntry());
    if (!createShipment.Count)
        createShipment.Count = 1;
    if (!_player)
        return;

    //Creature* unit(); //= GetPlayer()->GetNPCIfCanInteractWith(createShipment.NpcGUID, UNIT_NPC_FLAG_SHIPMENT_CRAFTER);
    //if (!unit)
   // return;

    WorldPackets::Garrison::GarrisonCreateShipmentResponse result;
    result.ShipmentID = 0;
    result.ShipmentRecID = 0;
    result.Result = 1;
    SendPacket(result.Write());
}

void WorldSession::HandleGarrisonAssignFollowerToBuilding(WorldPackets::Garrison::GarrisonAssignFollowerToBuilding& packet)
{
}

void WorldSession::HandleGarrisonGenerateRecruits(WorldPackets::Garrison::GarrisonGenerateRecruits& packet)
{
}

void WorldSession::HandleGarrisonCheckUpgradeable(WorldPackets::Garrison::GarrisonCheckUpgradeable& /*garrisonCheckUpgradeable*/)
{
    bool canUpgrade = false;
  //  if (Garrison* garrison = _player->GetGarrison()) //GARRISON_TYPE_GARRISON
  //      canUpgrade = garrison->ToWodGarrison()->CanUpgrade(false);

    SendPacket(WorldPackets::Garrison::GarrisonCheckUpgradeableResult(canUpgrade).Write());
}

void WorldSession::HandleGarrisonCompleteMission(WorldPackets::Garrison::GarrisonCompleteMission& completeMission)
{
    if (!_player->GetNPCIfCanInteractWith(completeMission.NpcGUID, UNIT_NPC_FLAG_NONE, UNIT_NPC_FLAG_2_GARRISON_MISSION_NPC))
        return;

    GarrMissionEntry const* missionEntry = sGarrMissionStore.LookupEntry(completeMission.MissionID);
    if (!missionEntry)
        return;

 /*  Garrison* garrison = _player->GetGarrison(GarrisonType(missionEntry->GarrTypeID));
    if (!garrison)
        return; TODO
        */
    //    garrison->CompleteMission(completeMission.MissionID);
}

void WorldSession::HandleGarrisonOpenMissionNpc(WorldPackets::Garrison::GarrisonOpenMissionNpcClient& garrisonOpenMissionNpcClient)
{
    Creature* adventureMap = _player->GetNPCIfCanInteractWith(garrisonOpenMissionNpcClient.NpcGUID, UNIT_NPC_FLAG_NONE, UNIT_NPC_FLAG_2_GARRISON_MISSION_NPC);
    if (!adventureMap)
        return;

    GarrisonType garType = GARRISON_TYPE_CLASS_ORDER; // Todo : differenciate depending of NPC

    switch (garrisonOpenMissionNpcClient.NpcGUID.GetEntry())
    {
    case 80432:
    case 81546:
        garType = GARRISON_TYPE_GARRISON;
        break;
    default:
        garType = GARRISON_TYPE_CLASS_ORDER;
        break;
    }

    uint32 uiMapId = sObjectMgr->GetAdventureMapUIByCreature(adventureMap->GetEntry());

    if (uiMapId)
    {
        SendPacket(WorldPackets::Garrison::ShowAdventureMap(garrisonOpenMissionNpcClient.NpcGUID, uiMapId).Write());
    }
    else
    {
        if (Garrison const* garrison = _player->GetGarrison())
        {
            WorldPackets::Garrison::GarrisonOpenMissionNpc garrisonOpenMissionNpc;
          //  for (auto const& p : garrison->GetMissions())
            {
           //     garrisonOpenMissionNpc.Missions.push_back(p.first);
            }
            SendPacket(garrisonOpenMissionNpc.Write());
        }
    }
}

void WorldSession::HandleGetLandingPageShipmentsOpcode(WorldPackets::Garrison::GarrisonGetLandingPageShipments& /*garrisonGetLandingPageShipments*/)
{
    if (!_player)
        return;

    Garrison* garrison = _player->GetGarrison(/*GarrisonType(GarrisonType::GARRISON_TYPE_CLASS_ORDER)*/);
    if (!garrison)
        return;
    // TODO rewrite
    WorldPackets::Garrison::GarrisonGetLandingPageShipmentsResponse packet;
    packet.Result = GarrisonType::GARRISON_TYPE_CLASS_ORDER;
    _player->SendDirectMessage(packet.Write());
}

void WorldSession::HandleUpgradeGarrison(WorldPackets::Garrison::UpgradeGarrison& packet)
{
    if (!_player->GetNPCIfCanInteractWith(packet.NpcGUID, UNIT_NPC_FLAG_NONE, UNIT_NPC_FLAG_2_GARRISON_ARCHITECT))
        return;

    if (Garrison* garrison = _player->GetGarrison())
        garrison->Upgrade();
}

void WorldSession::HandleTrophyData(WorldPackets::Garrison::TrophyData& packet)
{
    switch (packet.GetOpcode())
    {
    case CMSG_REPLACE_TROPHY:
        break;
    case CMSG_CHANGE_MONUMENT_APPEARANCE:
        break;
    default:
        break;
    }
}

void WorldSession::HandleGetTrophyList(WorldPackets::Garrison::GetTrophyList& packet)
{
}

void WorldSession::HandleGarrisonMissionBonusRoll(WorldPackets::Garrison::GarrisonMissionBonusRoll& packet)
{
    if (!_player->GetNPCIfCanInteractWith(packet.NpcGUID, UNIT_NPC_FLAG_NONE, UNIT_NPC_FLAG_2_GARRISON_MISSION_NPC /*| UNIT_NPC_FLAG_2_SHIPMENT_CRAFTER*/))
        return;

    auto const& garrison = _player->GetGarrison();
    if (!garrison)
        return;

  /*  auto const& mission = garrison->GetMissionByRecID(packet.MissionRecID);
    if (!mission)
        return;*/

    //auto canBonusRoll([&mission]() -> bool
    /*{
       // if (!sGarrMissionStore.LookupEntry(mission->PacketInfo.RecID))
            return false;

        if (mission->PacketInfo.State != MISSION_STATE_WAITING_BONUS && mission->PacketInfo.State != MISSION_STATE_WAITING_OWERMAX_BONUS)
            return false;

        if (mission->PacketInfo.StartTime + mission->PacketInfo.Duration > time(nullptr))
            return false;

        return true;
    });*/

   // if (canBonusRoll())
    //    mission->BonusRoll(_player);
    else
    {
        WorldPackets::Garrison::GarrisonMissionBonusRollResult res;
       // res.MissionData = mission->PacketInfo;
       // res.MissionRecID = mission->PacketInfo.RecID;
        res.Result = GARRISON_SUCCESS; // which code thre?
        _player->SendDirectMessage(res.Write());
    }
}

void WorldSession::HandleGarrisonRecruitFollower(WorldPackets::Garrison::GarrisonRecruitFollower& packet)
{
    if (_player == nullptr)
        return;

    Garrison* garrison = _player->GetGarrison(/*GarrisonType::GARRISON_TYPE_CLASS_HALL*/);

    if (!garrison)
        return;

    WorldPackets::Garrison::GarrisonRecruitFollowerResult result;
   // std::unordered_map<uint64 /*dbId*/, Garrison::Follower> followers = garrison->GetFollowers();

    //  if (Creature* unit = _player->GetNPCIfCanInteractWith(garrisonRecruitsFollower.NpcGUID, 0))
    {
        result.resultID = uint32(GarrisonError::GARRISON_SUCCESS);

       // for (auto& follower : followers)
    //    {
          /*  if (follower.second.PacketInfo.GarrFollowerID == garrisonRecruitsFollower.FollowerID)
            {
                result.followers.push_back(follower.second.PacketInfo);
                garrison->AddFollower(garrisonRecruitsFollower.FollowerID);
                //l_Garrison->SetCanRecruitFollower(false);
                //m_Player->SetCharacterWorldState(CharacterWorldStates::GarrisonTavernBoolCanRecruitFollower, 0);
                break;
            }*/
      //  }
        SendPacket(result.Write());
    }
}

