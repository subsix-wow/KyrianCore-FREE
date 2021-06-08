/*
 * This file is part of TrinityCore Project. See AUTHORS file for Copyright information
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

#ifndef GarrisonPackets_h__
#define GarrisonPackets_h__

#include "Packet.h"
#include "ItemPacketsCommon.h"
#include "ObjectGuid.h"
#include "Optional.h"
#include "Position.h"
#include "PacketUtilities.h"
#include <list>
#include <unordered_set>
#include <vector>

struct GarrAbilityEntry;

namespace WorldPackets
{
    namespace Garrison
    {
        struct Shipment
        {
            Shipment() = default;
            uint64 FollowerDBID = 0;
            uint64 ShipmentID = 0;
            uint32 ShipmentRecID = 0;
            uint32 BuildingTypeID = 0;
            time_t CreationTime = time(nullptr);
            int32 ShipmentDuration = 0;

            bool finished = false;
            ObjectDBState DbState = DB_STATE_NEW;
            uint32 end = 0;
        };

        class GarrisonCreateResult final : public ServerPacket
        {
        public:
            GarrisonCreateResult() : ServerPacket(SMSG_GARRISON_CREATE_RESULT, 4 + 4) { }

            WorldPacket const* Write() override;

            uint32 GarrSiteLevelID = 0;
            uint32 Result = 0;
        };

        class GarrisonDeleteResult final : public ServerPacket
        {
        public:
            GarrisonDeleteResult() : ServerPacket(SMSG_GARRISON_DELETE_RESULT, 4 + 4) { }

            WorldPacket const* Write() override;

            uint32 Result = 0;
            uint32 GarrSiteID = 0;
        };

        class GetGarrisonInfo final : public ClientPacket
        {
        public:
            GetGarrisonInfo(WorldPacket&& packet) : ClientPacket(CMSG_GET_GARRISON_INFO, std::move(packet)) { }

            void Read() override { }
        };

        struct GarrisonPlotInfo
        {
            uint32 GarrPlotInstanceID = 0;
            TaggedPosition<Position::XYZO> PlotPos;
            uint32 PlotType = 0;
        };

        struct GarrisonBuildingInfo
        {
            uint32 GarrPlotInstanceID = 0;
            uint32 GarrBuildingID = 0;
            Timestamp<> TimeBuilt;
            uint32 CurrentGarSpecID = 0;
            Timestamp<> TimeSpecCooldown = time_t(2288912640);   // 06/07/1906 18:35:44 - another in the series of magic blizz dates
            bool Active = false;
        };

        struct GarrisonFollower
        {
            uint64 DbID = 0;
            uint32 GarrFollowerID = 0;
            uint32 Quality = 0;
            uint32 FollowerLevel = 0;
            uint32 ItemLevelWeapon = 0;
            uint32 ItemLevelArmor = 0;
            uint32 Xp = 0;
            uint32 Durability = 0;
            uint32 CurrentBuildingID = 0;
            uint32 CurrentMissionID = 0;
            std::list<GarrAbilityEntry const*> AbilityID;
            uint32 ZoneSupportSpellID = 0;
            uint32 FollowerStatus = 0;
            int32 Health = 0;
            Timestamp<> HealingTimestamp;
            int8 BoardIndex = 0;
            std::string CustomName;
        };

        struct GarrisonMission
        {
            uint64 DbID = 0;
            uint32 MissionRecID = 0;
            Timestamp<> OfferTime;
            Duration<Seconds> OfferDuration;
            Timestamp<> StartTime = time_t(2288912640);
            Duration<Seconds> TravelDuration;
            Duration<Seconds> MissionDuration;
            uint32 MissionState = 0;
            uint32 SuccessChance = 0;
            uint32 Flags = 0;
            float MissionScalar = 1.0f;
        };

        struct GarrisonMissionReward
        {
            int32 ItemID = 0;
            uint32 ItemQuantity = 0;
            int32 CurrencyID = 0;
            uint32 CurrencyQuantity = 0;
            uint32 FollowerXP = 0;
            uint32 GarrMssnBonusAbilityID = 0;
            int32 ItemFileDataID = 0;
            Optional<Item::ItemInstance> ItemInstance;
        };

        struct GarrisonMissionBonusAbility
        {
            uint32 GarrMssnBonusAbilityID = 0;
            Timestamp<> StartTime;
        };

        struct GarrisonTalentSocketData
        {
            int32 SoulbindConduitID = 0;
            int32 SoulbindConduitRank = 0;
        };

        struct GarrisonTalent
        {
            int32 GarrTalentID = 0;
            int32 Rank = 0;
            Timestamp<> ResearchStartTime;
            int32 Flags = 0;
            Optional<GarrisonTalentSocketData> Socket;
        };

        struct GarrisonCollectionEntry
        {
            int32 EntryID = 0;
            int32 Rank = 0;
        };

        struct GarrisonCollection
        {
            int32 Type = 0;
            std::vector<GarrisonCollectionEntry> Entries;
        };

        struct GarrisonEventEntry
        {
            int32 EntryID = 0;
            int64 EventValue = 0;
        };

        struct GarrisonEventList
        {
            int32 Type = 0;
            std::vector<GarrisonEventEntry> Events;
        };

        struct GarrisonInfo
        {
            int32 GarrTypeID = 0;
            uint32 GarrSiteID = 0;
            uint32 GarrSiteLevelID = 0;
            uint32 NumFollowerActivationsRemaining = 0;
            uint32 NumMissionsStartedToday = 0;   // might mean something else, but sending 0 here enables follower abilities "Increase success chance of the first mission of the day by %."
            int32 MinAutoTroopLevel = 0;
            std::vector<GarrisonPlotInfo*> Plots;
            std::vector<GarrisonBuildingInfo const*> Buildings;
            std::vector<GarrisonFollower const*> Followers;
            std::vector<GarrisonFollower const*> AutoTroops;
            std::vector<GarrisonMission const*> Missions;
            std::vector<std::vector<GarrisonMissionReward>> MissionRewards;
            std::vector<std::vector<GarrisonMissionReward>> MissionOvermaxRewards;
            std::vector<GarrisonMissionBonusAbility const*> MissionAreaBonuses;
            std::vector<GarrisonTalent> Talents;
            std::vector<GarrisonCollection> Collections;
            std::vector<GarrisonEventList> EventLists;
            std::vector<bool> CanStartMission;
            std::vector<int32> ArchivedMissions;
        };

        struct FollowerSoftCapInfo
        {
            int32 GarrFollowerTypeID;
            uint32 Count;
        };

        class GetGarrisonInfoResult final : public ServerPacket
        {
        public:
            GetGarrisonInfoResult() : ServerPacket(SMSG_GET_GARRISON_INFO_RESULT) { }

            WorldPacket const* Write() override;

            uint32 FactionIndex = 0;
            std::vector<GarrisonInfo> Garrisons;
            std::vector<FollowerSoftCapInfo> FollowerSoftCaps;
        };

        struct GarrisonRemoteBuildingInfo
        {
            GarrisonRemoteBuildingInfo() : GarrPlotInstanceID(0), GarrBuildingID(0) { }
            GarrisonRemoteBuildingInfo(uint32 plotInstanceId, uint32 buildingId) : GarrPlotInstanceID(plotInstanceId), GarrBuildingID(buildingId) { }

            uint32 GarrPlotInstanceID;
            uint32 GarrBuildingID;
        };

        struct GarrisonRemoteSiteInfo
        {
            uint32 GarrSiteLevelID = 0;
            std::vector<GarrisonRemoteBuildingInfo> Buildings;
        };

        class GarrisonRemoteInfo final : public ServerPacket
        {
        public:
            GarrisonRemoteInfo() : ServerPacket(SMSG_GARRISON_REMOTE_INFO) { }

            WorldPacket const* Write() override;

            std::vector<GarrisonRemoteSiteInfo> Sites;
        };

        class GarrisonPurchaseBuilding final : public ClientPacket
        {
        public:
            GarrisonPurchaseBuilding(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_PURCHASE_BUILDING, std::move(packet)) { }

            void Read() override;

            ObjectGuid NpcGUID;
            uint32 BuildingID = 0;
            uint32 PlotInstanceID = 0;
        };

        class GarrisonPlaceBuildingResult final : public ServerPacket
        {
        public:
            GarrisonPlaceBuildingResult() : ServerPacket(SMSG_GARRISON_PLACE_BUILDING_RESULT) { }

            WorldPacket const* Write() override;

            int32 GarrTypeID = 0;
            uint32 Result = 0;
            GarrisonBuildingInfo BuildingInfo;
            bool PlayActivationCinematic = false;
        };

        class GarrisonCancelConstruction final : public ClientPacket
        {
        public:
            GarrisonCancelConstruction(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_CANCEL_CONSTRUCTION, std::move(packet)) { }

            void Read() override;

            ObjectGuid NpcGUID;
            uint32 PlotInstanceID = 0;
        };

        class GarrisonBuildingRemoved final : public ServerPacket
        {
        public:
            GarrisonBuildingRemoved() : ServerPacket(SMSG_GARRISON_BUILDING_REMOVED, 4 + 4 + 4 + 4) { }

            WorldPacket const* Write() override;

            int32 GarrTypeID = 0;
            uint32 Result = 0;
            uint32 GarrPlotInstanceID = 0;
            uint32 GarrBuildingID = 0;
        };

        class GarrisonLearnBlueprintResult final : public ServerPacket
        {
        public:
            GarrisonLearnBlueprintResult() : ServerPacket(SMSG_GARRISON_LEARN_BLUEPRINT_RESULT, 4 + 4 + 4) { }

            WorldPacket const* Write() override;

            int32 GarrTypeID = 0;
            uint32 BuildingID = 0;
            uint32 Result = 0;
        };

        class GarrisonUnlearnBlueprintResult final : public ServerPacket
        {
        public:
            GarrisonUnlearnBlueprintResult() : ServerPacket(SMSG_GARRISON_UNLEARN_BLUEPRINT_RESULT, 4 + 4) { }

            WorldPacket const* Write() override;

            int32 GarrTypeID = 0;
            uint32 BuildingID = 0;
            uint32 Result = 0;
        };

        class GarrisonRequestBlueprintAndSpecializationData final : public ClientPacket
        {
        public:
            GarrisonRequestBlueprintAndSpecializationData(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_REQUEST_BLUEPRINT_AND_SPECIALIZATION_DATA, std::move(packet)) { }

            void Read() override { }
        };

        class GarrisonRequestBlueprintAndSpecializationDataResult final : public ServerPacket
        {
        public:
            GarrisonRequestBlueprintAndSpecializationDataResult() : ServerPacket(SMSG_GARRISON_REQUEST_BLUEPRINT_AND_SPECIALIZATION_DATA_RESULT, 400) { }

            WorldPacket const* Write() override;

            int32 GarrTypeID = 0;
            std::unordered_set<uint32> const* SpecializationsKnown = nullptr;
            std::unordered_set<uint32> const* BlueprintsKnown = nullptr;
        };

        class GarrisonGetMapData final : public ClientPacket
        {
        public:
            GarrisonGetMapData(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_GET_MAP_DATA, std::move(packet)) { }

            void Read() override { }
        };

        struct GarrisonBuildingMapData
        {
            GarrisonBuildingMapData() : GarrBuildingPlotInstID(0), Pos() { }
            GarrisonBuildingMapData(uint32 buildingPlotInstId, Position const& pos) : GarrBuildingPlotInstID(buildingPlotInstId), Pos(pos) { }

            uint32 GarrBuildingPlotInstID;
            TaggedPosition<Position::XYZ> Pos;
        };

        class GarrisonMapDataResponse final : public ServerPacket
        {
        public:
            GarrisonMapDataResponse() : ServerPacket(SMSG_GARRISON_MAP_DATA_RESPONSE) { }

            WorldPacket const* Write() override;

            std::vector<GarrisonBuildingMapData> Buildings;
        };

        class GarrisonPlotPlaced final : public ServerPacket
        {
        public:
            GarrisonPlotPlaced() : ServerPacket(SMSG_GARRISON_PLOT_PLACED) { }

            WorldPacket const* Write() override;

            int32 GarrTypeID = 0;
            GarrisonPlotInfo* PlotInfo = nullptr;
        };

        class GarrisonPlotRemoved final : public ServerPacket
        {
        public:
            GarrisonPlotRemoved() : ServerPacket(SMSG_GARRISON_PLOT_REMOVED, 4) { }

            WorldPacket const* Write() override;

            uint32 GarrPlotInstanceID = 0;
        };

        class GarrisonAddFollowerResult final : public ServerPacket
        {
        public:
            GarrisonAddFollowerResult() : ServerPacket(SMSG_GARRISON_ADD_FOLLOWER_RESULT, 8 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 5 * 4 + 4) { }

            WorldPacket const* Write() override;

            int32 GarrTypeID = 0;
            GarrisonFollower Follower;
            uint32 Result = 0;
        };

        class GarrisonRemoveFollowerResult final : public ServerPacket
        {
        public:
            GarrisonRemoveFollowerResult() : ServerPacket(SMSG_GARRISON_REMOVE_FOLLOWER_RESULT, 8 + 4 + 4 + 4) { }

            WorldPacket const* Write() override;

            uint64 FollowerDBID = 0;
            int32 GarrTypeID = 0;
            uint32 Result = 0;
            uint32 Destroyed = 0;
        };

        class GarrisonBuildingActivated final : public ServerPacket
        {
        public:
            GarrisonBuildingActivated() : ServerPacket(SMSG_GARRISON_BUILDING_ACTIVATED, 4) { }

            WorldPacket const* Write() override;

            uint32 GarrPlotInstanceID = 0;
        };

        class PlayerIsAdventureMapPoiValid final : public ServerPacket
        {
        public:
            PlayerIsAdventureMapPoiValid() : ServerPacket(SMSG_PLAYER_IS_ADVENTURE_MAP_POI_VALID, 5) { }

            WorldPacket const* Write() override;

            uint32 ID = 0;
            bool Active = true;
        };

        class CheckIsAdventureMapPoiValide final : public ClientPacket
        {
        public:
            CheckIsAdventureMapPoiValide(WorldPacket&& packet) : ClientPacket(CMSG_CHECK_IS_ADVENTURE_MAP_POI_VALID, std::move(packet)) { }

            void Read() override;

            uint32 ID = 0;
        };

        class CreateShipment final : public ClientPacket
        {
        public:
            CreateShipment(WorldPacket&& packet) : ClientPacket(CMSG_CREATE_SHIPMENT, std::move(packet)) { }

            void Read() override;

            ObjectGuid NpcGUID;
            uint32 Count = 0;
        };

        class CreateShipmentResponse final : public ServerPacket
        {
        public:
            CreateShipmentResponse() : ServerPacket(SMSG_CREATE_SHIPMENT_RESPONSE, 8 + 4 + 4) { }

            WorldPacket const* Write() override;

            uint64 ShipmentID = 0;
            uint32 ShipmentRecID = 0;
            uint32 Result = 0;
        };

        class GarrisonCreateShipment final : public ClientPacket
        {
        public:
            GarrisonCreateShipment(WorldPacket&& packet) : ClientPacket(CMSG_CREATE_SHIPMENT, std::move(packet)) { }

            void Read() override;

            ObjectGuid NpcGUID;
            uint32 Count = 1;
        };

        class GarrisonCreateShipmentResponse final : public ServerPacket
        {
        public:
            GarrisonCreateShipmentResponse() : ServerPacket(SMSG_CREATE_SHIPMENT_RESPONSE, 4) { }
            uint64 ShipmentID = 0;
            uint32 ShipmentRecID = 0;
            uint32 Result = 0;
            WorldPacket const* Write() override;
        };

        class GarrisonAssignFollowerToBuilding final : public ClientPacket
        {
        public:
            GarrisonAssignFollowerToBuilding(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_ASSIGN_FOLLOWER_TO_BUILDING, std::move(packet)) { }

            void Read() override;

            ObjectGuid NpcGUID;
            uint64 FollowerDBID = 0;
            uint32 PlotInstanceID = 0;
        };

        class GarrisonCheckUpgradeable final : public ClientPacket
        {
        public:
            GarrisonCheckUpgradeable(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_CHECK_UPGRADEABLE, std::move(packet)) { }

            void Read() override { }
        };

        class GarrisonCheckUpgradeableResult final : public ServerPacket
        {
        public:
            GarrisonCheckUpgradeableResult(bool upgradeable = false) : ServerPacket(SMSG_GARRISON_UPGRADE_RESULT, 4), IsUpgradeable(upgradeable) { }

            WorldPacket const* Write() override;

            bool IsUpgradeable = false;
        };

        class GarrisonCompleteMission final : public ClientPacket
        {
        public:
            GarrisonCompleteMission(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_COMPLETE_MISSION, std::move(packet)) { }

            void Read() override;

            ObjectGuid NpcGUID;
            uint32 MissionID = 0;
        };

        class TC_GAME_API ShowAdventureMap final : public ServerPacket
        {
        public:
            ShowAdventureMap(ObjectGuid guid, uint32 uiMapID) : ServerPacket(SMSG_ADVENTURE_MAP_OPEN_NPC, 20), UnitGUID(guid), UiMapID(uiMapID) { }

            WorldPacket const* Write() override;

            ObjectGuid UnitGUID;
            uint32 UiMapID = 0;
        };

        class GarrisonAddMissionResult final : public ServerPacket
        {
        public:
            GarrisonAddMissionResult() : ServerPacket(SMSG_GARRISON_ADD_MISSION_RESULT, 4) { }

            uint32 GarrType = 0;
            uint32 Result = 0;
            uint8 State = 0;
            GarrisonMission Mission;

            std::vector<GarrisonMissionReward> Rewards;
            std::vector<GarrisonMissionReward> BonusRewards;

            bool Success = true;

            WorldPacket const* Write() override;
        };

        class GarrisonCompleteMissionResult final : public ServerPacket
        {
        public:
            GarrisonCompleteMissionResult() : ServerPacket(SMSG_GARRISON_COMPLETE_MISSION_RESULT, 4) { }

            uint32 Result = 0;
            GarrisonMission Mission;
            std::map<uint64 /*followerDBID*/, uint32 /*unk*/> Followers;
            bool Succeed = false;

            WorldPacket const* Write() override;
        };

        class ReplaceTrophyResponse final : public ServerPacket
        {
        public:
            ReplaceTrophyResponse() : ServerPacket(SMSG_REPLACE_TROPHY_RESPONSE, 1) { }

            WorldPacket const* Write() override;

            bool Success = false;
        };

        class GarrisonOpenShipmentNpcFromGossip final : public ServerPacket
        {
        public:
            GarrisonOpenShipmentNpcFromGossip() : ServerPacket(SMSG_OPEN_SHIPMENT_NPC_FROM_GOSSIP, 4) { }

            ObjectGuid NpcGUID;
            uint32 ShipmentContainerID = 0;
            WorldPacket const* Write() override;
        };

        class GarrisonGetLandingPageShipmentsResponse final : public ServerPacket
        {
        public:
            GarrisonGetLandingPageShipmentsResponse() : ServerPacket(SMSG_GET_LANDING_PAGE_SHIPMENTS_RESPONSE, 4) { }

            WorldPacket const* Write() override;

            std::vector<Shipment> MessageData;
            uint32 Result = 0;
        };

        WorldPacket InsertGarrisonFollower(WorldPacket& worldPacke, WorldPackets::Garrison::GarrisonFollower follower);

        class GarrisonUpdateFollower final : public ServerPacket
        {
        public:
            GarrisonUpdateFollower() : ServerPacket(SMSG_GARRISON_UPDATE_FOLLOWER) { }

            WorldPacket const* Write() override;

            uint32 resultID = 0;
            std::vector <GarrisonFollower> followers;
        };

        class GarrisonStartMissionResult final : public ServerPacket
        {
        public:
            GarrisonStartMissionResult() : ServerPacket(SMSG_GARRISON_START_MISSION_RESULT, 4) { }

            uint32 Result = 0;
            uint16 FailReason = 1;
            GarrisonMission Mission;
            std::vector<uint64 /* dbID */> Followers;

            WorldPacket const* Write() override;
        };

        class GarrisonRemoveFollowerFromBuildingResult final : public ServerPacket
        {
        public:
            GarrisonRemoveFollowerFromBuildingResult() : ServerPacket(SMSG_GARRISON_REMOVE_FOLLOWER_FROM_BUILDING_RESULT, 12) { }

            WorldPacket const* Write() override;

            uint64 FollowerDBID = 0;
            int32 Result = 0;
        };

        class GarrisonRecruitFollowerResult final : public ServerPacket
        {
        public:
            GarrisonRecruitFollowerResult() : ServerPacket(SMSG_GARRISON_RECRUIT_FOLLOWER_RESULT, 64) { }

            uint32 resultID = 0;
            std::vector <GarrisonFollower> followers;

            WorldPacket const* Write() override;
        };

        class GarrisonOpenTalentNpc final : public ServerPacket
        {
        public:
            GarrisonOpenTalentNpc() : ServerPacket(SMSG_GARRISON_OPEN_TALENT_NPC, 4) { }

            ObjectGuid NpcGUID;

            WorldPacket const* Write() override;
        };

        class GarrisonOpenRecruitmentNpc final : public ServerPacket
        {
        public:
            GarrisonOpenRecruitmentNpc() : ServerPacket(SMSG_GARRISON_OPEN_RECRUITMENT_NPC, sizeof(GarrisonFollower) * 3 + 16 + 8 + 2) { }

            WorldPacket const* Write() override;

            std::vector<GarrisonFollower> Followers;
            ObjectGuid Guid;
            uint32 GarrTypeID = 0;
            uint32 Result = 0;
            bool CanRecruitFollower = false;
            bool UnkBit = false;
        };

        class GarrisonOpenMissionNpc final : public ServerPacket
        {
        public:
            GarrisonOpenMissionNpc() : ServerPacket(SMSG_GARRISON_OPEN_MISSION_NPC, 36 + 4 + 4) { }

            WorldPacket const* Write() override;

            std::vector<uint32> Missions;
            uint32 GarrTypeID = 0;
            uint32 Result = 0;
            bool UnkBit1 = false;
            bool PreventXmlOpenMissionEvent = false;
        };

        class GarrisonOpenArchitect final : public ServerPacket
        {
        public:
            GarrisonOpenArchitect() : ServerPacket(SMSG_GARRISON_OPEN_ARCHITECT, 16) { }

            WorldPacket const* Write() override;

            ObjectGuid NpcGUID;
        };

        class GarrisonMissionBonusRollResult final : public ServerPacket
        {
        public:
            GarrisonMissionBonusRollResult() : ServerPacket(SMSG_GARRISON_MISSION_BONUS_ROLL_RESULT, sizeof(GarrisonMission) + 4 + 4) { }

            WorldPacket const* Write() override;

            GarrisonMission MissionData;
            uint32 MissionRecID = 0;
            uint32 Result = 0;
        };


        class GarrisonFollowerChangedXP final : public ServerPacket
        {
        public:
            GarrisonFollowerChangedXP() : ServerPacket(SMSG_GARRISON_FOLLOWER_CHANGED_XP, sizeof(GarrisonFollower) * 2 + 8) { }

            WorldPacket const* Write() override;

            GarrisonFollower Follower;
            GarrisonFollower Follower2;
            int32 Result = 0;
            uint32 TotalXp = 0;
        };

        class GarrisonFollowerChangedItemLevel final : public ServerPacket
        {
        public:
            GarrisonFollowerChangedItemLevel() : ServerPacket(SMSG_GARRISON_FOLLOWER_CHANGED_ITEM_LEVEL, sizeof(GarrisonFollower)) { }

            WorldPacket const* Write() override;

            GarrisonFollower Follower;
        };


        class GarrisonAssignFollowerToBuildingResult final : public ServerPacket
        {
        public:
            GarrisonAssignFollowerToBuildingResult() : ServerPacket(SMSG_GARRISON_ASSIGN_FOLLOWER_TO_BUILDING_RESULT, 8 + 4 + 4) { }

            WorldPacket const* Write() override;

            uint64 FollowerDBID = 0;
            int32 Result = 0;
            int32 PlotInstanceID = 0;
        };

        class GarrisonCompleteShipmentResponse final : public ServerPacket
        {
        public:
            GarrisonCompleteShipmentResponse() : ServerPacket(SMSG_COMPLETE_SHIPMENT_RESPONSE, 4) { }
            uint64 ShipmentID = 0;
            uint32 Result = 0;

            WorldPacket const* Write() override;
        };

        class GarrisonOpenMissionNpcClient final : public ClientPacket
        {
        public:
            GarrisonOpenMissionNpcClient(WorldPacket&& packet) : ClientPacket(CMSG_OPEN_MISSION_NPC, std::move(packet)) { }

            void Read() override;

            ObjectGuid NpcGUID;
            int32 GarrTypeID = 0;
        };

        class GarrisonGetLandingPageShipments final : public ClientPacket
        {
        public:
            GarrisonGetLandingPageShipments(WorldPacket&& packet) : ClientPacket(CMSG_GET_LANDING_PAGE_SHIPMENTS, std::move(packet)) { }

            void Read() override { }
        };

        struct TrophyListInfo
        {
            uint32 TrophyID = 0;
            uint32 Unk1 = 0;
            uint32 Unk2 = 0;
        };

        class GetTrophyListResponse final : public ServerPacket
        {
        public:
            GetTrophyListResponse() : ServerPacket(SMSG_GET_TROPHY_LIST_RESPONSE, 1 + 4) { }

            WorldPacket const* Write() override;

            std::vector<TrophyListInfo> MsgData;
            bool Success = false;
        };

        class GetShipmentInfoResponse final : public ServerPacket
        {
        public:
            GetShipmentInfoResponse() : ServerPacket(SMSG_GET_SHIPMENT_INFO_RESPONSE, 1 + 4 + 4 + 4 + 4) { }

            WorldPacket const* Write() override;

            std::vector<Shipment> Shipments;
            uint32 ShipmentID = 0;
            uint32 MaxShipments = 0;
            uint32 PlotInstanceID = 0;
            bool Success = false;
        };

        class UpgradeGarrison final : public ClientPacket
        {
        public:
            UpgradeGarrison(WorldPacket&& packet) : ClientPacket(CMSG_UPGRADE_GARRISON, std::move(packet)) { }

            void Read() override;

            ObjectGuid NpcGUID;
        };

        //< CMSG_REPLACE_TROPHY
       //< CMSG_CHANGE_MONUMENT_APPEARANCE
        class TrophyData final : public ClientPacket
        {
        public:
            TrophyData(WorldPacket&& packet) : ClientPacket(std::move(packet)) { }

            void Read() override;

            ObjectGuid TrophyGUID;
            uint32 NewTrophyID = 0;
        };

        class GarrisonGenerateRecruits final : public ClientPacket
        {
        public:
            GarrisonGenerateRecruits(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_GENERATE_RECRUITS, std::move(packet)) { }

            void Read() override;

            ObjectGuid NpcGuid;
            int32 MechanicTypeID = 0;
            int32 TraitID = 0;
        };

        class GarrisonMissionBonusRoll final : public ClientPacket
        {
        public:
            GarrisonMissionBonusRoll(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_MISSION_BONUS_ROLL, std::move(packet)) { }

            void Read() override;

            ObjectGuid NpcGUID;
            uint32 MissionRecID = 0;
        };

        class GarrisonRecruitFollower final : public ClientPacket
        {
        public:
            GarrisonRecruitFollower(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_RECRUIT_FOLLOWER, std::move(packet)) { }

            void Read() override;

            ObjectGuid Guid;
            int32 FollowerIndex = 0;
        };

        class GetTrophyList final : public ClientPacket
        {
        public:
            GetTrophyList(WorldPacket&& packet) : ClientPacket(CMSG_GET_TROPHY_LIST, std::move(packet)) { }

            void Read() override;

            uint32 TrophyTypeID = 0;
        };
    }
}

#endif // GarrisonPackets_h__
