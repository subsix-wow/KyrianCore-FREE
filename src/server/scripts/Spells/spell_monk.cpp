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

/*
 * Scripts for spells with SPELLFAMILY_MONK and SPELLFAMILY_GENERIC spells used by monk players.
 * Scriptnames of files in this file should be prefixed with "spell_monk_".
 */

#include "AreaTrigger.h"
#include "AreaTriggerAI.h"
#include "GridNotifiers.h"
#include "ScriptMgr.h"
#include "DB2Stores.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "Unit.h"

enum MonkSpells
{
    SPELL_MONK_CRACKLING_JADE_LIGHTNING_CHANNEL         = 117952,
    SPELL_MONK_CRACKLING_JADE_LIGHTNING_CHI_PROC        = 123333,
    SPELL_MONK_CRACKLING_JADE_LIGHTNING_KNOCKBACK       = 117962,
    SPELL_MONK_CRACKLING_JADE_LIGHTNING_KNOCKBACK_CD    = 117953,
    SPELL_MONK_PROVOKE_SINGLE_TARGET                    = 116189,
    SPELL_MONK_PROVOKE_AOE                              = 118635,
    SPELL_MONK_SOOTHING_MIST                            = 115175,
    SPELL_MONK_STANCE_OF_THE_SPIRITED_CRANE             = 154436,
    SPELL_MONK_STAGGER_DAMAGE_AURA                      = 124255,
    SPELL_MONK_STAGGER_HEAVY                            = 124273,
    SPELL_MONK_STAGGER_LIGHT                            = 124275,
    SPELL_MONK_STAGGER_MODERATE                         = 124274,
    SPELL_MONK_SURGING_MIST_HEAL                        = 116995,
    SPELL_MONK_ROLL = 109132,
    SPELL_MONK_ROLL_ANIMATION = 111396,
    SPELL_MONK_ROLL_BACKWARD = 109131,
    SPELL_MONK_ROLL_TRIGGER = 107427,
    SPELL_MONK_ITEM_PVP_GLOVES_BONUS = 124489,
    SPELL_MONK_ENHANCED_ROLL = 157361,
    SPELL_MONK_CHI_WAVE_HEAL = 132463,
    SPELL_MONK_CHI_BURST_HEAL = 130654,
    SPELL_MONK_CHI_BURST_DAMAGE = 148135,
    SPELL_MONK_MANA_TEA_STACKS = 115867,
    SPELL_MONK_PLUS_ONE_MANA_TEA = 123760,
    SPELL_SPIRIT_OF_THE_CRANE = 210802,
    SPELL_MONK_SPIRIT_OF_THE_CRANE_AURA = 210802,
    SPELL_MONK_SPIRIT_OF_THE_CRANE_MANA = 210803,
    SPELL_MONK_BLACKOUT_KICK_TRIGGERED = 228649,
    SPELL_MONK_WAY_OF_THE_CRANE_HEAL = 216161,
    SPELL_MONK_POWER_STRIKES_AURA = 129914,
    SPELL_MONK_TOUCH_OF_DEATH = 115080,
    SPELL_MONK_TOUCH_OF_DEATH_DAMAGE = 229980,
    SPELL_MONK_TOUCH_OF_DEATH_AMPLIFIER = 271232,
    SPELL_MONK_JADE_LIGHTNING_ENERGIZE = 123333,
    SPELL_MONK_TRANSCENDENCE_CLONE_TARGET = 119051,
    SPELL_MONK_TRANSCENDENCE_VISUAL = 119053,
    SPELL_FOCUSED_THUNDER_TALENT = 197895,
    MONK_NPC_JADE_SERPENT_STATUE = 60849,
    SPELL_MONK_THUNDER_FOCUS_TEA = 116680,
};

#define MONK_TRANSCENDENCE_GUID "MONK_TRANSCENDENCE_GUID"

// Crackling Jade Lightning - 117952
class spell_monk_crackling_jade_lightning : public SpellScriptLoader
{
public:
    spell_monk_crackling_jade_lightning() : SpellScriptLoader("spell_monk_crackling_jade_lightning") { }

    class spell_monk_crackling_jade_lightning_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_monk_crackling_jade_lightning_AuraScript);

        void OnTick(AuraEffect const* /* aurEff */)
        {
            if (Unit* caster = GetCaster())
                if (roll_chance_i(25))
                    caster->CastSpell(caster, SPELL_MONK_JADE_LIGHTNING_ENERGIZE, true);
        }

        void OnProc(AuraEffect* /* aurEff */, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            if (!GetCaster())
                return;

            if (eventInfo.GetActor()->GetGUID() != GetTarget()->GetGUID())
                return;

            /*if (GetCaster()->ToPlayer())
            {
                ;
            }*/
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_monk_crackling_jade_lightning_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            OnEffectProc += AuraEffectProcFn(spell_monk_crackling_jade_lightning_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_monk_crackling_jade_lightning_AuraScript();
    }
};


// 117959 - Crackling Jade Lightning
class spell_monk_crackling_jade_lightning_knockback_proc_aura : public AuraScript
{
    PrepareAuraScript(spell_monk_crackling_jade_lightning_knockback_proc_aura);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo(
        {
            SPELL_MONK_CRACKLING_JADE_LIGHTNING_KNOCKBACK,
            SPELL_MONK_CRACKLING_JADE_LIGHTNING_KNOCKBACK_CD
        });
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (GetTarget()->HasAura(SPELL_MONK_CRACKLING_JADE_LIGHTNING_KNOCKBACK_CD))
            return false;

        if (eventInfo.GetActor()->HasAura(SPELL_MONK_CRACKLING_JADE_LIGHTNING_CHANNEL, GetTarget()->GetGUID()))
            return false;

