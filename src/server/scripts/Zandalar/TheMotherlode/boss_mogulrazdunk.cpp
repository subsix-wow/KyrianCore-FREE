/*
 * Copyright 2021 HellgarveCore
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

#include "AreaTrigger.h"
#include "AreaTriggerAI.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuras.h"
#include "SpellAuraEffects.h"
#include "the_motherlode.h"
#include "ScriptedCreature.h"
#include "SpellHistory.h"

enum Spells
{
    SPELL_ALPHA_CANNON_MISSILE = 260318,

    SPELL_HOMING_MISSILE_VISUAL = 260815,
    SPELL_HOMING_MISSILE_CAST = 260811,
    SPELL_HOMING_MISSILE_SPEED = 260836,
    SPELL_HOMING_MISSILE_DAMAGE = 260838,

    SPELL_GATLING_GUN_DAMAGE = 260279,
    SPELL_GATLING_GUN_CAST = 260280,

    SPELL_CONFIGURATION_DRILL = 260189, //x3

    SPELL_DRILL_SMASH_CAST_DAMAGE = 260202, // on player reach Z
    SPELL_BIG_RED_ROCKET_MISSILE = 270276,
    SPELL_BIG_RED_ROCKET_VISUAL = 270279,
    SPELL_DRILL_SMASH_VISUAL = 260194,
    SPELL_DRILL_SMASH_CAST = 271456, // cast while air Z

    SPELL_MICRO_MISSILE_CAST = 276229,
    SPELL_MICRO_MISSILE_MISSILE = 276231,

    SPELL_CONCUSSION_CHARGE_CAST = 281621,
    SPELL_CONCUSSION_CHARGE_MISSILE = 262284,

    SPELL_AZERITE_HEARTSEEKER_CAST = 262515,
    SPELL_AZERITE_HEARTSEEKER_DAMAGE = 262516,
};

enum Evets
{
    EVENT_ALPHA_CANNON = 1,
    EVENT_HOMING_MISSILE,
    EVENT_GATLING_GUN,
    
    EVENT_DRILL,
    EVENT_SUMMON_BIG_RED_ROCKET,

    EVENT_MICRO_MISSILE, 

    EVENT_MISSILE_EXPLODE,
    EVENT_FIXATE,
    EVENT_FOLLOW,

    EVENT_CONCUSSION_CHARGE,
    EVENT_AZERITE_HEARTSEEKER,
};

enum Timers
{
    TIMER_MICRO_MISSILE = 15 * IN_MILLISECONDS,
    TIMER_CONCUSSION_CHARGE = 10 * IN_MILLISECONDS,
    TIMER_AZERITE_HEARTSEEKER = 17 * IN_MILLISECONDS,

    TIMER_BIG_RED_ROCKET = 15 * IN_MILLISECONDS,

    TIMER_ALPHA_CANNON = 3 * IN_MILLISECONDS,
    TIMER_GATLING_GUN = 28 * IN_MILLISECONDS,
    TIMER_HOMING_MISSILE = 10 * IN_MILLISECONDS,
};

enum Creatures
{
    BOSS_MOGUL_RAZDUNK = 129232,
    NPC_HOMING_MISSILE = 132338,
    NPC_BIG_RED_ROCKET = 131973,
    NPC_VENTURE_SKYSCORCHER = 133436,
    NPC_MISSILE_TARGET = 132271,
    NPC_BOOMBA = 141303,
};

enum Actions
{
    ACTION_ENGAGE_MISSILE = 1,
};

enum Points
{
    POINT_SPELLS = 1,
};

const Position centerPos = { 1101.65f, -3918.86f, 79.52f }; //40

#define INTRO_TEXT "Do you bums realize how much property damage you've done !?"
#define AGGRO_TEXT "Right where you belong--under my heel!"
#define GATLING_GUN "Taste some high-caliber carnage!"
#define DRILL_TEXT "Crush!"
#define PHASE2_TEXT "What am I payin' you fools for?! Get out here and fix this!"

class bfa_boss_mogul_razdunk : public CreatureScript
{
public:
    bfa_boss_mogul_razdunk() : CreatureScript("bfa_boss_mogul_razdunk")
    {
    }
    struct bfa_boss_mogul_razdunk_AI : public BossAI
    {
        bfa_boss_mogul_razdunk_AI(Creature* creature) : BossAI(creature, DATA_MOGUL_RAZDUNK), summons(me)
        {
            instance = me->GetInstanceScript();
        }

        SummonList summons;
        EventMap events;
        InstanceScript* instance;
        bool phase2;
        bool introText;

        void Reset() override
        {
            introText = false;
            summons.DespawnAll();
            phase2 = false;
            events.Reset();
            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
        }

        void SelectSoundAndText(Creature* me, uint32  selectedTextSound = 0)
        {
            if (!me)
                return;

            if (me)
            {
                switch (selectedTextSound)
                {
                case 1:
                    me->Yell(AGGRO_TEXT, LANG_UNIVERSAL, NULL);
                    break;
                case 2:
                    me->Yell(INTRO_TEXT, LANG_UNIVERSAL, NULL);
                    break;
                case 3:
                    me->Yell(DRILL_TEXT, LANG_UNIVERSAL, NULL);
                    break;
                case 4:
                    me->Yell(PHASE2_TEXT, LANG_UNIVERSAL, NULL);
                    break;
                case 5:
                    me->Yell(GATLING_GUN, LANG_UNIVERSAL, NULL);
                    break;
                }
            }
        }

        void JustDied(Unit*) override
        {
            summons.DespawnAll();
            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
        }

        void MoveInLineOfSight(Unit* who) override
        {
            if (!introText)
            {
                Map::PlayerList const& lPlayers = me->GetMap()->GetPlayers();
                for (Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
                    if (Player* pPlayer = itr->GetSource())
                        if (pPlayer->IsAlive() && pPlayer->IsWithinDist(me, 50.f))
                        {
                            SelectSoundAndText(me, 2);
                            introText = true;
                            return;
                        }
            }
        }

        void JustSummoned(Creature* summon) override
        {
            summons.Summon(summon);

            switch (summon->GetEntry())
            {
            case NPC_BIG_RED_ROCKET:
                summon->SetUnitFlags(UNIT_FLAG_NON_ATTACKABLE);
                summon->SetUnitFlags(UNIT_FLAG_NOT_SELECTABLE);
                summon->CastSpell(summon, SPELL_BIG_RED_ROCKET_VISUAL, true);
                break;
            case NPC_VENTURE_SKYSCORCHER:
                summon->SetInCombatWithZone();
                break;
            }
        }

        void EnterEvadeMode(EvadeReason w) override
        {
            _DespawnAtEvade(15);
        }

        void DamageTaken(Unit* at, uint32& damage) override
        {
            if (me->HealthBelowPct(50) && !phase2)
            {
                phase2 = true;
                SummonVenture();
                HandlePhase2();
            }
        }

        void HandlePhase1()
        {
            events.Reset();
            SummonBoomba();
            me->GetMotionMaster()->Clear();
            events.ScheduleEvent(EVENT_ALPHA_CANNON, TIMER_ALPHA_CANNON);
            events.ScheduleEvent(EVENT_GATLING_GUN, TIMER_GATLING_GUN);
            events.ScheduleEvent(EVENT_HOMING_MISSILE, TIMER_HOMING_MISSILE);
        }

        void HandlePhase2()
        {
            events.Reset();
            SelectSoundAndText(me, 4);
            summons.DespawnEntry(NPC_BOOMBA);
            for (uint8 i = 0; i < 3; ++i)
                me->CastSpell(me, SPELL_CONFIGURATION_DRILL, true);
            me->SetCanFly(true);
            me->AddUnitState(UNIT_STATE_ROOT);
            me->GetMotionMaster()->MovePoint(0, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() + 3.0f, false);
            events.ScheduleEvent(EVENT_SUMMON_BIG_RED_ROCKET, TIMER_BIG_RED_ROCKET);
        }

        void EnterCombat(Unit*)// override
        {
            SelectSoundAndText(me, 1);
            instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
            HandlePhase1();
        }

        void SummonVenture()
        {
            me->SummonCreature(NPC_VENTURE_SKYSCORCHER, 1136.92f, -3900.25f, 79.71f, TEMPSUMMON_MANUAL_DESPAWN);
            me->SummonCreature(NPC_VENTURE_SKYSCORCHER, 1104.74f, -3956.37f, 79.72f, TEMPSUMMON_CORPSE_DESPAWN);
        }

        void SummonBoomba()
        {
            if (me->GetMap()->IsHeroic() || me->GetMap()->IsMythic())
            {
                if (Creature* boomba1 = me->SummonCreature(NPC_BOOMBA, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), TEMPSUMMON_MANUAL_DESPAWN))
                    boomba1->GetMotionMaster()->MovePoint(POINT_SPELLS, 1135.90f, -3886.88f, 94.96f, false);

                if (Creature* boomba2 = me->SummonCreature(NPC_BOOMBA, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), TEMPSUMMON_MANUAL_DESPAWN))
                    boomba2->GetMotionMaster()->MovePoint(POINT_SPELLS, 1111.04f, -3873.78f, 94.18f, false);
            }
        }

        void OnSpellFinished(SpellInfo const* spellInfo) //override
        {
            switch (spellInfo->Id)
            {
            case SPELL_HOMING_MISSILE_CAST:
                if (Creature* missile = me->SummonCreature(NPC_HOMING_MISSILE, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), TEMPSUMMON_MANUAL_DESPAWN))
                    missile->AI()->DoAction(ACTION_ENGAGE_MISSILE);
                break;
            case SPELL_DRILL_SMASH_CAST:
            {
                if (Unit* target = me->GetVictim())
                    me->GetMotionMaster()->MoveCharge(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 30.0f);

                me->GetScheduler().Schedule(2s, [this](TaskContext /*context*/)
                    {
                        me->CastSpell(me, SPELL_DRILL_SMASH_CAST_DAMAGE, true);
                        if (me->FindNearestCreature(NPC_BIG_RED_ROCKET, 5.0f, true))
                        {
                            if (Aura* tmp = me->GetAura(SPELL_CONFIGURATION_DRILL))
                            {
                                if (tmp->GetStackAmount() <= 1)
                                {
                                    me->RemoveAura(SPELL_CONFIGURATION_DRILL);
                                    HandlePhase1();
                                }
                                else if (tmp->GetStackAmount() > 1)
                                    tmp->SetStackAmount(tmp->GetStackAmount() - 1);
                            }
                        }
                    });

                me->GetScheduler().Schedule(3s, [this](TaskContext /*context*/)
                    {
                        me->GetMotionMaster()->MovePoint(0, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() + 3.0f, false);
                    });
                
                break;
            }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            events.Update(diff);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_HOMING_MISSILE:
                    me->CastSpell(me, SPELL_HOMING_MISSILE_CAST);
                    events.ScheduleEvent(EVENT_HOMING_MISSILE, TIMER_HOMING_MISSILE);
                    break;
                case EVENT_GATLING_GUN:
                    SelectSoundAndText(me, 5);
                    me->CastSpell(me, SPELL_GATLING_GUN_CAST);
                    events.ScheduleEvent(EVENT_GATLING_GUN, TIMER_GATLING_GUN);
                    break;
                case EVENT_ALPHA_CANNON:
                    if (Unit* target = me->GetVictim())
                        me->CastSpell(target, SPELL_ALPHA_CANNON_MISSILE);
                    events.ScheduleEvent(EVENT_ALPHA_CANNON, TIMER_ALPHA_CANNON);
                    break;
                case EVENT_SUMMON_BIG_RED_ROCKET:
                {
                    std::list<Creature*> microList;
                    me->GetCreatureListWithEntryInGrid(microList, NPC_MISSILE_TARGET, 100.0f);
                    if (!microList.empty())
                    {
                        if (microList.size() >= 1)
                            microList.resize(1);

                        for (auto target : microList)
                        {
                            me->CastSpell(target, SPELL_BIG_RED_ROCKET_MISSILE, true);
                        }
                    }

                    events.ScheduleEvent(EVENT_DRILL, 3 * IN_MILLISECONDS);
                    break;
                }
                case EVENT_DRILL:
                {
                    SelectSoundAndText(me, 3);
                    me->GetMotionMaster()->Clear();
                    std::list<Unit*> targets;
                  //  SelectTargetList(targets, 1, SELECT_TARGET_RANDOM, 500.0f, true);

                    if (!targets.empty())
                        if (targets.size() >= 1)
                            targets.resize(1);

                    for (auto target : targets)
                    {
                        me->CastSpell(target, SPELL_DRILL_SMASH_CAST);
                    }
                    events.ScheduleEvent(EVENT_SUMMON_BIG_RED_ROCKET, TIMER_BIG_RED_ROCKET);
                    break;
                }
                }
            }
            // no melee
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new bfa_boss_mogul_razdunk_AI(creature);
    }
};

