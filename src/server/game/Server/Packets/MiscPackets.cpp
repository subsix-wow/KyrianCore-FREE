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

#include "MiscPackets.h"
#include "PacketUtilities.h"
#include "Player.h"
#include "Common.h"
#include <time.h>

WorldPacket const* WorldPackets::Misc::BindPointUpdate::Write()
{
    _worldPacket << BindPosition;
    _worldPacket << uint32(BindMapID);
    _worldPacket << uint32(BindAreaID);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::InvalidatePlayer::Write()
{
    _worldPacket << Guid;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::LoginSetTimeSpeed::Write()
{
    _worldPacket.AppendPackedTime(ServerTime);
    _worldPacket.AppendPackedTime(GameTime);
    _worldPacket << float(NewSpeed);
    _worldPacket << uint32(ServerTimeHolidayOffset);
    _worldPacket << uint32(GameTimeHolidayOffset);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::SetCurrency::Write()
{
    _worldPacket << int32(Type);
    _worldPacket << int32(Quantity);
    _worldPacket << uint32(Flags);
    _worldPacket.WriteBit(WeeklyQuantity.is_initialized());
    _worldPacket.WriteBit(TrackedQuantity.is_initialized());
    _worldPacket.WriteBit(MaxQuantity.is_initialized());
    _worldPacket.WriteBit(Unused901.is_initialized());
    _worldPacket.WriteBit(SuppressChatLog);
    _worldPacket.WriteBit(QuantityChange.is_initialized());
    _worldPacket.WriteBit(QuantityGainSource.is_initialized());
    _worldPacket.WriteBit(QuantityLostSource.is_initialized());
    _worldPacket.FlushBits();

    if (WeeklyQuantity)
        _worldPacket << int32(*WeeklyQuantity);

    if (TrackedQuantity)
        _worldPacket << int32(*TrackedQuantity);

    if (MaxQuantity)
        _worldPacket << int32(*MaxQuantity);

    if (Unused901)
        _worldPacket << int32(*Unused901);

    if (QuantityChange)
        _worldPacket << int32(*QuantityChange);

    if (QuantityGainSource)
        _worldPacket << int32(*QuantityGainSource);

    if (QuantityLostSource)
        _worldPacket << int32(*QuantityLostSource);

    return &_worldPacket;
}

void WorldPackets::Misc::SetSelection::Read()
{
    _worldPacket >> Selection;
}

WorldPacket const* WorldPackets::Misc::SetupCurrency::Write()
{
    _worldPacket << uint32(Data.size());

    for (Record const& data : Data)
    {
        _worldPacket << int32(data.Type);
        _worldPacket << int32(data.Quantity);

        _worldPacket.WriteBit(data.WeeklyQuantity.is_initialized());
        _worldPacket.WriteBit(data.MaxWeeklyQuantity.is_initialized());
        _worldPacket.WriteBit(data.TrackedQuantity.is_initialized());
        _worldPacket.WriteBit(data.MaxQuantity.is_initialized());
        _worldPacket.WriteBit(data.Unused901.is_initialized());
        _worldPacket.WriteBits(data.Flags, 5);
        _worldPacket.FlushBits();

        if (data.WeeklyQuantity)
            _worldPacket << uint32(*data.WeeklyQuantity);
        if (data.MaxWeeklyQuantity)
            _worldPacket << uint32(*data.MaxWeeklyQuantity);
        if (data.TrackedQuantity)
            _worldPacket << uint32(*data.TrackedQuantity);
        if (data.MaxQuantity)
            _worldPacket << int32(*data.MaxQuantity);
        if (data.Unused901)
            _worldPacket << int32(*data.Unused901);
    }

    return &_worldPacket;
}

void WorldPackets::Misc::ViolenceLevel::Read()
{
    _worldPacket >> ViolenceLvl;
}

WorldPacket const* WorldPackets::Misc::TimeSyncRequest::Write()
{
    _worldPacket << SequenceIndex;

    return &_worldPacket;
}

void WorldPackets::Misc::TimeSyncResponse::Read()
{
    _worldPacket >> SequenceIndex;
    _worldPacket >> ClientTime;
}

WorldPacket const* WorldPackets::Misc::ServerTimeOffset::Write()
{
    _worldPacket << Time;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::TriggerMovie::Write()
{
    _worldPacket << uint32(MovieID);

    return &_worldPacket;
}
WorldPacket const* WorldPackets::Misc::TriggerCinematic::Write()
{
    _worldPacket << uint32(CinematicID);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::TutorialFlags::Write()
{
    _worldPacket.append(TutorialData, MAX_ACCOUNT_TUTORIAL_VALUES);

    return &_worldPacket;
}

void WorldPackets::Misc::TutorialSetFlag::Read()
{
    Action = _worldPacket.ReadBits(2);

    if (Action == TUTORIAL_ACTION_UPDATE)
        _worldPacket >> TutorialBit;
}

WorldPacket const* WorldPackets::Misc::WorldServerInfo::Write()
{
    _worldPacket << uint32(DifficultyID);
    _worldPacket << uint8(IsTournamentRealm);
    _worldPacket.WriteBit(XRealmPvpAlert);
    _worldPacket.WriteBit(BlockExitingLoadingScreen);
    _worldPacket.WriteBit(RestrictedAccountMaxLevel.is_initialized());
    _worldPacket.WriteBit(RestrictedAccountMaxMoney.is_initialized());
    _worldPacket.WriteBit(InstanceGroupSize.is_initialized());

    if (RestrictedAccountMaxLevel)
        _worldPacket << uint32(*RestrictedAccountMaxLevel);

    if (RestrictedAccountMaxMoney)
        _worldPacket << uint64(*RestrictedAccountMaxMoney);

    if (InstanceGroupSize)
        _worldPacket << uint32(*InstanceGroupSize);

    _worldPacket.FlushBits();

    return &_worldPacket;
}

void WorldPackets::Misc::SetDungeonDifficulty::Read()
{
    _worldPacket >> DifficultyID;
}

void WorldPackets::Misc::SetRaidDifficulty::Read()
{
    _worldPacket >> DifficultyID;
    _worldPacket >> Legacy;
}

WorldPacket const* WorldPackets::Misc::DungeonDifficultySet::Write()
{
    _worldPacket << int32(DifficultyID);
    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::RaidDifficultySet::Write()
{
    _worldPacket << int32(DifficultyID);
    _worldPacket << uint8(Legacy);
    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::CorpseReclaimDelay::Write()
{
    _worldPacket << Remaining;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::DeathReleaseLoc::Write()
{
    _worldPacket << MapID;
    _worldPacket << Loc;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::PreRessurect::Write()
{
    _worldPacket << PlayerGUID;

    return &_worldPacket;
}

void WorldPackets::Misc::ReclaimCorpse::Read()
{
    _worldPacket >> CorpseGUID;
}

void WorldPackets::Misc::RepopRequest::Read()
{
    CheckInstance = _worldPacket.ReadBit();
}

WorldPacket const* WorldPackets::Misc::RequestCemeteryListResponse::Write()
{
    _worldPacket.WriteBit(IsGossipTriggered);
    _worldPacket.FlushBits();

    _worldPacket << uint32(CemeteryID.size());
    for (uint32 cemetery : CemeteryID)
        _worldPacket << cemetery;

    return &_worldPacket;
}

void WorldPackets::Misc::ResurrectResponse::Read()
{
    _worldPacket >> Resurrecter;
    _worldPacket >> Response;
}

WorldPackets::Misc::Weather::Weather() : ServerPacket(SMSG_WEATHER, 4 + 4 + 1) { }

WorldPackets::Misc::Weather::Weather(WeatherState weatherID, float intensity /*= 0.0f*/, bool abrupt /*= false*/)
    : ServerPacket(SMSG_WEATHER, 4 + 4 + 1), Abrupt(abrupt), Intensity(intensity), WeatherID(weatherID) { }

WorldPacket const* WorldPackets::Misc::Weather::Write()
{
    _worldPacket << uint32(WeatherID);
    _worldPacket << float(Intensity);
    _worldPacket.WriteBit(Abrupt);

    _worldPacket.FlushBits();
    return &_worldPacket;
}

void WorldPackets::Misc::StandStateChange::Read()
{
    uint32 state;
    _worldPacket >> state;

    StandState = UnitStandStateType(state);
}

WorldPacket const* WorldPackets::Misc::StandStateUpdate::Write()
{
    _worldPacket << uint32(AnimKitID);
    _worldPacket << uint8(State);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::SetAnimTier::Write()
{
    _worldPacket << Unit;
    _worldPacket.WriteBits(Tier, 3);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::PlayerBound::Write()
{
    _worldPacket << BinderID;
    _worldPacket << uint32(AreaID);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::BinderConfirm::Write()
{
    _worldPacket << Unit;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::StartMirrorTimer::Write()
{
    _worldPacket << int32(Timer);
    _worldPacket << int32(Value);
    _worldPacket << int32(MaxValue);
    _worldPacket << int32(Scale);
    _worldPacket << int32(SpellID);
    _worldPacket.WriteBit(Paused);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::PauseMirrorTimer::Write()
{
    _worldPacket << int32(Timer);
    _worldPacket.WriteBit(Paused);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::StopMirrorTimer::Write()
{
    _worldPacket << int32(Timer);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::ExplorationExperience::Write()
{
    _worldPacket << int32(AreaID);
    _worldPacket << int32(Experience);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::LevelUpInfo::Write()
{
    _worldPacket << int32(Level);
    _worldPacket << int32(HealthDelta);

    for (int32 power : PowerDelta)
        _worldPacket << power;

    for (int32 stat : StatDelta)
        _worldPacket << stat;

    _worldPacket << int32(NumNewTalents);
    _worldPacket << int32(NumNewPvpTalentSlots);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::PlayMusic::Write()
{
    _worldPacket << uint32(SoundKitID);

    return &_worldPacket;
}

void WorldPackets::Misc::RandomRollClient::Read()
{
    _worldPacket >> Min;
    _worldPacket >> Max;
    _worldPacket >> PartyIndex;
}

WorldPacket const* WorldPackets::Misc::RandomRoll::Write()
{
    _worldPacket << Roller;
    _worldPacket << RollerWowAccount;
    _worldPacket << int32(Min);
    _worldPacket << int32(Max);
    _worldPacket << int32(Result);

    return &_worldPacket;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Misc::PhaseShiftDataPhase const& phaseShiftDataPhase)
{
    data << uint16(phaseShiftDataPhase.PhaseFlags);
    data << uint16(phaseShiftDataPhase.Id);
    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Misc::PhaseShiftData const& phaseShiftData)
{
    data << uint32(phaseShiftData.PhaseShiftFlags);
    data << uint32(phaseShiftData.Phases.size());
    data << phaseShiftData.PersonalGUID;
    for (WorldPackets::Misc::PhaseShiftDataPhase const& phaseShiftDataPhase : phaseShiftData.Phases)
        data << phaseShiftDataPhase;

    return data;
}

WorldPacket const* WorldPackets::Misc::PhaseShiftChange::Write()
{
    _worldPacket << Client;
    _worldPacket << Phaseshift;
    _worldPacket << uint32(VisibleMapIDs.size() * 2);           // size in bytes
    for (uint16 visibleMapId : VisibleMapIDs)
        _worldPacket << uint16(visibleMapId);                   // Active terrain swap map id

    _worldPacket << uint32(PreloadMapIDs.size() * 2);           // size in bytes
    for (uint16 preloadMapId : PreloadMapIDs)
        _worldPacket << uint16(preloadMapId);                   // Inactive terrain swap map id

    _worldPacket << uint32(UiMapPhaseIDs.size() * 2);           // size in bytes
    for (uint16 uiMapPhaseId : UiMapPhaseIDs)
        _worldPacket << uint16(uiMapPhaseId);                   // phase id, controls only map display (visible on all maps)

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::ZoneUnderAttack::Write()
{
    _worldPacket << int32(AreaID);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::DurabilityDamageDeath::Write()
{
    _worldPacket << int32(Percent);

    return &_worldPacket;
}

void WorldPackets::Misc::ObjectUpdateFailed::Read()
{
    _worldPacket >> ObjectGUID;
}

void WorldPackets::Misc::ObjectUpdateRescued::Read()
{
    _worldPacket >> ObjectGUID;
}

WorldPacket const* WorldPackets::Misc::PlayObjectSound::Write()
{
    _worldPacket << int32(SoundKitID);
    _worldPacket << SourceObjectGUID;
    _worldPacket << TargetObjectGUID;
    _worldPacket << Position;
    _worldPacket << int32(BroadcastTextID);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::PlaySound::Write()
{
    _worldPacket << int32(SoundKitID);
    _worldPacket << SourceObjectGuid;
    _worldPacket << int32(BroadcastTextID);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::PlaySpeakerbotSound::Write()
{
    _worldPacket << SourceObjectGUID;
    _worldPacket << int32(SoundKitID);

    return &_worldPacket;
}

void WorldPackets::Misc::FarSight::Read()
{
    Enable = _worldPacket.ReadBit();
}

void WorldPackets::Misc::SaveCUFProfiles::Read()
{
    CUFProfiles.resize(_worldPacket.read<uint32>());
    for (std::unique_ptr<CUFProfile>& cufProfile : CUFProfiles)
    {
        cufProfile = std::make_unique<CUFProfile>();

        uint8 strLen = _worldPacket.ReadBits(7);

        // Bool Options
        for (uint8 option = 0; option < CUF_BOOL_OPTIONS_COUNT; option++)
            cufProfile->BoolOptions.set(option, _worldPacket.ReadBit());

        // Other Options
        _worldPacket >> cufProfile->FrameHeight;
        _worldPacket >> cufProfile->FrameWidth;

        _worldPacket >> cufProfile->SortBy;
        _worldPacket >> cufProfile->HealthText;

        _worldPacket >> cufProfile->TopPoint;
        _worldPacket >> cufProfile->BottomPoint;
        _worldPacket >> cufProfile->LeftPoint;

        _worldPacket >> cufProfile->TopOffset;
        _worldPacket >> cufProfile->BottomOffset;
        _worldPacket >> cufProfile->LeftOffset;

        cufProfile->ProfileName = _worldPacket.ReadString(strLen);
    }
}

WorldPacket const* WorldPackets::Misc::LoadCUFProfiles::Write()
{
    _worldPacket << uint32(CUFProfiles.size());

    for (CUFProfile const* cufProfile : CUFProfiles)
    {
        _worldPacket.WriteBits(cufProfile->ProfileName.size(), 7);

        // Bool Options
        for (uint8 option = 0; option < CUF_BOOL_OPTIONS_COUNT; option++)
            _worldPacket.WriteBit(cufProfile->BoolOptions[option]);

        // Other Options
        _worldPacket << cufProfile->FrameHeight;
        _worldPacket << cufProfile->FrameWidth;

        _worldPacket << cufProfile->SortBy;
        _worldPacket << cufProfile->HealthText;

        _worldPacket << cufProfile->TopPoint;
        _worldPacket << cufProfile->BottomPoint;
        _worldPacket << cufProfile->LeftPoint;

        _worldPacket << cufProfile->TopOffset;
        _worldPacket << cufProfile->BottomOffset;
        _worldPacket << cufProfile->LeftOffset;

        _worldPacket.WriteString(cufProfile->ProfileName);
    }

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::PlayOneShotAnimKit::Write()
{
    _worldPacket << Unit;
    _worldPacket << uint16(AnimKitID);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::SetAIAnimKit::Write()
{
    _worldPacket << Unit;
    _worldPacket << uint16(AnimKitID);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::SetMovementAnimKit::Write()
{
    _worldPacket << Unit;
    _worldPacket << uint16(AnimKitID);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::SetMeleeAnimKit::Write()
{
    _worldPacket << Unit;
    _worldPacket << uint16(AnimKitID);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::SetPlayHoverAnim::Write()
{
    _worldPacket << UnitGUID;
    _worldPacket.WriteBit(PlayHoverAnim);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

void WorldPackets::Misc::SetPvP::Read()
{
    EnablePVP = _worldPacket.ReadBit();
}

WorldPacket const* WorldPackets::Misc::AccountHeirloomUpdate::Write()
{
    _worldPacket.WriteBit(IsFullUpdate);
    _worldPacket.FlushBits();

    _worldPacket << int32(Unk);

    // both lists have to have the same size
    _worldPacket << uint32(Heirlooms->size());
    _worldPacket << uint32(Heirlooms->size());

    for (auto const& item : *Heirlooms)
        _worldPacket << int32(item.first);

    for (auto const& flags : *Heirlooms)
        _worldPacket << uint32(flags.second.flags);

    return &_worldPacket;
}

void WorldPackets::Misc::MountSpecial::Read()
{
    SpellVisualKitIDs.resize(_worldPacket.read<uint32>());
    for (int32& spellVisualKitId : SpellVisualKitIDs)
        _worldPacket >> spellVisualKitId;
}

WorldPacket const* WorldPackets::Misc::SpecialMountAnim::Write()
{
    _worldPacket << UnitGUID;
    _worldPacket << uint32(SpellVisualKitIDs.size());
    if (!SpellVisualKitIDs.empty())
        _worldPacket.append(SpellVisualKitIDs.data(), SpellVisualKitIDs.size());

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::CrossedInebriationThreshold::Write()
{
    _worldPacket << Guid;
    _worldPacket << int32(Threshold);
    _worldPacket << int32(ItemID);

    return &_worldPacket;
}

void WorldPackets::Misc::SetTaxiBenchmarkMode::Read()
{
    Enable = _worldPacket.ReadBit();
}

WorldPacket const* WorldPackets::Misc::OverrideLight::Write()
{
    _worldPacket << int32(AreaLightID);
    _worldPacket << int32(OverrideLightID);
    _worldPacket << int32(TransitionMilliseconds);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::DisplayGameError::Write()
{
    _worldPacket << uint32(Error);
    _worldPacket.WriteBit(Arg.is_initialized());
    _worldPacket.WriteBit(Arg2.is_initialized());
    _worldPacket.FlushBits();

    if (Arg)
        _worldPacket << int32(*Arg);

    if (Arg2)
        _worldPacket << int32(*Arg2);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::AccountMountUpdate::Write()
{
    _worldPacket.WriteBit(IsFullUpdate);
    _worldPacket << uint32(Mounts->size());

    for (auto const& spell : *Mounts)
    {
        _worldPacket << int32(spell.first);
        _worldPacket.WriteBits(spell.second, 2);
    }

    _worldPacket.FlushBits();

    return &_worldPacket;
}

void WorldPackets::Misc::MountSetFavorite::Read()
{
    _worldPacket >> MountSpellID;
    IsFavorite = _worldPacket.ReadBit();
}

void WorldPackets::Misc::CloseInteraction::Read()
{
    _worldPacket >> SourceGuid;
}

WorldPacket const* WorldPackets::Misc::StartTimer::Write()
{
    _worldPacket << TimeLeft;
    _worldPacket << TotalTime;
    _worldPacket << int32(Type);

    return &_worldPacket;
}

void WorldPackets::Misc::DiscardedTimeSyncAcks::Read()
{
    _worldPacket >> MaxSequenceIndex;
}

WorldPacket const* WorldPackets::Misc::VignetteUpdate::Write()
{
    _worldPacket.WriteBit(ForceUpdate);
    _worldPacket.WriteBit(UnkBit901);
    _worldPacket.FlushBits();

    _worldPacket << static_cast<uint32>(Removed.IDs.size());
    for (ObjectGuid const& ID : Removed.IDs)
        _worldPacket << ID;

    _worldPacket << static_cast<uint32>(Added.IdList.IDs.size());
    for (ObjectGuid const& ID : Added.IdList.IDs)
        _worldPacket << ID;

    _worldPacket << static_cast<uint32>(Added.Data.size());
    for (auto const& x : Added.Data)
    {
        _worldPacket << x.Pos;
        _worldPacket << x.ObjGUID;
        _worldPacket << x.VignetteID;
        _worldPacket << x.AreaID;
        _worldPacket << x.Unk901_1;
        _worldPacket << x.Unk901_2;
    }

    _worldPacket << static_cast<uint32>(Updated.IdList.IDs.size());
    for (ObjectGuid const& ID : Updated.IdList.IDs)
        _worldPacket << ID;

    _worldPacket << static_cast<uint32>(Updated.Data.size());
    for (auto const& x : Updated.Data)
    {
        _worldPacket << x.Pos;
        _worldPacket << x.ObjGUID;
        _worldPacket << x.VignetteID;
        _worldPacket << x.AreaID;
        _worldPacket << x.Unk901_1;
        _worldPacket << x.Unk901_2;
    }

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::StopElapsedTimer::Write()
{
    _worldPacket << TimerID;
    _worldPacket.WriteBit(KeepTimer);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::StartElapsedTimer::Write()
{
    _worldPacket << Timer.TimerID;
    _worldPacket << uint32(Timer.CurrentDuration);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::SetTaskComplete::Write()
{
    _worldPacket << TaskID;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::SetAllTaskProgress::Write()
{
    _worldPacket << static_cast<uint32>(Progress.size());
  //  for (auto const& x : Progress)
      //  _worldPacket << x;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::UpdateTaskProgress::Write()
{
    _worldPacket << static_cast<uint32>(Progress.size());
   // for (auto const& x : Progress)
      //  _worldPacket << x;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::StreamingMovie::Write()
{
    _worldPacket << static_cast<uint32>(MovieIDs.size());
    for (auto const& v : MovieIDs)
        _worldPacket << v;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::SetMaxWeeklyQuantity::Write()
{
    _worldPacket << Type;
    _worldPacket << MaxWeeklyQuantity;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::ResearchSetupHistory::Write()
{
    _worldPacket << int32(ResearchHistory.size());

  //  for (auto const& Research : ResearchHistory)
   //     _worldPacket << Research;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::PlayerSkinned::Write()
{
    _worldPacket.WriteBit(FreeRepop);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::ChromieTimeOpenNpc::Write()
{
    _worldPacket << ObjGUID;
    
        return &_worldPacket;
    }

WorldPacket const* WorldPackets::Misc::ChromieTimeSelectExpansionSuccess::Write()
{
    return &_worldPacket;
}

void WorldPackets::Misc::ChromieTimeSelectExpansion::Read()
{
    _worldPacket >> ObjGUID;
    _worldPacket >> Expansion;
}

WorldPacket const* WorldPackets::Misc::MapObjEvents::Write()
{
    _worldPacket << UniqueID;
    _worldPacket << DataSize;
    _worldPacket << static_cast<uint32>(Unk2.size());
    for (auto const& itr : Unk2)
        _worldPacket << itr;

    return &_worldPacket;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Misc::ResearchHistory const& researchHistory)
{
    data << researchHistory.ProjectID;
    data << uint32(researchHistory.FirstCompleted);
    data << researchHistory.CompletionCount;

    return data;
}

WorldPacket const* WorldPackets::Misc::ResearchComplete::Write()
{
    _worldPacket << Research;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::IslandOpenNpc::Write()
{
    _worldPacket << QueueNPCGuid;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::IslandAzeriteXpGain::Write()
{
    _worldPacket << XpGain;
    _worldPacket << PlayerGuid;
    _worldPacket << SourceGuid;
    _worldPacket << SourceID;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::IslandCompleted::Write()
{
    _worldPacket << MapID;
    _worldPacket << Winner;
    _worldPacket << DisplayInfos.size();
    /*
    if (!DisplayInfos.empty())
        for (auto displayInfo : DisplayInfos)
        {
            _worldPacket << displayInfo.GUID;
            _worldPacket << int32(displayInfo.SpecializationID);
            _worldPacket << uint32(displayInfo.Items.size());
            _worldPacket.WriteBits(displayInfo.Name.length(), 6);
            _worldPacket << uint8(displayInfo.GenderID);
            _worldPacket << uint8(displayInfo.Skin);
            _worldPacket << uint8(displayInfo.HairColor);
            _worldPacket << uint8(displayInfo.HairStyle);
            _worldPacket << uint8(displayInfo.FacialHairStyle);
            _worldPacket << uint8(displayInfo.Face);
            _worldPacket << uint8(displayInfo.Race);
            _worldPacket << uint8(displayInfo.ClassID);
            _worldPacket.append(displayInfo.CustomDisplay.data(), displayInfo.CustomDisplay.size());
            _worldPacket.WriteString(displayInfo.Name);

            for (WorldPackets::Inspect::InspectItemData const& itemData : displayInfo.Items)
            {
                _worldPacket << itemData.CreatorGUID;
                _worldPacket << uint8(itemData.Index);
                _worldPacket << uint32(itemData.AzeritePowers.size());
                _worldPacket << uint32(itemData.AzeriteEssences.size());

                if (!itemData.AzeritePowers.empty())
                    _worldPacket.append(itemData.AzeritePowers.data(), itemData.AzeritePowers.size());

                _worldPacket << itemData.Item;
                _worldPacket.WriteBit(itemData.Usable);
                _worldPacket.WriteBits(itemData.Enchants.size(), 4);
                _worldPacket.WriteBits(itemData.Gems.size(), 2);
                _worldPacket.FlushBits();

                for (WorldPackets::Inspect::AzeriteEssenceData const& azeriteEssenceData : itemData.AzeriteEssences)
                {
                    _worldPacket << uint32(azeriteEssenceData.Index);
                    _worldPacket << uint32(azeriteEssenceData.AzeriteEssenceID);
                    _worldPacket << uint32(azeriteEssenceData.Rank);
                    _worldPacket.WriteBit(azeriteEssenceData.SlotUnlocked);
                    _worldPacket.FlushBits();
                }

                for (WorldPackets::Inspect::InspectEnchantData const& enchantData : itemData.Enchants)
                {
                    _worldPacket << uint32(enchantData.Id);
                    _worldPacket << uint8(enchantData.Index);
                }

                for (WorldPackets::Item::ItemGemData const& gem : itemData.Gems)
                    _worldPacket << gem;
            }

        }
    */
    return &_worldPacket;
}

void WorldPackets::Misc::IslandOnQueue::Read()
{
    _worldPacket >> QueueNPCGuid;
    _worldPacket >> ActivityID;
}

WorldPacket const* WorldPackets::Misc::ShowTradeSkillResponse::Write()
{
    _worldPacket << PlayerGUID;
    _worldPacket << SpellId;
    _worldPacket << static_cast<uint32>(SkillLineIDs.size());
    _worldPacket << static_cast<uint32>(SkillRanks.size());
    _worldPacket << static_cast<uint32>(SkillMaxRanks.size());
    _worldPacket << static_cast<uint32>(KnownAbilitySpellIDs.size());

    for (auto const& v : SkillLineIDs)
        _worldPacket << v;

    for (auto const& c : SkillRanks)
        _worldPacket << c;

    for (auto const& z : SkillMaxRanks)
        _worldPacket << z;

    for (auto const& t : KnownAbilitySpellIDs)
        _worldPacket << t;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::DismountResult::Write()
{
    _worldPacket << Result;

    return &_worldPacket;
}