        Spell* currentChanneledSpell = GetTarget()->GetCurrentSpell(CURRENT_CHANNELED_SPELL);
        if (!currentChanneledSpell || currentChanneledSpell->GetSpellInfo()->Id != SPELL_MONK_CRACKLING_JADE_LIGHTNING_CHANNEL)
            return false;

        return true;
    }

    void HandleProc(AuraEffect* /*aurEff*/, ProcEventInfo& eventInfo)
    {
        GetTarget()->CastSpell(eventInfo.GetActor(), SPELL_MONK_CRACKLING_JADE_LIGHTNING_KNOCKBACK, TRIGGERED_FULL_MASK);
        GetTarget()->CastSpell(GetTarget(), SPELL_MONK_CRACKLING_JADE_LIGHTNING_KNOCKBACK_CD, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_monk_crackling_jade_lightning_knockback_proc_aura::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_monk_crackling_jade_lightning_knockback_proc_aura::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 115546 - Provoke
class spell_monk_provoke : public SpellScript
{
    PrepareSpellScript(spell_monk_provoke);

    static uint32 const BlackOxStatusEntry = 61146;

    bool Validate(SpellInfo const* spellInfo) override
    {
        if (!(spellInfo->GetExplicitTargetMask() & TARGET_FLAG_UNIT_MASK)) // ensure GetExplTargetUnit() will return something meaningful during CheckCast
            return false;
        return ValidateSpellInfo(
        {
            SPELL_MONK_PROVOKE_SINGLE_TARGET,
            SPELL_MONK_PROVOKE_AOE
        });
    }

    SpellCastResult CheckExplicitTarget()
    {
        if (GetExplTargetUnit()->GetEntry() != BlackOxStatusEntry)
        {
            SpellInfo const* singleTarget = sSpellMgr->AssertSpellInfo(SPELL_MONK_PROVOKE_SINGLE_TARGET, GetCastDifficulty());
            SpellCastResult singleTargetExplicitResult = singleTarget->CheckExplicitTarget(GetCaster(), GetExplTargetUnit());
            if (singleTargetExplicitResult != SPELL_CAST_OK)
                return singleTargetExplicitResult;
        }
        else if (GetExplTargetUnit()->GetOwnerGUID() != GetCaster()->GetGUID())
            return SPELL_FAILED_BAD_TARGETS;

        return SPELL_CAST_OK;
    }

    void HandleDummy(SpellEffIndex effIndex)
    {
        PreventHitDefaultEffect(effIndex);
        if (GetHitUnit()->GetEntry() != BlackOxStatusEntry)
            GetCaster()->CastSpell(GetHitUnit(), SPELL_MONK_PROVOKE_SINGLE_TARGET, true);
        else
            GetCaster()->CastSpell(GetHitUnit(), SPELL_MONK_PROVOKE_AOE, true);
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_monk_provoke::CheckExplicitTarget);
        OnEffectHitTarget += SpellEffectFn(spell_monk_provoke::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// Utility for stagger scripts
Aura* FindExistingStaggerEffect(Unit* unit)
{
    if (Aura* auraLight = unit->GetAura(SPELL_MONK_STAGGER_LIGHT))
        return auraLight;

    if (Aura* auraModerate = unit->GetAura(SPELL_MONK_STAGGER_MODERATE))
        return auraModerate;

    if (Aura* auraHeavy = unit->GetAura(SPELL_MONK_STAGGER_HEAVY))
        return auraHeavy;

    return nullptr;
}

static constexpr SpellEffIndex AuraStaggerEffectTick = EFFECT_0;
static constexpr SpellEffIndex AuraStaggerEffectTotal = EFFECT_1;

// 115069 - Stagger
class spell_monk_stagger : public AuraScript
{
    PrepareAuraScript(spell_monk_stagger);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_MONK_STAGGER_LIGHT, SPELL_MONK_STAGGER_MODERATE, SPELL_MONK_STAGGER_HEAVY });
    }

    void AbsorbNormal(AuraEffect* /*aurEff*/, DamageInfo& dmgInfo, uint32& /*absorbAmount*/)
    {
        Absorb(dmgInfo, 1.0f);
    }

    void AbsorbMagic(AuraEffect* /*aurEff*/, DamageInfo& dmgInfo, uint32& /*absorbAmount*/)
    {
        AuraEffect const* effect = GetEffect(EFFECT_4);
        if (!effect)
            return;

        Absorb(dmgInfo, float(effect->GetAmount()) / 100.0f);
    }

    void Absorb(DamageInfo& dmgInfo, float multiplier)
    {
        // Prevent default action (which would remove the aura)
        PreventDefaultAction();

        // make sure damage doesn't come from stagger damage spell SPELL_MONK_STAGGER_DAMAGE_AURA
        if (SpellInfo const* dmgSpellInfo = dmgInfo.GetSpellInfo())
            if (dmgSpellInfo->Id == SPELL_MONK_STAGGER_DAMAGE_AURA)
                return;

        AuraEffect const* effect = GetEffect(AuraStaggerEffectTick);
        if (!effect)
            return;

        Unit* target = GetTarget();
        float agility = target->GetStat(STAT_AGILITY);
        float base = CalculatePct(agility, float(effect->GetAmount()));
        float K = sDB2Manager.EvaluateExpectedStat(ExpectedStatType::ArmorConstant, target->getLevel(), -2, 0, Classes(target->getClass()));

        float newAmount = (base / (base + K));
        newAmount *= multiplier;

        // Absorb X percentage of the damage
        float absorbAmount = float(dmgInfo.GetDamage()) * newAmount;
        if (absorbAmount > 0)
        {
            dmgInfo.AbsorbDamage(absorbAmount);

            // Cast stagger and make it tick on each tick
            AddAndRefreshStagger(absorbAmount);
        }
    }

    void Register() override
    {
        OnEffectAbsorb += AuraEffectAbsorbFn(spell_monk_stagger::AbsorbNormal, EFFECT_1);
        OnEffectAbsorb += AuraEffectAbsorbFn(spell_monk_stagger::AbsorbMagic, EFFECT_2);
    }

private:
    void AddAndRefreshStagger(float amount)
    {
        Unit* target = GetTarget();
        if (Aura* auraStagger = FindExistingStaggerEffect(target))
        {
            AuraEffect* effStaggerRemaining = auraStagger->GetEffect(AuraStaggerEffectTotal);
            if (!effStaggerRemaining)
                return;

            float newAmount = effStaggerRemaining->GetAmount() + amount;
            uint32 spellId = GetStaggerSpellId(target, newAmount);
            if (spellId == effStaggerRemaining->GetSpellInfo()->Id)
            {
                auraStagger->RefreshDuration();
                effStaggerRemaining->ChangeAmount(newAmount, false, true /* reapply */);
            }
            else
            {
                // amount changed the stagger type so we need to change the stagger amount (e.g. from medium to light)
                GetTarget()->RemoveAura(auraStagger);
                AddNewStagger(target, spellId, newAmount);
            }
        }
        else
            AddNewStagger(target, GetStaggerSpellId(target, amount), amount);
    }

    uint32 GetStaggerSpellId(Unit* unit, float amount)
    {
        const float StaggerHeavy = 0.6f;
        const float StaggerModerate = 0.3f;

        float staggerPct = amount / float(unit->GetMaxHealth());
        return (staggerPct >= StaggerHeavy) ? SPELL_MONK_STAGGER_HEAVY :
            (staggerPct >= StaggerModerate) ? SPELL_MONK_STAGGER_MODERATE :
            SPELL_MONK_STAGGER_LIGHT;
    }

    void AddNewStagger(Unit* unit, uint32 staggerSpellId, float staggerAmount)
    {
        // We only set the total stagger amount. The amount per tick will be set by the stagger spell script
        unit->CastSpell(unit, staggerSpellId, CastSpellExtraArgs(SPELLVALUE_BASE_POINT1, staggerAmount).SetTriggerFlags(TRIGGERED_FULL_MASK));
    }
};

// 124255 - Stagger - SPELL_MONK_STAGGER_DAMAGE_AURA
class spell_monk_stagger_damage_aura : public AuraScript
{
    PrepareAuraScript(spell_monk_stagger_damage_aura);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_MONK_STAGGER_LIGHT, SPELL_MONK_STAGGER_MODERATE, SPELL_MONK_STAGGER_HEAVY });
    }

    void OnPeriodicDamage(AuraEffect const* aurEff)
    {
        // Update our light/medium/heavy stagger with the correct stagger amount left
        if (Aura* auraStagger = FindExistingStaggerEffect(GetTarget()))
        {
            if (AuraEffect* auraEff = auraStagger->GetEffect(AuraStaggerEffectTotal))
            {
                float total = float(auraEff->GetAmount());
                float tickDamage = float(aurEff->GetDamage());
                auraEff->ChangeAmount(total - tickDamage);
            }
        }
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_monk_stagger_damage_aura::OnPeriodicDamage, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
    }
};