class bfa_npc_homing_missile : public CreatureScript
{
public:
    bfa_npc_homing_missile() : CreatureScript("bfa_npc_homing_missile")
    {
    }
    struct bfa_npc_homing_missile_AI : public ScriptedAI
    {
        bfa_npc_homing_missile_AI(Creature* creature) : ScriptedAI(creature)
        {
            me->SetSpeed(MOVE_WALK, 0.5f);
            me->SetSpeed(MOVE_FLIGHT, 0.5f);
            me->SetSpeed(MOVE_RUN, 0.5f);
            me->SetUnitFlags(UNIT_FLAG_NOT_SELECTABLE);
            me->SetUnitFlags(UNIT_FLAG_NON_ATTACKABLE);
            me->SetWalk(true);
        }

        EventMap events;

        void Reset() override
        {
            me->CastSpell(me, SPELL_HOMING_MISSILE_SPEED, true);
            me->CastSpell(me, SPELL_HOMING_MISSILE_VISUAL, true);
        }

        void DoAction(int32 action)
        {
            switch (action)
            {
            case ACTION_ENGAGE_MISSILE:
                events.Reset();
                events.ScheduleEvent(EVENT_MISSILE_EXPLODE, 10 * IN_MILLISECONDS);
                events.ScheduleEvent(EVENT_FIXATE, 500);
                break;
            }
        }

