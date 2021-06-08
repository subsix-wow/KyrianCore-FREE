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

#include "GarrisonPackets.h"
#include "DB2Structure.h"

namespace WorldPackets
{
namespace Garrison
{
WorldPacket const* GarrisonCreateResult::Write()
{
    _worldPacket << uint32(Result);
    _worldPacket << uint32(GarrSiteLevelID);

    return &_worldPacket;
}

WorldPacket const* GarrisonDeleteResult::Write()
{
    _worldPacket << uint32(Result);
    _worldPacket << uint32(GarrSiteID);

    return &_worldPacket;
}

ByteBuffer& operator<<(ByteBuffer& data, GarrisonPlotInfo& plotInfo)
{
    data << uint32(plotInfo.GarrPlotInstanceID);
    data << plotInfo.PlotPos;
    data << uint32(plotInfo.PlotType);

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, GarrisonBuildingInfo const& buildingInfo)
{
    data << uint32(buildingInfo.GarrPlotInstanceID);
    data << uint32(buildingInfo.GarrBuildingID);
    data << buildingInfo.TimeBuilt;
    data << uint32(buildingInfo.CurrentGarSpecID);
    data << buildingInfo.TimeSpecCooldown;
    data.WriteBit(buildingInfo.Active);
    data.FlushBits();

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, GarrisonFollower const& follower)
{
    data << uint64(follower.DbID);
    data << uint32(follower.GarrFollowerID);
    data << uint32(follower.Quality);
    data << uint32(follower.FollowerLevel);
    data << uint32(follower.ItemLevelWeapon);
    data << uint32(follower.ItemLevelArmor);
    data << uint32(follower.Xp);
    data << uint32(follower.Durability);
    data << uint32(follower.CurrentBuildingID);
    data << uint32(follower.CurrentMissionID);
    data << uint32(follower.AbilityID.size());
    data << uint32(follower.ZoneSupportSpellID);
    data << uint32(follower.FollowerStatus);
    data << int32(follower.Health);
    data << int8(follower.BoardIndex);
    data << follower.HealingTimestamp;
    for (GarrAbilityEntry const* ability : follower.AbilityID)
        data << uint32(ability->ID);

    data.WriteBits(follower.CustomName.length(), 7);
    data.FlushBits();
    data.WriteString(follower.CustomName);

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, GarrisonMission const& mission)
{
    data << uint64(mission.DbID);
    data << mission.OfferTime;
    data << mission.OfferDuration;
    data << mission.StartTime;
    data << mission.TravelDuration;
    data << mission.MissionDuration;
    data << uint32(mission.MissionRecID);
    data << uint32(mission.MissionState);
    data << uint32(mission.SuccessChance);
    data << uint32(mission.Flags);
    data << float(mission.MissionScalar);

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, GarrisonMissionReward const& missionRewardItem)
{
    data << int32(missionRewardItem.ItemID);
    data << uint32(missionRewardItem.ItemQuantity);
    data << int32(missionRewardItem.CurrencyID);
    data << uint32(missionRewardItem.CurrencyQuantity);
    data << uint32(missionRewardItem.FollowerXP);
    data << uint32(missionRewardItem.GarrMssnBonusAbilityID);
    data << int32(missionRewardItem.ItemFileDataID);
    data.WriteBit(missionRewardItem.ItemInstance.is_initialized());
    data.FlushBits();

    if (missionRewardItem.ItemInstance)
        data << *missionRewardItem.ItemInstance;

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, GarrisonMissionBonusAbility const& areaBonus)
{
    data << areaBonus.StartTime;
    data << uint32(areaBonus.GarrMssnBonusAbilityID);

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, GarrisonTalentSocketData const& talentSocketData)
{
    data << int32(talentSocketData.SoulbindConduitID);
    data << int32(talentSocketData.SoulbindConduitRank);

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, GarrisonTalent const& talent)
{
    data << int32(talent.GarrTalentID);
    data << int32(talent.Rank);
    data << talent.ResearchStartTime;
    data << int32(talent.Flags);
    data.WriteBit(talent.Socket.is_initialized());
    data.FlushBits();

    if (talent.Socket)
        data << *talent.Socket;

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, GarrisonCollectionEntry const& collectionEntry)
{
    data << int32(collectionEntry.EntryID);
    data << int32(collectionEntry.Rank);

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, GarrisonCollection const& collection)
{
    data << int32(collection.Type);
    data << uint32(collection.Entries.size());
    for (GarrisonCollectionEntry const& collectionEntry : collection.Entries)
        data << collectionEntry;

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, GarrisonEventEntry const& event)
{
    data << int64(event.EventValue);
    data << int32(event.EntryID);

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, GarrisonEventList const& eventList)
{
    data << int32(eventList.Type);
    data << uint32(eventList.Events.size());
    for (GarrisonEventEntry const& event : eventList.Events)
        data << event;

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, GarrisonInfo const& garrison)
{
    ASSERT(garrison.Missions.size() == garrison.MissionRewards.size());
    ASSERT(garrison.Missions.size() == garrison.MissionOvermaxRewards.size());
    ASSERT(garrison.Missions.size() == garrison.CanStartMission.size());

    data << int32(garrison.GarrTypeID);
    data << int32(garrison.GarrSiteID);
    data << int32(garrison.GarrSiteLevelID);
    data << uint32(garrison.Buildings.size());
    data << uint32(garrison.Plots.size());
    data << uint32(garrison.Followers.size());
    data << uint32(garrison.AutoTroops.size());
    data << uint32(garrison.Missions.size());
    data << uint32(garrison.MissionRewards.size());
    data << uint32(garrison.MissionOvermaxRewards.size());
    data << uint32(garrison.MissionAreaBonuses.size());
    data << uint32(garrison.Talents.size());
    data << uint32(garrison.Collections.size());
    data << uint32(garrison.EventLists.size());
    data << uint32(garrison.CanStartMission.size());
    data << uint32(garrison.ArchivedMissions.size());
    data << int32(garrison.NumFollowerActivationsRemaining);
    data << uint32(garrison.NumMissionsStartedToday);
    data << int32(garrison.MinAutoTroopLevel);

    for (GarrisonPlotInfo* plot : garrison.Plots)
        data << *plot;

    for (GarrisonMission const* mission : garrison.Missions)
        data << *mission;

    for (std::vector<GarrisonMissionReward> const& missionReward : garrison.MissionRewards)
        data << uint32(missionReward.size());

    for (std::vector<GarrisonMissionReward> const& missionReward : garrison.MissionOvermaxRewards)
        data << uint32(missionReward.size());

    for (GarrisonMissionBonusAbility const* areaBonus : garrison.MissionAreaBonuses)
        data << *areaBonus;

    for (GarrisonCollection const& collection : garrison.Collections)
        data << collection;

    for (GarrisonEventList const& eventList : garrison.EventLists)
        data << eventList;

    if (!garrison.ArchivedMissions.empty())
        data.append(garrison.ArchivedMissions.data(), garrison.ArchivedMissions.size());

    for (GarrisonBuildingInfo const* building : garrison.Buildings)
        data << *building;

    for (bool canStartMission : garrison.CanStartMission)
        data.WriteBit(canStartMission);

    data.FlushBits();

    for (GarrisonFollower const* follower : garrison.Followers)
        data << *follower;

    for (GarrisonFollower const* follower : garrison.AutoTroops)
        data << *follower;

    for (GarrisonTalent const& talent : garrison.Talents)
        data << talent;

    for (std::vector<GarrisonMissionReward> const& missionReward : garrison.MissionRewards)
        for (GarrisonMissionReward const& missionRewardItem : missionReward)
            data << missionRewardItem;

    for (std::vector<GarrisonMissionReward> const& missionReward : garrison.MissionOvermaxRewards)
        for (GarrisonMissionReward const& missionRewardItem : missionReward)
            data << missionRewardItem;

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, FollowerSoftCapInfo const& followerSoftCapInfo)
{
    data << int32(followerSoftCapInfo.GarrFollowerTypeID);
    data << uint32(followerSoftCapInfo.Count);
    return data;
}

WorldPacket const* GetGarrisonInfoResult::Write()
{
    _worldPacket << int32(FactionIndex);
    _worldPacket << uint32(Garrisons.size());
    _worldPacket << uint32(FollowerSoftCaps.size());
    for (FollowerSoftCapInfo const& followerSoftCapInfo : FollowerSoftCaps)
        _worldPacket << followerSoftCapInfo;

    for (GarrisonInfo const& garrison : Garrisons)
        _worldPacket << garrison;

    return &_worldPacket;
}

ByteBuffer& operator<<(ByteBuffer& data, GarrisonRemoteBuildingInfo const& building)
{
    data << uint32(building.GarrPlotInstanceID);
    data << uint32(building.GarrBuildingID);

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, GarrisonRemoteSiteInfo const& site)
{
    data << uint32(site.GarrSiteLevelID);
    data << uint32(site.Buildings.size());
    for (GarrisonRemoteBuildingInfo const& building : site.Buildings)
        data << building;

    return data;
}

WorldPacket const* GarrisonRemoteInfo::Write()
{
    _worldPacket << uint32(Sites.size());
    for (GarrisonRemoteSiteInfo const& site : Sites)
        _worldPacket << site;

    return &_worldPacket;
}

void GarrisonPurchaseBuilding::Read()
{
    _worldPacket >> NpcGUID;
    _worldPacket >> PlotInstanceID;
    _worldPacket >> BuildingID;
}

WorldPacket const* GarrisonPlaceBuildingResult::Write()
{
    _worldPacket << int32(GarrTypeID);
    _worldPacket << uint32(Result);
    _worldPacket << BuildingInfo;
    _worldPacket.WriteBit(PlayActivationCinematic);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

void GarrisonCancelConstruction::Read()
{
    _worldPacket >> NpcGUID;
    _worldPacket >> PlotInstanceID;
}

WorldPacket const* GarrisonBuildingRemoved::Write()
{
    _worldPacket << int32(GarrTypeID);
    _worldPacket << uint32(Result);
    _worldPacket << uint32(GarrPlotInstanceID);
    _worldPacket << uint32(GarrBuildingID);

    return &_worldPacket;
}

WorldPacket const* GarrisonLearnBlueprintResult::Write()
{
    _worldPacket << int32(GarrTypeID);
    _worldPacket << uint32(Result);
    _worldPacket << uint32(BuildingID);

    return &_worldPacket;
}

WorldPacket const* GarrisonUnlearnBlueprintResult::Write()
{
    _worldPacket << int32(GarrTypeID);
    _worldPacket << uint32(Result);
    _worldPacket << uint32(BuildingID);

    return &_worldPacket;
}

WorldPacket const* GarrisonRequestBlueprintAndSpecializationDataResult::Write()
{
    _worldPacket << int32(GarrTypeID);
    _worldPacket << uint32(BlueprintsKnown ? BlueprintsKnown->size() : 0);
    _worldPacket << uint32(SpecializationsKnown ? SpecializationsKnown->size() : 0);
    if (BlueprintsKnown)
        for (uint32 blueprint : *BlueprintsKnown)
            _worldPacket << uint32(blueprint);

    if (SpecializationsKnown)
        for (uint32 specialization : *SpecializationsKnown)
            _worldPacket << uint32(specialization);

    return &_worldPacket;
}

ByteBuffer& operator<<(ByteBuffer& data, GarrisonBuildingMapData& building)
{
    data << uint32(building.GarrBuildingPlotInstID);
    data << building.Pos;

    return data;
}

WorldPacket const* GarrisonMapDataResponse::Write()
{
    _worldPacket << uint32(Buildings.size());
    for (GarrisonBuildingMapData& landmark : Buildings)
        _worldPacket << landmark;

    return &_worldPacket;
}

WorldPacket const* GarrisonPlotPlaced::Write()
{
    _worldPacket << int32(GarrTypeID);
    _worldPacket << *PlotInfo;

    return &_worldPacket;
}

WorldPacket const* GarrisonPlotRemoved::Write()
{
    _worldPacket << uint32(GarrPlotInstanceID);

    return &_worldPacket;
}

WorldPacket const* GarrisonAddFollowerResult::Write()
{
    _worldPacket << int32(GarrTypeID);
    _worldPacket << uint32(Result);
    _worldPacket << Follower;

    return &_worldPacket;
}

WorldPacket const* GarrisonRemoveFollowerResult::Write()
{
    _worldPacket << uint64(FollowerDBID);
    _worldPacket << int32(GarrTypeID);
    _worldPacket << uint32(Result);
    _worldPacket << uint32(Destroyed);

    return &_worldPacket;
}

WorldPacket const* GarrisonBuildingActivated::Write()
{
    _worldPacket << uint32(GarrPlotInstanceID);

    return &_worldPacket;
}

WorldPacket const* PlayerIsAdventureMapPoiValid::Write()
{
    _worldPacket << ID;
    _worldPacket.WriteBit(Active);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

void CheckIsAdventureMapPoiValide::Read()
{
    _worldPacket >> ID;
}

void CreateShipment::Read()
{
    _worldPacket >> NpcGUID >> Count;
}

WorldPacket const* CreateShipmentResponse::Write()
{
    _worldPacket << ShipmentID;
    _worldPacket << ShipmentRecID;
    _worldPacket << Result;

    return &_worldPacket;
}

void GarrisonCreateShipment::Read()
{
    _worldPacket >> NpcGUID;
    _worldPacket >> Count;
}

WorldPacket const* GarrisonCreateShipmentResponse::Write()
{
    _worldPacket << ShipmentID;
    _worldPacket << ShipmentRecID;
    _worldPacket << Result;
    return &_worldPacket;
}

void WorldPackets::Garrison::GarrisonAssignFollowerToBuilding::Read()
{
    _worldPacket >> NpcGUID;
    _worldPacket >> PlotInstanceID;
    _worldPacket >> FollowerDBID;
}

WorldPacket const* GarrisonCheckUpgradeableResult::Write()
{
    _worldPacket << uint32(!IsUpgradeable);

    return &_worldPacket;
}

void GarrisonCompleteMission::Read()
{
    _worldPacket >> NpcGUID;
    _worldPacket >> MissionID;
}

WorldPacket const* ShowAdventureMap::Write()
{
    _worldPacket << UnitGUID;
    _worldPacket << uint32(UiMapID);

    return &_worldPacket;
}

WorldPacket const* GarrisonAddMissionResult::Write()
{
    _worldPacket << GarrType;
    _worldPacket << Result;
    _worldPacket << State;
    _worldPacket << Mission;

    _worldPacket << uint32(Rewards.size());
    _worldPacket << uint32(BonusRewards.size());

    for (GarrisonMissionReward const& missionRewardItem : Rewards)
        _worldPacket << missionRewardItem;

    for (GarrisonMissionReward const& missionRewardItem : BonusRewards)
        _worldPacket << missionRewardItem;

    _worldPacket.WriteBit(Success);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* GarrisonCompleteMissionResult::Write()
{
    _worldPacket << Result;
    _worldPacket << Mission;
    _worldPacket << Mission.MissionRecID;
    _worldPacket << uint32(Followers.size());

    for (auto itr : Followers)
    {
        _worldPacket << itr.first;
        _worldPacket << itr.second;
    }

    _worldPacket.WriteBit(Succeed);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::ReplaceTrophyResponse::Write()
{
    _worldPacket.WriteBit(Success);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* GarrisonOpenShipmentNpcFromGossip::Write()
{
    _worldPacket << NpcGUID;
    _worldPacket << ShipmentContainerID;
    return &_worldPacket;
}

ByteBuffer& operator<<(ByteBuffer& data, Shipment const& shipment)
{
    data << shipment.ShipmentRecID;
    data << shipment.ShipmentID;
    data << shipment.FollowerDBID;
    data << uint32(shipment.CreationTime);
    data << shipment.ShipmentDuration;
    data << shipment.BuildingTypeID;

    return data;
}

WorldPacket const* GarrisonGetLandingPageShipmentsResponse::Write()
{
    _worldPacket << Result;
    _worldPacket << static_cast<uint32>(MessageData.size());
    for (auto const& map : MessageData)
        _worldPacket << map;

    return &_worldPacket;
}

WorldPacket const* GarrisonUpdateFollower::Write()
{
    _worldPacket << resultID;

    if (!followers.empty())
    {
        for (GarrisonFollower follower : followers)
        {
            InsertGarrisonFollower(_worldPacket, follower);
        }
    }

    return &_worldPacket;
}

WorldPacket InsertGarrisonFollower(WorldPacket& worldPacket, GarrisonFollower follower)
{
    worldPacket << follower.DbID;
    worldPacket << follower.GarrFollowerID;
    worldPacket << follower.Quality;
    worldPacket << follower.FollowerLevel;
    worldPacket << follower.ItemLevelWeapon;
    worldPacket << follower.ItemLevelArmor;
    worldPacket << follower.Xp;
    worldPacket << follower.Durability;
    worldPacket << follower.CurrentBuildingID;

    worldPacket << follower.CurrentMissionID;
    worldPacket << uint32(follower.AbilityID.size());
    worldPacket << follower.ZoneSupportSpellID;
    worldPacket << follower.FollowerStatus;

    for (auto it = follower.AbilityID.begin(); it != follower.AbilityID.end(); it++)
        worldPacket << int32(((const GarrAbilityEntry*)*it)->ID);

    worldPacket << follower.CustomName;
    return worldPacket;
}

WorldPacket const* GarrisonStartMissionResult::Write()
{
    _worldPacket << Result;
    _worldPacket << FailReason;
    _worldPacket << Mission;

    _worldPacket << Followers.size();
    for (uint64 followerDbID : Followers)
        _worldPacket << followerDbID;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::GarrisonRemoveFollowerFromBuildingResult::Write()
{
    _worldPacket << FollowerDBID;
    _worldPacket << Result;

    return &_worldPacket;
}

WorldPacket const* GarrisonRecruitFollowerResult::Write()
{
    _worldPacket << resultID;

    if (followers.empty())
    {
        for (uint8 l_Itr = 0; l_Itr < 3; ++l_Itr)
        {
            GarrisonFollower follower;
            InsertGarrisonFollower(_worldPacket, follower);
        }
    }
    else
    {
        for (GarrisonFollower follower : followers)
        {
            InsertGarrisonFollower(_worldPacket, follower);
        }
    }

    return &_worldPacket;
}

WorldPacket const* GarrisonOpenTalentNpc::Write()
{
    _worldPacket << NpcGUID;
    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::GarrisonOpenRecruitmentNpc::Write()
{
    _worldPacket << Guid;
    _worldPacket << GarrTypeID;
    _worldPacket << Result;
    for (auto const& follower : Followers)
        _worldPacket << follower;
    _worldPacket << CanRecruitFollower;
    _worldPacket << UnkBit;
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::GarrisonOpenMissionNpc::Write()
{
    _worldPacket << int32(GarrTypeID);
    _worldPacket << int32(Result);
    _worldPacket << static_cast<uint32>(Missions.size());
    for (auto const& missionID : Missions)
        _worldPacket << int32(missionID);

    _worldPacket.WriteBit(UnkBit1);
    _worldPacket.WriteBit(PreventXmlOpenMissionEvent);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::GarrisonOpenArchitect::Write()
{
    _worldPacket << NpcGUID;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::GarrisonMissionBonusRollResult::Write()
{
    _worldPacket << MissionData;
    _worldPacket << MissionRecID;
    _worldPacket << Result;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::GarrisonFollowerChangedXP::Write()
{
    _worldPacket << TotalXp;
    _worldPacket << Result;
    _worldPacket << Follower;
    _worldPacket << Follower2;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::GarrisonFollowerChangedItemLevel::Write()
{
    _worldPacket << Follower;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::GarrisonAssignFollowerToBuildingResult::Write()
{
    _worldPacket << FollowerDBID;
    _worldPacket << Result;
    _worldPacket << PlotInstanceID;

    return &_worldPacket;
}

WorldPacket const* GarrisonCompleteShipmentResponse::Write()
{
    _worldPacket << ShipmentID;
    _worldPacket << Result;
    return &_worldPacket;
}

void GarrisonOpenMissionNpcClient::Read()
{
    _worldPacket >> NpcGUID;
    _worldPacket >> GarrTypeID;
}

WorldPacket const* WorldPackets::Garrison::GetTrophyListResponse::Write()
{
    _worldPacket.WriteBit(Success);
    _worldPacket.FlushBits();

    _worldPacket << static_cast<uint32>(MsgData.size());
    for (auto const& map : MsgData)
    {
        _worldPacket << map.TrophyID;
        _worldPacket << map.Unk1;
        _worldPacket << map.Unk2;
    }

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Garrison::GetShipmentInfoResponse::Write()
{
    _worldPacket.WriteBit(Success);
    _worldPacket.FlushBits();

    _worldPacket << ShipmentID;
    _worldPacket << MaxShipments;
    _worldPacket << static_cast<uint32>(Shipments.size());
    _worldPacket << PlotInstanceID;
    for (auto const& map : Shipments)
        _worldPacket << map;

    return &_worldPacket;
}

void WorldPackets::Garrison::UpgradeGarrison::Read()
{
    _worldPacket >> NpcGUID;
}

void WorldPackets::Garrison::TrophyData::Read()
{
    _worldPacket >> TrophyGUID;
    _worldPacket >> NewTrophyID;
}

void WorldPackets::Garrison::GarrisonGenerateRecruits::Read()
{
    _worldPacket >> NpcGuid;
    _worldPacket >> MechanicTypeID;
    _worldPacket >> TraitID;
}

void WorldPackets::Garrison::GarrisonMissionBonusRoll::Read()
{
    _worldPacket >> NpcGUID;
    _worldPacket >> MissionRecID;
}

void WorldPackets::Garrison::GarrisonRecruitFollower::Read()
{
    _worldPacket >> Guid;
    _worldPacket >> FollowerIndex;
}

void WorldPackets::Garrison::GetTrophyList::Read()
{
    _worldPacket >> TrophyTypeID;
}
}
}