// 124273, 124274, 124275 - Light/Moderate/Heavy Stagger - SPELL_MONK_STAGGER_LIGHT / SPELL_MONK_STAGGER_MODERATE / SPELL_MONK_STAGGER_HEAVY
class spell_monk_stagger_debuff_aura : public AuraScript
{
    PrepareAuraScript(spell_monk_stagger_debuff_aura);

    bool Load() override
    {
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(SPELL_MONK_STAGGER_DAMAGE_AURA, GetCastDifficulty());
        SpellEffectInfo const* effInfo = !spellInfo ? nullptr : spellInfo->GetEffect(EFFECT_0);
        if (!effInfo)
            return false;

        _period = float(effInfo->ApplyAuraPeriod);
        return true;
    }

    void OnReapply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
    {
        // Calculate damage per tick
        float total = float(aurEff->GetAmount());
        float perTick = total * _period / float(GetDuration()); // should be same as GetMaxDuration() TODO: verify

        // Set amount on effect for tooltip
        AuraEffect* effInfo = GetAura()->GetEffect(AuraStaggerEffectTick);
        if (effInfo)
            effInfo->ChangeAmount(perTick);

        // Set amount on damage aura (or cast it if needed)
        CastOrChangeTickDamage(perTick);
    }

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes mode)
    {
        if (mode != AURA_EFFECT_HANDLE_REAL)
            return;

        // Remove damage aura
        GetTarget()->RemoveAura(SPELL_MONK_STAGGER_DAMAGE_AURA);
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectRemoveFn(spell_monk_stagger_debuff_aura::OnReapply, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        AfterEffectRemove += AuraEffectRemoveFn(spell_monk_stagger_debuff_aura::OnRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }

private:
    float _period = 0.0f;

    void CastOrChangeTickDamage(float tickDamage)
    {
        Unit* unit = GetTarget();
        Aura* auraDamage = unit->GetAura(SPELL_MONK_STAGGER_DAMAGE_AURA);
        if (!auraDamage)
        {
            unit->CastSpell(unit, SPELL_MONK_STAGGER_DAMAGE_AURA, true);
            auraDamage = unit->GetAura(SPELL_MONK_STAGGER_DAMAGE_AURA);
        }

        if (auraDamage)
            if (AuraEffect* eff = auraDamage->GetEffect(AuraStaggerEffectTick))
                eff->SetDamage(tickDamage);
    }
};

// 109132 - Roll
class spell_monk_roll : public SpellScriptLoader
{
public:
    spell_monk_roll() : SpellScriptLoader("spell_monk_roll") { }

    class spell_monk_roll_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_roll_SpellScript);

    private:
        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_MONK_ROLL, DIFFICULTY_NONE) ||
                !sSpellMgr->GetSpellInfo(SPELL_MONK_ROLL_TRIGGER, DIFFICULTY_NONE) ||
                !sSpellMgr->GetSpellInfo(SPELL_MONK_ROLL_BACKWARD, DIFFICULTY_NONE))
                return false;
            return true;
        }

        void HandleAfterCast()
        {
            Unit* caster = GetCaster();
            if (!caster || caster->GetTypeId() != TYPEID_PLAYER)
                return;

            if (caster->HasAura(SPELL_MONK_ITEM_PVP_GLOVES_BONUS))
                caster->RemoveAurasByType(SPELL_AURA_MOD_DECREASE_SPEED);
        }

        void HandleDummy()
        {
            if (Unit* caster = GetCaster())
            {
                if (caster->HasUnitMovementFlag(MOVEMENTFLAG_BACKWARD))
                    caster->CastSpell(caster, SPELL_MONK_ROLL_BACKWARD, true);
                else
                    caster->CastSpell(caster, SPELL_MONK_ROLL_TRIGGER, true);
            }
        }

        void Register() override
        {
            AfterCast += SpellCastFn(spell_monk_roll_SpellScript::HandleAfterCast);
            AfterHit += SpellHitFn(spell_monk_roll_SpellScript::HandleDummy);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_monk_roll_SpellScript();
    }
};