        void DamageTaken(Unit* a, uint32& damage) override
        {
            damage = 0;
        }

        void UpdateAI(uint32 diff) override
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_FIXATE:
                {
                    std::list<Unit*> targets;
                  //  SelectTargetList(targets, 1, SELECT_TARGET_RANDOM, 500.0f, true);

                    if (!targets.empty())
                        if (targets.size() >= 1)
                            targets.resize(1);

                    for (auto target : targets)
                    {
                      //  me->AddThreat(target, 9999999999.9f);
                        me->AI()->AttackStart(target);
                    }

                    events.ScheduleEvent(EVENT_FOLLOW, 1000);
                    break;
                }
                case EVENT_FOLLOW:
                {
                    if (Unit* victim = me->GetVictim())
                    {
                        me->GetMotionMaster()->MoveFollow(victim, 0.0f, 0.0f);
                        if (me->GetDistance(victim) < 2.0f)
                        {
                            me->CastSpell(victim, SPELL_HOMING_MISSILE_DAMAGE, true);
                            me->DespawnOrUnsummon();
                        }

                    }

                    events.ScheduleEvent(EVENT_FOLLOW, 1000);
                    break;
                }
                case EVENT_MISSILE_EXPLODE:
                    me->CastSpell(me, SPELL_HOMING_MISSILE_DAMAGE, true);
                    me->DespawnOrUnsummon();
                    break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new bfa_npc_homing_missile_AI(creature);
    }
};