// Roll trigger - 107427
class spell_monk_roll_trigger : public SpellScriptLoader
{
public:
    spell_monk_roll_trigger() : SpellScriptLoader("spell_monk_roll_trigger") {}

    class spell_monk_roll_trigger_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_monk_roll_trigger_AuraScript);

        void CalcSpeed(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (caster->HasAura(SPELL_MONK_ENHANCED_ROLL))
                amount = 277;
        }

        void CalcSpeed2(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (!caster->HasAura(SPELL_MONK_ENHANCED_ROLL))
                return;

            amount = 377;
        }

        void SendAmount(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (!caster->HasAura(SPELL_MONK_ENHANCED_ROLL))
                return;

            Aura* aur = GetAura();
            if (!aur)
                return;

            aur->SetMaxDuration(600);
            aur->SetDuration(600);

            if (AuraApplication* aurApp = GetAura()->GetApplicationOfTarget(caster->GetGUID()))
                aurApp->ClientUpdate();
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_monk_roll_trigger_AuraScript::CalcSpeed, EFFECT_0, SPELL_AURA_MOD_SPEED_NO_CONTROL);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_monk_roll_trigger_AuraScript::CalcSpeed2, EFFECT_2, SPELL_AURA_MOD_MINIMUM_SPEED);
            AfterEffectApply += AuraEffectApplyFn(spell_monk_roll_trigger_AuraScript::SendAmount, EFFECT_4, SPELL_AURA_USE_NORMAL_MOVEMENT_SPEED, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_monk_roll_trigger_AuraScript();
    }
};

// Fists of Fury (stun effect) - 120086
class spell_monk_fists_of_fury_stun : public SpellScriptLoader
{
public:
    spell_monk_fists_of_fury_stun() : SpellScriptLoader("spell_monk_fists_of_fury_stun") { }

    class spell_monk_fists_of_fury_stun_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_fists_of_fury_stun_SpellScript);

        void RemoveInvalidTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if(Trinity::UnitAuraCheck(true, GetSpellInfo()->Id));
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_monk_fists_of_fury_stun_SpellScript::RemoveInvalidTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_24);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_monk_fists_of_fury_stun_SpellScript();
    }
};

// Expel Harm - 115072
class spell_monk_expel_harm : public SpellScriptLoader
{
public:
    spell_monk_expel_harm() : SpellScriptLoader("spell_monk_expel_harm") { }

    class spell_monk_expel_harm_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_expel_harm_SpellScript);

        void HandleOnHit()
        {
            if (!GetCaster())
                return;

            if (Player* _player = GetCaster()->ToPlayer())
            {
                std::list<Unit*> targetList;
                _player->GetAttackableUnitListInRange(targetList, 10.0f);

                for (auto itr : targetList)
                {
                    if (_player->IsValidAttackTarget(itr))
                    {
                        float bp = CalculatePct((-GetHitDamage()), 50);
                        _player->CastCustomSpell(115129, SPELLVALUE_BASE_POINT1, true, NULL); 
                    }
                }
            }
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_monk_expel_harm_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_monk_expel_harm_SpellScript();
    }
};
// Chi Wave (healing bolt) - 132464
class spell_monk_chi_wave_healing_bolt : public SpellScriptLoader
{
public:
    spell_monk_chi_wave_healing_bolt() : SpellScriptLoader("spell_monk_chi_wave_healing_bolt") { }

    class spell_monk_chi_wave_healing_bolt_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_chi_wave_healing_bolt_SpellScript);

        void HandleOnHit()
        {
            if (!GetOriginalCaster())
                return;

            if (Player* _player = GetOriginalCaster()->ToPlayer())
                if (Unit* target = GetHitUnit())
                    _player->CastSpell(target, SPELL_MONK_CHI_WAVE_HEAL, true);
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_monk_chi_wave_healing_bolt_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_monk_chi_wave_healing_bolt_SpellScript();
    }
};

// 132464 - Chi Wave (heal missile)
class spell_monk_chi_wave_heal_missile : public SpellScriptLoader
{
public:
    spell_monk_chi_wave_heal_missile() : SpellScriptLoader("spell_monk_chi_wave_heal_missile") {}

    class spell_monk_chi_wave_heal_missile_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_monk_chi_wave_heal_missile_AuraScript);

        void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetTarget();
            if (!target || !caster)
                return;

            caster->CastSpell(target, 132463, true);
            // rerun target selector
            caster->CastCustomSpell(132466, SPELLVALUE_BASE_POINT1, aurEff->GetAmount() - 1, target, true, NULL, aurEff);
        }

        void Register() override
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_monk_chi_wave_heal_missile_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_monk_chi_wave_heal_missile_AuraScript();
    }
};

// 132467 - Chi Wave (damage missile)
class spell_monk_chi_wave_damage_missile : public SpellScriptLoader
{
public:
    spell_monk_chi_wave_damage_missile() : SpellScriptLoader("spell_monk_chi_wave_damage_missile") {}

    class spell_monk_chi_wave_damage_missile_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_monk_chi_wave_damage_missile_AuraScript);

        void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetTarget();
            if (!target || !caster)
                return;

            // rerun target selector
            caster->CastCustomSpell(132466, SPELLVALUE_BASE_POINT1, aurEff->GetAmount() - 1, target, true, NULL, aurEff);
        }

        void Register() override
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_monk_chi_wave_damage_missile_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_monk_chi_wave_damage_missile_AuraScript();
    }
};

// 132466 - Chi Wave (target selector)
class spell_monk_chi_wave_target_selector : public SpellScriptLoader
{
public:
    spell_monk_chi_wave_target_selector() : SpellScriptLoader("spell_monk_chi_wave_target_selector") {}

    class DamageUnitCheck
    {
    public:
        DamageUnitCheck(Unit const* source, float range) : m_source(source), m_range(range) {}
        bool operator()(WorldObject* object)
        {
            Unit* unit = object->ToUnit();
            if (!unit)
                return true;

            if (m_source->IsValidAttackTarget(unit) && unit->isTargetableForAttack() && m_source->IsWithinDistInMap(unit, m_range))
            {
                m_range = m_source->GetDistance(unit);
                return false;
            }

            return true;
        }
    private:
        Unit const* m_source;
        float m_range;
    };

    class HealUnitCheck
    {
    public:
        HealUnitCheck(Unit const* source) : m_source(source) {}
        bool operator()(WorldObject* object)
        {
            Unit* unit = object->ToUnit();
            if (!unit)
                return true;

            if (m_source->IsFriendlyTo(unit))
                return false;

            return true;
        }
    private:
        Unit const* m_source;
    };

    class spell_monk_chi_wave_target_selector_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_chi_wave_target_selector_SpellScript);

        bool Load() override
        {
            m_shouldHeal = true; // just for initializing
            return true;
        }

        void SelectTarget(std::list<WorldObject*>& targets)
        {
            if (targets.empty())
                return;

            SpellInfo const* spellInfo = GetTriggeringSpell();
            if (spellInfo->Id == 132467) // Triggered by damage, so we need heal selector
            {
                targets.remove_if(HealUnitCheck(GetCaster()));
                targets.sort(Trinity::HealthPctOrderPred(false)); // Reverse order due to target is selected via std::list back
                m_shouldHeal = true;
            }
            else if (spellInfo->Id == 132464) // Triggered by heal, so we need damage selector
            {
                targets.remove_if(DamageUnitCheck(GetCaster(), 25.0f));
                m_shouldHeal = false;
            }

            if (targets.empty())
                return;

            WorldObject* target = targets.back();
            if (!target)
                return;

            targets.clear();
            targets.push_back(target);
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (!GetEffectValue()) // Ran out of bounces
                return;

            if (!GetExplTargetUnit() || !GetOriginalCaster())
                return;

            Unit* target = GetHitUnit();
            if (m_shouldHeal)
                GetExplTargetUnit()->CastCustomSpell(132464, SPELLVALUE_BASE_POINT1, GetEffectValue(), target, true, NULL, NULL, GetOriginalCaster()->GetGUID());
            else
                GetExplTargetUnit()->CastCustomSpell(132467, SPELLVALUE_BASE_POINT1, GetEffectValue(), target, true, NULL, NULL, GetOriginalCaster()->GetGUID());
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_monk_chi_wave_target_selector_SpellScript::SelectTarget, EFFECT_1, TARGET_UNIT_DEST_AREA_ENTRY);
            OnEffectHitTarget += SpellEffectFn(spell_monk_chi_wave_target_selector_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
        }

        bool m_shouldHeal;
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_monk_chi_wave_target_selector_SpellScript();
    }
};

// Chi Burst heal - 130654
class spell_monk_chi_burst_heal : public SpellScriptLoader
{
public:
    spell_monk_chi_burst_heal() : SpellScriptLoader("spell_monk_chi_burst_heal") { }

    class spell_monk_chi_burst_heal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_chi_burst_heal_SpellScript);

        void HandleHeal(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            Unit* unit = GetHitUnit();
            if (!caster || !unit)
                return;

            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(SPELL_MONK_CHI_BURST_HEAL, DIFFICULTY_NONE);
            if (!spellInfo)
                return;
            SpellEffectInfo const* effectInfo = spellInfo->GetEffect(EFFECT_0);
            if (!effectInfo)
                return;

            int32 damage = (float)caster->GetTotalAttackPowerValue(BASE_ATTACK) * 4.125f;
            damage = caster->SpellDamageBonusDone(unit, spellInfo, damage, HEAL, effectInfo);
            damage = unit->SpellDamageBonusTaken(caster, spellInfo, damage, HEAL, effectInfo);

            SetHitHeal(damage);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_monk_chi_burst_heal_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_monk_chi_burst_heal_SpellScript();
    }
};