class bfa_npc_boomba : public CreatureScript
{
public:
    bfa_npc_boomba() : CreatureScript("bfa_npc_boomba")
    {
    }
    struct bfa_npc_boomba_AI : public ScriptedAI
    {
        bfa_npc_boomba_AI(Creature* creature) : ScriptedAI(creature)
        {
            me->SetUnitFlags(UNIT_FLAG_NON_ATTACKABLE);
            me->SetUnitFlags(UNIT_FLAG_IMMUNE_TO_PC);
        }

        EventMap events;

        void MovementInform(uint32 type, uint32 pointId) override
        {
            switch (pointId)
            {
            case POINT_SPELLS:
                me->AddUnitState(UNIT_STATE_ROOT);
                events.ScheduleEvent(EVENT_MICRO_MISSILE, TIMER_MICRO_MISSILE);
                break;
            }
        }

        void Reset() override
        {
            events.Reset();
        }

        void OnSpellFinished(SpellInfo const* spellInfo)// override
        {
            switch (spellInfo->Id)
            {
            case SPELL_MICRO_MISSILE_CAST:
            {
                std::list<Creature*> microList;
                me->GetCreatureListWithEntryInGrid(microList, NPC_MISSILE_TARGET, 150.0f);
                if (!microList.empty())
                {
                    if (microList.size() >= 15)
                        microList.resize(15);

                    for (auto target : microList)
                        me->CastSpell(target, SPELL_MICRO_MISSILE_MISSILE, true);
                }
                break;
            }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_MICRO_MISSILE:
                    me->CastSpell(me, SPELL_MICRO_MISSILE_CAST);
                    events.ScheduleEvent(EVENT_MICRO_MISSILE, TIMER_MICRO_MISSILE);
                    break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new bfa_npc_boomba_AI(creature);
    }
};

class bfa_npc_venture_skyscorcher : public CreatureScript
{
public:
    bfa_npc_venture_skyscorcher() : CreatureScript("bfa_npc_venture_skyscorcher")
    {
    }
    struct bfa_npc_venture_skyscorcher_AI : public ScriptedAI
    {
        bfa_npc_venture_skyscorcher_AI(Creature* creature) : ScriptedAI(creature)
        {
        }