//123986, 5300
struct at_monk_chi_burst : AreaTriggerAI
{
    at_monk_chi_burst(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger) { }

    void OnUnitEnter(Unit* target) override
    {
        if (!at->GetCaster())
            return;

        if (at->GetCaster()->IsValidAssistTarget(target))
            at->GetCaster()->CastSpell(target, SPELL_MONK_CHI_BURST_HEAL, true);

        if (at->GetCaster()->IsValidAttackTarget(target))
            at->GetCaster()->CastSpell(target, SPELL_MONK_CHI_BURST_DAMAGE, true);
    }
};

// Brewing : Mana Tea - 123766
class spell_monk_mana_tea_stacks : public SpellScriptLoader
{
public:
    spell_monk_mana_tea_stacks() : SpellScriptLoader("spell_monk_mana_tea_stacks") { }

    class spell_monk_mana_tea_stacks_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_monk_mana_tea_stacks_AuraScript);

        uint32 chiConsumed;

        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            chiConsumed = 0;
        }

        void SetData(uint32 /* type */, uint32 data)
        {
            while ((chiConsumed += data) >= 4)
            {
                chiConsumed = 0;
                data = data > 4 ? data - 4 : 0;

                if (GetCaster())
                {
                    GetCaster()->CastSpell(GetCaster(), SPELL_MONK_MANA_TEA_STACKS, true);
                    GetCaster()->CastSpell(GetCaster(), SPELL_MONK_PLUS_ONE_MANA_TEA, true);
                }
            }
        }

        void Register() override
        {
            AfterEffectApply += AuraEffectApplyFn(spell_monk_mana_tea_stacks_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_monk_mana_tea_stacks_AuraScript();
    }
};

// 210802 - Spirit of the Crane (Passive)
class spell_monk_spirit_of_the_crane_passive : public AuraScript
{
    PrepareAuraScript(spell_monk_spirit_of_the_crane_passive);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo(
            {
                SPELL_MONK_SPIRIT_OF_THE_CRANE_MANA,
                SPELL_MONK_BLACKOUT_KICK_TRIGGERED
            });
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetSpellInfo()->Id != SPELL_MONK_BLACKOUT_KICK_TRIGGERED)
            return false;
        return true;
    }

    void HandleProc(AuraEffect* /*aurEff*/, ProcEventInfo& /*eventInfo*/)
    {
        // TODO: Basepoints can be float now... this is 1 but needs to be lower.
        GetTarget()->CastSpell(GetTarget(), SPELL_MONK_SPIRIT_OF_THE_CRANE_MANA, true);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_monk_spirit_of_the_crane_passive::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_monk_spirit_of_the_crane_passive::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 216113 - Way of the crane (Passive)
class spell_monk_way_of_the_crane : public AuraScript
{
    PrepareAuraScript(spell_monk_way_of_the_crane);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo(
            {
                SPELL_MONK_WAY_OF_THE_CRANE_HEAL
            });
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (DamageInfo* dInfo = eventInfo.GetDamageInfo())
            if (dInfo->GetDamage() > 0)
                return true;
        return false;
    }

    void HandleProc(AuraEffect* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();
        int32 damage = eventInfo.GetDamageInfo()->GetDamage();
        AddPct(damage, aurEff->GetAmount());
        eventInfo.GetActor()->CastCustomSpell(SPELL_MONK_WAY_OF_THE_CRANE_HEAL, SPELLVALUE_BASE_POINT0, damage, eventInfo.GetActor(), true);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_monk_way_of_the_crane::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_monk_way_of_the_crane::HandleProc, EFFECT_3, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

// 210802 Spirit of the Crane
// 228649 Blackout Kick triggered by Teachings of the Monastery
class bfa_spell_blackout_kick_spirit_of_the_crane_talent : public SpellScriptLoader
{
public:
    bfa_spell_blackout_kick_spirit_of_the_crane_talent() : SpellScriptLoader("bfa_spell_blackout_kick_spirit_of_the_crane_talent") { }

    class bfa_spell_blackout_kick_spirit_of_the_crane_talent_SpellScript : public SpellScript
    {
        PrepareSpellScript(bfa_spell_blackout_kick_spirit_of_the_crane_talent_SpellScript);

        void Dummy()
        {
            Player* caster = GetCaster()->ToPlayer();
            if (!caster)
                return;

            if (caster->HasAura(SPELL_SPIRIT_OF_THE_CRANE))
            {
                caster->SetPower(POWER_MANA, caster->GetPower(POWER_MANA) + ((caster->GetMaxPower(POWER_MANA) * 0.65f) / 100));
            }
        }
        void Register()
        {
            AfterCast += SpellCastFn(bfa_spell_blackout_kick_spirit_of_the_crane_talent_SpellScript::Dummy);
        }

    };

    SpellScript* GetSpellScript() const
    {
        return new bfa_spell_blackout_kick_spirit_of_the_crane_talent_SpellScript();
    }
};

// 100780
class spell_monk_tiger_palm : public SpellScript
{
    PrepareSpellScript(spell_monk_tiger_palm);

    void HandleHit(SpellEffIndex /*effIndex*/)
    {
        if (Aura* powerStrikes = GetCaster()->GetAura(SPELL_MONK_POWER_STRIKES_AURA))
        {
            SetEffectValue(GetEffectValue() + powerStrikes->GetEffect(EFFECT_0)->GetBaseAmount());
            powerStrikes->Remove();
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_monk_tiger_palm::HandleHit, EFFECT_1, SPELL_EFFECT_ENERGIZE);
    }
};

class spell_monk_life_cocoon : public SpellScriptLoader
{
public:
    spell_monk_life_cocoon() : SpellScriptLoader("spell_monk_life_cocoon") { }

    class spell_monk_life_cocoon_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_monk_life_cocoon_AuraScript);

        void CalcAbsorb(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
        {
            if (!GetCaster())
                return;
            Unit* caster = GetCaster();

            //Formula:  [(((Spell power * 11) + 0)) * (1 + Versatility)]
            //Simplified to : [(Spellpower * 11)]
            //Versatility will be taken into account at a later date.
            amount += caster->GetTotalSpellPowerValue(GetSpellInfo()->GetSchoolMask(), true) * 11;
            canBeRecalculated = false;
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_monk_life_cocoon_AuraScript::CalcAbsorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_monk_life_cocoon_AuraScript();
    }
};

// Touch of Death - 115080
class spell_monk_touch_of_death : public AuraScript
{
    PrepareAuraScript(spell_monk_touch_of_death);

    void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        if (Unit* caster = GetCaster())
            if (SpellEffectInfo const* effInfo = GetAura()->GetSpellInfo()->GetEffect(EFFECT_1))
            {
                amount = int32(caster->CountPctFromMaxHealth(effInfo->CalcValue()));
                const_cast<AuraEffect*>(aurEff)->SetDamage(amount);
            }
    }

    void OnTick(AuraEffect const* aurEff)
    {
        if (Unit* caster = GetCaster())
        {
            int32 damage = aurEff->GetAmount();

            // Damage reduced to Players, need to check reduction value
            if (GetTarget()->GetTypeId() == TYPEID_PLAYER)
                damage /= 2;

            caster->CastCustomSpell(SPELL_MONK_TOUCH_OF_DEATH_DAMAGE, SPELLVALUE_BASE_POINT0, damage, GetTarget());
        }
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_monk_touch_of_death::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_monk_touch_of_death::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

// 271232 - Touch of Death Amplifier - Triggers: 271233 on ToD Cast via proc
class spell_monk_touch_of_death_passive : public AuraScript
{
    PrepareAuraScript(spell_monk_touch_of_death_passive);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo(
            {
                SPELL_MONK_TOUCH_OF_DEATH
            });
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetSpellInfo()->Id != SPELL_MONK_TOUCH_OF_DEATH)
            return false;
        return true;
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_monk_touch_of_death_passive::CheckProc);
    }
};

// 271233 - Amplifier (Applied with ToD)
class spell_monk_touch_of_death_amplifier : public AuraScript
{
    PrepareAuraScript(spell_monk_touch_of_death_amplifier);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo(
            {
                SPELL_MONK_TOUCH_OF_DEATH,
                SPELL_MONK_TOUCH_OF_DEATH_AMPLIFIER
            });
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0;
    }

    void HandleProc(AuraEffect* /*aurEff*/, ProcEventInfo& eventInfo)
    {
        if (AuraEffect* aurEff = GetTarget()->GetAuraEffect(SPELL_MONK_TOUCH_OF_DEATH, EFFECT_0))
            if (AuraEffect* aurEffAmplifier = eventInfo.GetActor()->GetAuraEffect(SPELL_MONK_TOUCH_OF_DEATH_AMPLIFIER, EFFECT_0))
            {
                int32 damage = aurEff->GetAmount() + CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEffAmplifier->GetAmount());
                aurEff->SetDamage(damage);
                aurEff->SetAmount(damage);
            }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_monk_touch_of_death_amplifier::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_monk_touch_of_death_amplifier::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 101643
class spell_monk_transcendence : public SpellScript
{
public:
    PrepareSpellScript(spell_monk_transcendence);

    void HandleSummon(Creature* creature)
    {
        DespawnSpirit(GetCaster());
        GetCaster()->CastSpell(creature, SPELL_MONK_TRANSCENDENCE_CLONE_TARGET, true);
        creature->CastSpell(creature, SPELL_MONK_TRANSCENDENCE_VISUAL, true);
        creature->SetAIAnimKitId(2223); // Sniff Data
        creature->SetDisableGravity(true);
        creature->SetControlled(true, UNIT_STATE_ROOT);
     //   GetCaster()->Variables.Set(MONK_TRANSCENDENCE_GUID, creature->GetGUID());
    }

    static Creature* GetSpirit(Unit* caster)
    {
      //  ObjectGuid spiritGuid = caster->Variables.GetValue<ObjectGuid>(MONK_TRANSCENDENCE_GUID, ObjectGuid());

       // if (spiritGuid.IsEmpty())
            return nullptr;

    //    return ObjectAccessor::GetCreature(*caster, spiritGuid);
    }

    static void DespawnSpirit(Unit* caster)
    {
        // Remove previous one if any
        if (Creature* spirit = GetSpirit(caster))
            spirit->DespawnOrUnsummon();

     //   caster->Variables.Remove(MONK_TRANSCENDENCE_GUID);
    }

    void Register() override
    {
        OnEffectSummon += SpellOnEffectSummonFn(spell_monk_transcendence::HandleSummon);
    }
};