        EventMap events;

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit*) //override
        {
            events.ScheduleEvent(EVENT_CONCUSSION_CHARGE, TIMER_CONCUSSION_CHARGE);
            events.ScheduleEvent(EVENT_AZERITE_HEARTSEEKER, TIMER_AZERITE_HEARTSEEKER);
        }

        void OnSpellFinished(SpellInfo const* spellInfo)// override
        {
            switch (spellInfo->Id)
            {
            case SPELL_CONCUSSION_CHARGE_CAST:
            {
                if (Unit* target = me->GetVictim())
                    me->CastSpell(target, SPELL_CONCUSSION_CHARGE_MISSILE, true);
                break;
            }
            case SPELL_AZERITE_HEARTSEEKER_CAST:
                if(Unit* target = me->GetVictim())
                    me->CastSpell(SPELL_AZERITE_HEARTSEEKER_DAMAGE, true);
            }
        }

        void UpdateAI(uint32 diff) override
        {
            events.Update(diff);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_CONCUSSION_CHARGE:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                    {
                        me->CastSpell(target, SPELL_CONCUSSION_CHARGE_CAST);
                    }
                    events.ScheduleEvent(EVENT_CONCUSSION_CHARGE, TIMER_CONCUSSION_CHARGE);
                    break;
                case EVENT_AZERITE_HEARTSEEKER:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                    {
                        me->CastSpell(target, SPELL_AZERITE_HEARTSEEKER_CAST);
                    }
                    events.ScheduleEvent(EVENT_AZERITE_HEARTSEEKER, TIMER_AZERITE_HEARTSEEKER);
                    break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new bfa_npc_venture_skyscorcher_AI(creature);
    }
};

// 260280
class bfa_spell_gatling_gun : public SpellScriptLoader
{
public:
    bfa_spell_gatling_gun() : SpellScriptLoader("bfa_spell_gatling_gun") { }

    class bfa_spell_gatling_gun_AuraScript : public AuraScript
    {
        PrepareAuraScript(bfa_spell_gatling_gun_AuraScript);

        float orientation;

        void OnPeriodic(AuraEffect const* aurEff)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (aurEff->GetTickNumber() == 1)
            {
                caster->SetOrientation(0.0f);
                orientation = 5.76f;
                caster->SetOrientation(orientation);
                caster->SetFacingTo(orientation, true);
                CastOnCone();
            }
            else if (aurEff->GetTickNumber() == 2)
            {
                caster->SetOrientation(0.0f);
                orientation = 0.60f;
                caster->SetOrientation(orientation);
                caster->SetFacingTo(orientation, true);
                CastOnCone();
            }
            else if (aurEff->GetTickNumber() == 3)
            {
                caster->SetOrientation(0.0f);
                orientation = 1.61f;
                caster->SetOrientation(orientation);
                caster->SetFacingTo(orientation, true);
                CastOnCone();
            }
            else if (aurEff->GetTickNumber() == 4)
            {
                caster->SetOrientation(0.0f);
                orientation = 2.71f;
                caster->SetOrientation(orientation);
                caster->SetFacingTo(orientation, true);
                CastOnCone();
            }
            else if (aurEff->GetTickNumber() == 5)
            {
                caster->SetOrientation(0.0f);
                orientation = 3.87f;
                caster->SetOrientation(orientation);
                caster->SetFacingTo(orientation, true);
                CastOnCone();
            }
            else if (aurEff->GetTickNumber() == 6)
            {
                caster->SetOrientation(0.0f);
                orientation = 4.86f;
                caster->SetOrientation(orientation);
                caster->SetFacingTo(orientation, true);
                CastOnCone();
                this->Remove();
            }
        }

        void CastOnCone()
        {
            std::list<Player*> playerList;
            GetCaster()->GetPlayerListInGrid(playerList, 100.0f);
            if (playerList.size())
            {
                for (auto player : playerList)
                {
                    if (GetCaster()->isInFront(player, 3.14f / 4))
                    {
                        if (Aura* tmp = GetCaster()->AddAura(SPELL_GATLING_GUN_DAMAGE, player))
                        {
                            tmp->SetMaxDuration(2000);
                            tmp->SetDuration(2000);
                        }
                    }
                }
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(bfa_spell_gatling_gun_AuraScript::OnPeriodic, EFFECT_2, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new bfa_spell_gatling_gun_AuraScript;
    }
};

void AddSC_boss_mogulrazdunk()
{
    new bfa_boss_mogul_razdunk();
    
    new bfa_npc_boomba();
    new bfa_npc_homing_missile();
    new bfa_npc_venture_skyscorcher();

    new bfa_spell_gatling_gun();
}