// 101643
class aura_monk_transcendence : public AuraScript
{
    PrepareAuraScript(aura_monk_transcendence);

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        spell_monk_transcendence::DespawnSpirit(GetTarget());
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(aura_monk_transcendence::OnRemove, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// 119996 - Transcendence: Transfer
class spell_monk_transcendence_transfer : public SpellScript
{
    PrepareSpellScript(spell_monk_transcendence_transfer);

    SpellCastResult CheckCast()
    {
        Unit* caster = GetCaster();

        if (!caster)
            return SPELL_FAILED_ERROR;

        Unit* spirit = spell_monk_transcendence::GetSpirit(caster);
        if (!spirit)
        {
            SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_YOU_HAVE_NO_SPIRIT_ACTIVE);
            return SPELL_FAILED_CUSTOM_ERROR;
        }

        if (!spirit->IsWithinDist(caster, GetSpellInfo()->GetMaxRange(true, caster, GetSpell())))
            return SPELL_FAILED_OUT_OF_RANGE;

        return SPELL_CAST_OK;
    }

    void HandleOnCast()
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        Unit* spirit = spell_monk_transcendence::GetSpirit(caster);
        if (!spirit)
            return;

        caster->NearTeleportTo(*spirit, true);
        spirit->NearTeleportTo(*caster, true);
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_monk_transcendence_transfer::CheckCast);
        OnCast += SpellCastFn(spell_monk_transcendence_transfer::HandleOnCast);
    }
};

// 116680 Thunder Focus Tea
// 197895 Focused Thunder
class bfa_spell_focused_thunder_talent_thunder_focus_tea : public SpellScriptLoader
{
public:
    bfa_spell_focused_thunder_talent_thunder_focus_tea() : SpellScriptLoader("bfa_spell_focused_thunder_talent_thunder_focus_tea") { }

    class bfa_spell_focused_thunder_talent_thunder_focus_tea_AuraScript : public AuraScript
    {
        PrepareAuraScript(bfa_spell_focused_thunder_talent_thunder_focus_tea_AuraScript);

        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster()->ToPlayer();
            if (!caster)
                return;

            if (caster->HasAura(SPELL_FOCUSED_THUNDER_TALENT))
            {
                if (Aura* thunder = caster->GetAura(SPELL_MONK_THUNDER_FOCUS_TEA))
                    thunder->SetStackAmount(2);
            }
        }

        void Register()
        {
            OnEffectApply += AuraEffectApplyFn(bfa_spell_focused_thunder_talent_thunder_focus_tea_AuraScript::OnApply, EFFECT_0, SPELL_AURA_ADD_FLAT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
        }

    };

    AuraScript* GetAuraScript() const
    {
        return new bfa_spell_focused_thunder_talent_thunder_focus_tea_AuraScript();
    }
};

//115313 - Summon Jade Serpent Statue
class spell_monk_jade_serpent_statue : public SpellScriptLoader
{
public:
    spell_monk_jade_serpent_statue() : SpellScriptLoader("spell_monk_jade_serpent_statue") { }

    class spell_monk_jade_serpent_statue_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_jade_serpent_statue_SpellScript);

        void HandleSummon()
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            Player* player = caster->ToPlayer();
            if (!player)
                return;

            std::list<Creature*> serpentStatueList;
            player->GetCreatureListWithEntryInGrid(serpentStatueList, MONK_NPC_JADE_SERPENT_STATUE, 500.0f);

            for (std::list<Creature*>::iterator i = serpentStatueList.begin(); i != serpentStatueList.end(); ++i)
            {
                Unit* owner = (*i)->GetOwner();

                if (owner && owner == player && (*i)->IsSummon())
                    continue;

                i = serpentStatueList.erase(i);
            }

            if ((int32)serpentStatueList.size() >= 1)
                serpentStatueList.back()->ToTempSummon()->UnSummon();
        }

        void Register() override
        {
            OnCast += SpellCastFn(spell_monk_jade_serpent_statue_SpellScript::HandleSummon);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_monk_jade_serpent_statue_SpellScript();
    }
};

//60849
struct npc_monk_jade_serpent_statue : public ScriptedAI
{
    npc_monk_jade_serpent_statue(Creature* c) : ScriptedAI(c) { }

    void UpdateAI(uint32 diff) override
    {
        if (Unit* owner = me->GetOwner())
        {
            if (Player* player = owner->ToPlayer())
            {
                if (player->getClass() != CLASS_MONK)
                    return;
                else
                {
                    if (player->GetSpecializationId() != TALENT_SPEC_MONK_MISTWEAVER && me->IsInWorld())
                        me->DespawnOrUnsummon();
                }
            }
        }
    }
};


void AddSC_monk_spell_scripts()
{
    new spell_monk_crackling_jade_lightning();
    RegisterAuraScript(spell_monk_crackling_jade_lightning_knockback_proc_aura);
    RegisterSpellScript(spell_monk_provoke);
    RegisterAuraScript(spell_monk_stagger);
    RegisterAuraScript(spell_monk_stagger_damage_aura);
    RegisterAuraScript(spell_monk_stagger_debuff_aura);
    new spell_monk_roll();
    new spell_monk_roll_trigger();
    new spell_monk_chi_wave_heal_missile();
    new spell_monk_chi_wave_healing_bolt();
    new spell_monk_chi_burst_heal();
    RegisterAreaTriggerAI(at_monk_chi_burst);
    new spell_monk_mana_tea_stacks();
    new bfa_spell_blackout_kick_spirit_of_the_crane_talent();
    RegisterAuraScript(spell_monk_spirit_of_the_crane_passive);
    RegisterAuraScript(spell_monk_way_of_the_crane);
    RegisterSpellScript(spell_monk_tiger_palm);
    new spell_monk_life_cocoon();
    RegisterAuraScript(spell_monk_touch_of_death_amplifier);
    RegisterAuraScript(spell_monk_touch_of_death_passive);
    RegisterAuraScript(spell_monk_touch_of_death);
    RegisterSpellScript(spell_monk_transcendence);
    RegisterSpellScript(spell_monk_transcendence_transfer);
    new bfa_spell_focused_thunder_talent_thunder_focus_tea();
    new spell_monk_jade_serpent_statue();
    RegisterCreatureAI(npc_monk_jade_serpent_statue);
}
