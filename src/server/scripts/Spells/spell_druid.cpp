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
 * Scripts for spells with SPELLFAMILY_DRUID and SPELLFAMILY_GENERIC spells used by druid players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_dru_".
 */

#include "ScriptMgr.h"
#include "Containers.h"
#include "Map.h"
#include "Player.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellHistory.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "AreaTrigger.h"
#include "AreaTriggerAI.h"
#include "ScriptedCreature.h"


enum DruidSpells
{
    SPELL_DRUID_BALANCE_T10_BONUS              = 70718,
    SPELL_DRUID_BALANCE_T10_BONUS_PROC         = 70721,
    SPELL_DRUID_MOONFIRE_CAT = 155625,
    SPELL_DRUID_BLESSING_OF_CENARIUS           = 40452,
    SPELL_DRUID_BLESSING_OF_ELUNE              = 40446,
    SPELL_DRUID_BLESSING_OF_REMULOS            = 40445,
    SPELL_DRUID_BLESSING_OF_THE_CLAW           = 28750,
    SPELL_DRUID_BLOOD_FRENZY_AURA              = 203962,
    SPELL_DRUID_BLOOD_FRENZY_RAGE_GAIN         = 203961,
    SPELL_DRUID_BRAMBLES_DAMAGE_AURA           = 213709,
    SPELL_DRUID_BRAMBLES_PASSIVE               = 203953,
    SPELL_DRUID_BRAMBLES_REFLECT               = 203958,
    SPELL_DRUID_BRISTLING_FUR_GAIN_RAGE        = 204031,
    SPELL_DRUID_SWIPE_CAT = 106785,
    SPELL_DRUID_GLYPH_OF_STARS = 114301,
    SPELL_DRU_STARLORD_BUFF = 279709,
    SPELL_DRUID_CHOSEN_OF_ELUNE = 102560,
    SPELL_DRUID_BLUE_COLOR = 108268,
    SPELL_DRUID_SHADOWY_GHOST = 165114,
    SPELL_DRU_GLYPH_OF_REJUVENATION = 17076,
    SPELL_DRU_GLYPH_OF_REJUVENATION_EFFECT = 96206,
    SPELL_DRU_GERMINATION = 155675,
    SPELL_DRU_GERMINATION_HOT = 155777,
    SPELL_DRUID_REJUVENATION = 774,
    SPELL_DRU_PREDATOR = 202021,
    SPELL_DRU_CULTIVATION = 200390,
    SPELL_DRU_CULTIVATION_HEAL = 200389,
    SPELL_DRUID_GLYPH_OF_TYPHOON = 62135,
   
    SPELL_DRUID_SHRED = 5221,
    SPELL_DRU_MASS_ENTANGLEMENT = 102359,
    SPELL_DRUID_EARTHWARDEN_AURA               = 203975,
    SPELL_DRUID_ECLIPSE_DUMMY                  = 79577,
    SPELL_DRUID_ECLIPSE_LUNAR_AURA             = 48518,
    SPELL_DRUID_STARLORD_DUMMY = 202345,
    SPELL_DRUID_STARLORD_SOLAR = 202416,
    SPELL_DRUID_STARLORD_LUNAR = 202423,
    SPELL_DRUID_ECLIPSE_LUNAR_SPELL_CNT        = 326055,
    SPELL_DRUID_ECLIPSE_OOC                    = 329910,
    SPELL_DRUID_ECLIPSE_SOLAR_AURA             = 48517,
    SPELL_DRUID_ECLIPSE_SOLAR_SPELL_CNT        = 326053,
    SPELL_DRUID_EXHILARATE                     = 28742,
    SPELL_DRUID_FERAL_CHARGE_BEAR              = 16979,
    SPELL_DRUID_FERAL_CHARGE_CAT               = 49376,
    SPELL_DRUID_FORM_AQUATIC_PASSIVE           = 276012,
    SPELL_DRUID_FORM_AQUATIC                   = 1066,
    SPELL_DRUID_FORM_FLIGHT                    = 33943,
    SPELL_DRUID_FORM_STAG                      = 165961,
    SPELL_DRUID_FORM_SWIFT_FLIGHT              = 40120,
    SPELL_DRUID_FORMS_TRINKET_BEAR             = 37340,
    SPELL_DRUID_FORMS_TRINKET_CAT              = 37341,
    SPELL_DRUID_FORMS_TRINKET_MOONKIN          = 37343,
    SPELL_DRUID_FORMS_TRINKET_NONE             = 37344,
    SPELL_DRUID_FORMS_TRINKET_TREE             = 37342,
    SPELL_DRUID_GALACTIC_GUARDIAN_AURA         = 213708,
    SPELL_DRUID_GORE_PROC                      = 93622,
    SPELL_DRUID_IDOL_OF_FERAL_SHADOWS          = 34241,
    SPELL_DRUID_IDOL_OF_WORSHIP                = 60774,
    SPELL_DRUID_INCARNATION_KING_OF_THE_JUNGLE = 102543,
    SPELL_DRU_PHOTOSYNTHESIS = 274902,
    SPELL_DRU_PHOTOSYNTHESIS_MOD_HEAL_TICKS = 274906,
    SPELL_DRU_LUNAR_BEAM_DAMAGE_HEAL = 204069,
    SPELL_DRU_URSOL_VORTEX_PULL = 118283,
    SPELL_DRU_URSOL_VORTEX_DEBUFF = 127797,
    SPELL_DRUID_INFUSION                       = 37238,
    SPELL_DRUID_LANGUISH                       = 71023,
    SPELL_DRUID_LIFEBLOOM_ENERGIZE             = 64372,
    SPELL_DRUID_LIFEBLOOM_FINAL_HEAL           = 33778,
    SPELL_DRUID_LIVING_SEED_HEAL               = 48503,
    SPELL_DRUID_LIVING_SEED_PROC               = 48504,
    SPELL_DRUID_MANGLE                         = 33917,
    SPELL_DRUID_MOONFIRE_DAMAGE                = 164812,
    SPELL_DRUID_PROWL                          = 5215,
    SPELL_DRUID_REJUVENATION_T10_PROC          = 70691,
    SPELL_DRU_TIGER_FURY = 5217,
    SPELL_DRUID_RESTORATION_T10_2P_BONUS       = 70658,
    SPELL_DRUID_SAVAGE_ROAR                    = 62071,
    SPELL_DRUID_SKULL_BASH_CHARGE              = 221514,
    SPELL_DRUID_YSERA_GIFT = 145108,
    SPELL_DRUID_YSERA_GIFT_CASTER_HEAL = 145109,
    SPELL_DRUID_YSERA_GIFT_RAID_HEAL = 145110,
    SPELL_DRUID_HEALING_TOUCH = 5185,
    SPELL_DRUID_SWIFTMEND = 18562,
    SPELL_DRUID_SKULL_BASH_INTERRUPT           = 93985,
    SPELL_DRUID_STAMPEDE_BAER_RANK_1           = 81016,
    SPELL_DRUID_STAMPEDE_CAT_RANK_1            = 81021,
    SPELL_DRUID_STAMPEDE_CAT_STATE             = 109881,
    SPELL_DRUID_SUNFIRE_DAMAGE                 = 164815,
    SPELL_DRUID_RAKE = 1822,
    SPELL_DRUID_RIP = 1079,
    SPELL_FERAL_FRENZY_MAIN = 274837,
    SPELL_FERAL_FRENZY_BLEED = 274838,
    SPELL_DRUID_FEROCIOUS_BITE = 22568,
    SPELL_DRUID_SURVIVAL_INSTINCTS             = 50322,
    SPELL_DRUID_TRAVEL_FORM                    = 783,
    SPELL_DRUID_THRASH_BEAR                    = 77758,
    SPELL_DRUID_THRASH_BEAR_AURA               = 192090,
    SPELL_DRUID_THRASH_CAT                     = 106830,
    SPELL_DRUID_FELINE_SWIFTNESS = 131768,
};

enum EfflorescenceSpells
{
    SPELL_DRUID_EFFLORESCENCE_HEAL = 81269,
    SPELL_DRUID_EFFLORESCENCE_DUMMY = 81262
};

enum ShapeshiftFormSpells
{
    SPELL_DRUID_BEAR_FORM = 5487,
    SPELL_DRUID_CAT_FORM = 768,
    SPELL_DRUID_MOONKIN_FORM = 24858,
    SPELL_DRUID_INCARNATION_TREE_OF_LIFE = 33891,
    SPELL_DRUID_INCARNATION_KING_OF_JUNGLE = 102543
};
enum BalanceAffinitySpells
{
    SPELL_DRUID_STARSURGE = 78674,
    SPELL_DRUID_SUNFIRE = 93402,
    SPELL_DRUID_LUNAR_STRIKE = 194153,
    SPELL_DRUID_SOLAR_WRATH = 190984
};

enum FeralAffinitySpells
{
    SPELL_DRUID_FERAL_AFFINITY_BALANCE = 202157,
    SPELL_DRUID_FERAL_AFFINITY_RESTO = 197490,
    SPELL_DRUID_FERAL_AFFINITY_TANK = 202155
};

enum SoulOfTheForestSpells
{
    SPELL_DRUID_SOUL_OF_THE_FOREST_RESTO_TALENT = 158478,
    SPELL_DRUID_SOUL_OF_THE_FOREST_RESTO = 114108
};

class RaidCheck
{
public:
    explicit RaidCheck(Unit const* caster) : _caster(caster) { }

    bool operator()(WorldObject* obj) const
    {
        if (Unit* target = obj->ToUnit())
            return !_caster->IsInRaidWith(target);

        return true;
    }

private:
    Unit const* _caster;
};



class spell_dru_base_transformer : public SpellScript
{
    PrepareSpellScript(spell_dru_base_transformer);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ GetShapeshiftFormSpell() });
    }

    void HandleOnCast()
    {
        // Change into cat form
        if (GetCaster()->GetShapeshiftForm() != GetShapeshiftForm())
            GetCaster()->CastSpell(GetCaster(), GetShapeshiftFormSpell(), true);
    }

    void Register() override
    {
        BeforeCast += SpellCastFn(spell_dru_base_transformer::HandleOnCast);
    }

protected:
    virtual bool ToCatForm() const = 0;

    ShapeshiftForm GetShapeshiftForm() const { return ToCatForm() ? FORM_CAT_FORM : FORM_BEAR_FORM; }
    uint32 GetShapeshiftFormSpell() const { return ToCatForm() ? SPELL_DRUID_CAT_FORM : SPELL_DRUID_BEAR_FORM; }
};

// Solar empowerment - 164545
class aura_dru_solar_empowerment : public AuraScript
{
    PrepareAuraScript(aura_dru_solar_empowerment);

    void OnApply(const AuraEffect* /* aurEff */, AuraEffectHandleModes /*mode*/)
    {
        if (GetTarget()->HasAura(SPELL_DRUID_STARLORD_DUMMY))
            GetTarget()->CastSpell(nullptr, SPELL_DRUID_STARLORD_SOLAR, true);
    }

    void OnRemove(const AuraEffect* /* aurEff */, AuraEffectHandleModes /*mode*/)
    {
        GetTarget()->RemoveAurasDueToSpell(SPELL_DRUID_STARLORD_SOLAR);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(aura_dru_solar_empowerment::OnApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(aura_dru_solar_empowerment::OnRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

//78674
class spell_dru_starsurge : public SpellScript
{
    PrepareSpellScript(spell_dru_starsurge);

    void HandleOnHit()
    {
        if (GetCaster())
            if (GetCaster()->GetAuraCount(SPELL_DRU_STARLORD_BUFF) < 3)
                GetCaster()->CastSpell(nullptr, SPELL_DRU_STARLORD_BUFF, true);
    }

    void Register() override
    {
        OnHit += SpellHitFn(spell_dru_starsurge::HandleOnHit);
    }
};


// Thrash (Cat Form) - 106830
class spell_dru_thrash_cat : public SpellScript
{
    PrepareSpellScript(spell_dru_thrash_cat);

    void HandleOnEffectHitTarget(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();
        if (!caster || !target)
            return;

        // This prevent awarding multiple Combo Points when multiple targets hit with Thrash AoE
        if (m_awardComboPoint)
            // Awards the caster 1 Combo Point
            caster->ModifyPower(POWER_COMBO_POINTS, 1);

        m_awardComboPoint = false;
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_dru_thrash_cat::HandleOnEffectHitTarget, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }

private:
    bool m_awardComboPoint = true;
};
// Lunar empowerment - 164547
class aura_dru_lunar_empowerment : public AuraScript
{
    PrepareAuraScript(aura_dru_lunar_empowerment);

    void OnApply(const AuraEffect* /* aurEff */, AuraEffectHandleModes /*mode*/)
    {
        if (GetTarget()->HasAura(SPELL_DRUID_STARLORD_DUMMY))
            GetTarget()->CastSpell(nullptr, SPELL_DRUID_STARLORD_LUNAR, true);
    }

    void OnRemove(const AuraEffect* /* aurEff */, AuraEffectHandleModes /*mode*/)
    {
        GetTarget()->RemoveAurasDueToSpell(SPELL_DRUID_STARLORD_LUNAR);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(aura_dru_lunar_empowerment::OnApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(aura_dru_lunar_empowerment::OnRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

// 22812 - Barkskin
class spell_dru_barkskin : public AuraScript
{
    PrepareAuraScript(spell_dru_barkskin);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_BRAMBLES_PASSIVE });
    }

    void HandlePeriodic(AuraEffect const* /*aurEff*/)
    {
        Unit* target = GetTarget();
        if (target->HasAura(SPELL_DRUID_BRAMBLES_PASSIVE))
            target->CastSpell(target, SPELL_DRUID_BRAMBLES_DAMAGE_AURA, true);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_barkskin::HandlePeriodic, EFFECT_2, SPELL_AURA_PERIODIC_DUMMY);
    }
};

// 77758 - Berserk
class spell_dru_berserk : public spell_dru_base_transformer
{
protected:
    bool ToCatForm() const override { return false; }
};

// 203953 - Brambles - SPELL_DRUID_BRAMBLES_PASSIVE
class spell_dru_brambles : public AuraScript
{
    PrepareAuraScript(spell_dru_brambles);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_BRAMBLES_REFLECT, SPELL_DRUID_BRAMBLES_DAMAGE_AURA });
    }

    void HandleAbsorb(AuraEffect* /*aurEff*/, DamageInfo& /*dmgInfo*/, uint32& /*absorbAmount*/)
    {
        // Prevent Removal
        PreventDefaultAction();
    }

    void HandleAfterAbsorb(AuraEffect* /*aurEff*/, DamageInfo& dmgInfo, uint32& absorbAmount)
    {
        // reflect back damage to the attacker
        Unit* target = GetTarget();
        if (Unit* attacker = dmgInfo.GetAttacker())
            target->CastSpell(attacker, SPELL_DRUID_BRAMBLES_REFLECT, CastSpellExtraArgs(TRIGGERED_FULL_MASK).AddSpellMod(SPELLVALUE_BASE_POINT0, absorbAmount));
    }

    void Register() override
    {
        OnEffectAbsorb += AuraEffectAbsorbFn(spell_dru_brambles::HandleAbsorb, EFFECT_0);
        AfterEffectAbsorb += AuraEffectAbsorbFn(spell_dru_brambles::HandleAfterAbsorb, EFFECT_0);
    }
};

// 155835 - Bristling Fur
class spell_dru_bristling_fur : public AuraScript
{
    PrepareAuraScript(spell_dru_bristling_fur);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_BRISTLING_FUR_GAIN_RAGE });
    }

    void HandleProc(AuraEffect* /*aurEff*/, ProcEventInfo& eventInfo)
    {
        // BristlingFurRage = 100 * Damage / MaxHealth.
        if (DamageInfo* damageInfo = eventInfo.GetDamageInfo())
        {
            Unit* target = GetTarget();
            uint32 rage = target->GetMaxPower(POWER_RAGE) * (float)damageInfo->GetDamage() / (float)target->GetMaxHealth();
            if (rage > 0)
                target->CastSpell(target, SPELL_DRUID_BRISTLING_FUR_GAIN_RAGE, CastSpellExtraArgs(TRIGGERED_FULL_MASK).AddSpellMod(SPELLVALUE_BASE_POINT0, rage));
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_dru_bristling_fur::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 768 - CatForm - SPELL_DRUID_CAT_FORM
class spell_dru_cat_form : public AuraScript
{
    PrepareAuraScript(spell_dru_cat_form);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_PROWL });
    }

    void HandleAfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        GetTarget()->RemoveOwnedAura(SPELL_DRUID_PROWL);
    }

    void Register() override
    {
        AfterEffectRemove += AuraEffectRemoveFn(spell_dru_cat_form::HandleAfterRemove, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
    }
};

// 1850 - Dash
class spell_dru_dash : public SpellScriptLoader
{
public:
    spell_dru_dash() : SpellScriptLoader("spell_dru_dash") { }

    class spell_dru_dash_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_dash_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            // do not set speed if not in cat form
            if (GetUnitOwner()->GetShapeshiftForm() != FORM_CAT_FORM)
                amount = 0;
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_dash_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_INCREASE_SPEED);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_dash_AuraScript();
    }
};

class spell_dru_eclipse_common
{
public:
    static void SetSpellCount(Unit* unitOwner, uint32 spellId, uint32 amount)
    {
        Aura* aura = unitOwner->GetAura(spellId);
        if (!aura)
            unitOwner->CastSpell(unitOwner, spellId, CastSpellExtraArgs(TRIGGERED_FULL_MASK).AddSpellMod(SPELLVALUE_AURA_STACK, amount));
        else
            aura->SetStackAmount(amount);
    }
};

// 48517 Eclipse (Solar) + 48518 Eclipse (Lunar)
class spell_dru_eclipse_aura : public AuraScript
{
    PrepareAuraScript(spell_dru_eclipse_aura);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_ECLIPSE_LUNAR_SPELL_CNT, SPELL_DRUID_ECLIPSE_SOLAR_SPELL_CNT, SPELL_DRUID_ECLIPSE_DUMMY });
    }

    void HandleRemoved(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        AuraEffect const* auraEffDummy = GetTarget()->GetAuraEffect(SPELL_DRUID_ECLIPSE_DUMMY, EFFECT_0);
        if (!auraEffDummy)
            return;

        uint32 spellId = GetSpellInfo()->Id == SPELL_DRUID_ECLIPSE_SOLAR_AURA ? SPELL_DRUID_ECLIPSE_LUNAR_SPELL_CNT : SPELL_DRUID_ECLIPSE_SOLAR_SPELL_CNT;
        spell_dru_eclipse_common::SetSpellCount(GetTarget(), spellId, auraEffDummy->GetAmount());
    }

    void Register() override
    {
        AfterEffectRemove += AuraEffectRemoveFn(spell_dru_eclipse_aura::HandleRemoved, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

// 79577 - Eclipse - SPELL_DRUID_ECLIPSE_DUMMY
class spell_dru_eclipse_dummy : public AuraScript
{
    PrepareAuraScript(spell_dru_eclipse_dummy);

    class InitializeEclipseCountersEvent : public BasicEvent
    {
    public:
        InitializeEclipseCountersEvent(Unit* owner, uint32 count) : BasicEvent(), _owner(owner), _count(count) { }

        bool Execute(uint64, uint32) override
        {
            spell_dru_eclipse_common::SetSpellCount(_owner, SPELL_DRUID_ECLIPSE_SOLAR_SPELL_CNT, _count);
            spell_dru_eclipse_common::SetSpellCount(_owner, SPELL_DRUID_ECLIPSE_LUNAR_SPELL_CNT, _count);
            return true;
        }

    private:
        Unit* _owner;
        uint32 _count;
    };

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_ECLIPSE_SOLAR_SPELL_CNT, SPELL_DRUID_ECLIPSE_LUNAR_SPELL_CNT,
            SPELL_DRUID_ECLIPSE_SOLAR_AURA, SPELL_DRUID_ECLIPSE_LUNAR_AURA });
    }

    void HandleProc(ProcEventInfo& eventInfo)
    {
        if (SpellInfo const* spellInfo = eventInfo.GetSpellInfo())
        {
            if (spellInfo->SpellFamilyFlags & flag128(0x4, 0x0, 0x0, 0x0)) // Starfire
                OnSpellCast(SPELL_DRUID_ECLIPSE_SOLAR_SPELL_CNT, SPELL_DRUID_ECLIPSE_LUNAR_SPELL_CNT, SPELL_DRUID_ECLIPSE_SOLAR_AURA);
            else if (spellInfo->SpellFamilyFlags & flag128(0x1, 0x0, 0x0, 0x0)) // Wrath
                OnSpellCast(SPELL_DRUID_ECLIPSE_LUNAR_SPELL_CNT, SPELL_DRUID_ECLIPSE_SOLAR_SPELL_CNT, SPELL_DRUID_ECLIPSE_LUNAR_AURA);
        }
    }

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
    {
        // counters are applied with a delay
        GetTarget()->m_Events.AddEventAtOffset(new InitializeEclipseCountersEvent(GetTarget(), aurEff->GetAmount()), 1s);
    }

    void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        GetTarget()->RemoveAura(SPELL_DRUID_ECLIPSE_SOLAR_SPELL_CNT);
        GetTarget()->RemoveAura(SPELL_DRUID_ECLIPSE_LUNAR_SPELL_CNT);
    }

    void OnOwnerOutOfCombat(bool isNowInCombat)
    {
        if (!isNowInCombat)
            GetTarget()->CastSpell(GetTarget(), SPELL_DRUID_ECLIPSE_OOC, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(spell_dru_eclipse_dummy::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectApplyFn(spell_dru_eclipse_dummy::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnProc += AuraProcFn(spell_dru_eclipse_dummy::HandleProc);
        OnEnterLeaveCombat += AuraEnterLeaveCombatFn(spell_dru_eclipse_dummy::OnOwnerOutOfCombat);
    }

private:
    void OnSpellCast(uint32 cntSpellId, uint32 otherCntSpellId, uint32 eclipseAuraSpellId)
    {
        Unit* target = GetTarget();
        if (Aura* aura = target->GetAura(cntSpellId))
        {
            uint32 remaining = aura->GetStackAmount();
            if (remaining == 0)
                return;

            if (remaining > 1)
                aura->SetStackAmount(remaining - 1);
            else
            {
                // cast eclipse
                target->CastSpell(target, eclipseAuraSpellId, TRIGGERED_FULL_MASK);

                // Remove stacks from other one as well
                // reset remaining power on other spellId
                target->RemoveAura(cntSpellId);
                target->RemoveAura(otherCntSpellId);
            }
        }
    }
};

// 329910 - Eclipse out of combat - SPELL_DRUID_ECLIPSE_OOC
class spell_dru_eclipse_ooc : public AuraScript
{
    PrepareAuraScript(spell_dru_eclipse_ooc);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_ECLIPSE_DUMMY, SPELL_DRUID_ECLIPSE_SOLAR_SPELL_CNT, SPELL_DRUID_ECLIPSE_LUNAR_SPELL_CNT });
    }

    void Tick(AuraEffect const* /*aurEff*/)
    {
        Unit* owner = GetTarget();
        AuraEffect const* auraEffDummy = owner->GetAuraEffect(SPELL_DRUID_ECLIPSE_DUMMY, EFFECT_0);
        if (!auraEffDummy)
            return;

        if (!owner->IsInCombat() && (!owner->HasAura(SPELL_DRUID_ECLIPSE_SOLAR_SPELL_CNT) || !owner->HasAura(SPELL_DRUID_ECLIPSE_LUNAR_SPELL_CNT)))
        {
            // Restore 2 stacks to each spell when out of combat
            spell_dru_eclipse_common::SetSpellCount(owner, SPELL_DRUID_ECLIPSE_SOLAR_SPELL_CNT, auraEffDummy->GetAmount());
            spell_dru_eclipse_common::SetSpellCount(owner, SPELL_DRUID_ECLIPSE_LUNAR_SPELL_CNT, auraEffDummy->GetAmount());
        }
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_eclipse_ooc::Tick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class dru_predator : public PlayerScript
{
public:
    dru_predator() : PlayerScript("dru_predator") { }

    void OnPVPKill(Player* killer, Player* killed)
    {
        if (killer->getClass() == CLASS_DRUID)
            return;

        if (!killer->HasAura(SPELL_DRU_PREDATOR))
            return;

        if (killer->GetSpellHistory()->HasCooldown(SPELL_DRU_TIGER_FURY))
            killer->GetSpellHistory()->ResetCooldown(SPELL_DRU_TIGER_FURY);
    }

    void OnCreatureKill(Player* killer, Creature* killed)
    {
        if (killer->getClass() == CLASS_DRUID)
            return;

        if (!killer->HasAura(SPELL_DRU_PREDATOR))
            return;

        if (killer->GetSpellHistory()->HasCooldown(SPELL_DRU_TIGER_FURY))
            killer->GetSpellHistory()->ResetCooldown(SPELL_DRU_TIGER_FURY);
    }
};

// 203974 - Earthwarden
class spell_dru_earthwarden : public AuraScript
{
    PrepareAuraScript(spell_dru_earthwarden);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_THRASH_CAT, SPELL_DRUID_THRASH_BEAR, SPELL_DRUID_EARTHWARDEN_AURA });
    }

    void HandleProc(AuraEffect* /*aurEff*/, ProcEventInfo& /*eventInfo*/)
    {
        Unit* target = GetTarget();
        target->CastSpell(target, SPELL_DRUID_EARTHWARDEN_AURA, true);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_dru_earthwarden::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

enum PredatorySwiftnessSpells
{
    SPELL_DRUID_PREDATORY_SWIFTNESS = 16974,
    SPELL_DRUID_PREDATORY_SWIFTNESS_AURA = 69369
};

// Predatory Swiftness - 16974
// @Called : Maim - 22570, Ferocious Bite - 22568, Rip - 1079, Savage Roar - 1079
// @Version : 7.1.0.22908
class spell_dru_predatory_swiftness : public SpellScriptLoader
{
public:
    spell_dru_predatory_swiftness() : SpellScriptLoader("spell_dru_predatory_swiftness") { }

    class spell_dru_predatory_swiftness_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_predatory_swiftness_SpellScript);

        uint8 _cp;

        bool Load() override
        {
            _cp = GetCaster()->GetPower(POWER_COMBO_POINTS);
            return true;
        }

        SpellCastResult CheckCast()
        {
            if (GetCaster())
            {
                if (GetCaster()->GetTypeId() != TYPEID_PLAYER)
                    return SPELL_FAILED_DONT_REPORT;

                if (!GetCaster()->ToPlayer()->GetComboPoints())
                    return SPELL_FAILED_NO_COMBO_POINTS;
            }
            else
                return SPELL_FAILED_DONT_REPORT;

            return SPELL_CAST_OK;
        }

        void HandleOnHit()
        {
            if (Player* player = GetCaster()->ToPlayer())
                if (player->HasAura(SPELL_DRUID_PREDATORY_SWIFTNESS) && roll_chance_i(20 * _cp))
                    player->CastSpell(player, SPELL_DRUID_PREDATORY_SWIFTNESS_AURA, true);
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_dru_predatory_swiftness_SpellScript::CheckCast);
            AfterHit += SpellHitFn(spell_dru_predatory_swiftness_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_predatory_swiftness_SpellScript();
    }
};

// Predatory Swiftness (Aura) - 69369
// @Called : Entangling Roots - 339, Rebirth - 20484, Regrowth - 8936
// @Version : 7.1.0.22908
class spell_dru_predatory_swiftness_aura : public SpellScriptLoader
{
public:
    spell_dru_predatory_swiftness_aura() : SpellScriptLoader("spell_dru_predatory_swiftness_aura") { }

    class spell_dru_predatory_swiftness_aura_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_predatory_swiftness_aura_SpellScript);

        void HandleAfterHit()
        {
            if (Player* player = GetCaster()->ToPlayer())
            {
                if (player->HasAura(SPELL_DRUID_PREDATORY_SWIFTNESS_AURA))
                    player->RemoveAurasDueToSpell(SPELL_DRUID_PREDATORY_SWIFTNESS_AURA);
            }
        }

        void Register() override
        {
            AfterHit += SpellHitFn(spell_dru_predatory_swiftness_aura_SpellScript::HandleAfterHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_predatory_swiftness_aura_SpellScript();
    }
};

// 22568 - Ferocious Bite
class spell_dru_ferocious_bite : public SpellScript
{
    PrepareSpellScript(spell_dru_ferocious_bite);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_INCARNATION_KING_OF_THE_JUNGLE  })
            && sSpellMgr->AssertSpellInfo(SPELL_DRUID_INCARNATION_KING_OF_THE_JUNGLE, DIFFICULTY_NONE)->GetEffect(EFFECT_1);
    }

    void HandleHitTargetBurn(SpellEffIndex /*effIndex*/)
    {
        int32 newValue = int32(float(GetEffectValue()) * _damageMultiplier);
        SetEffectValue(newValue);
    }

    void HandleHitTargetDmg(SpellEffIndex /*effIndex*/)
    {
        int32 newValue = int32(float(GetHitDamage()) * (1.0f + _damageMultiplier));
        SetHitDamage(newValue);
    }

    void HandleLaunchTarget(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();

        int32 maxExtraConsumedPower = GetEffectValue();

        if (AuraEffect* auraEffect = caster->GetAuraEffect(SPELL_DRUID_INCARNATION_KING_OF_THE_JUNGLE, EFFECT_1))
        {
            float multiplier = 1.0f + float(auraEffect->GetAmount()) / 100.0f;
            maxExtraConsumedPower = int32(float(maxExtraConsumedPower) * multiplier);
            SetEffectValue(maxExtraConsumedPower);
        }

        _damageMultiplier = std::min<float>(caster->GetPower(POWER_ENERGY), maxExtraConsumedPower) / maxExtraConsumedPower;
    }

    void Register() override
    {
        OnEffectLaunchTarget += SpellEffectFn(spell_dru_ferocious_bite::HandleLaunchTarget, EFFECT_1, SPELL_EFFECT_POWER_BURN);
        OnEffectHitTarget += SpellEffectFn(spell_dru_ferocious_bite::HandleHitTargetBurn, EFFECT_1, SPELL_EFFECT_POWER_BURN);
        OnEffectHitTarget += SpellEffectFn(spell_dru_ferocious_bite::HandleHitTargetDmg, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }

private:
    float _damageMultiplier = 0.0f;
};

// -33943 - Flight Form
class spell_dru_flight_form : public SpellScriptLoader
{
public:
    spell_dru_flight_form() : SpellScriptLoader("spell_dru_flight_form") { }

    class spell_dru_flight_form_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_flight_form_SpellScript);

        SpellCastResult CheckCast()
        {
            Unit* caster = GetCaster();
            if (caster->IsInDisallowedMountForm())
                return SPELL_FAILED_NOT_SHAPESHIFT;

            return SPELL_CAST_OK;
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_dru_flight_form_SpellScript::CheckCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_flight_form_SpellScript();
    }
};

// 37336 - Druid Forms Trinket
class spell_dru_forms_trinket : public SpellScriptLoader
{
public:
    spell_dru_forms_trinket() : SpellScriptLoader("spell_dru_forms_trinket") { }

    class spell_dru_forms_trinket_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_forms_trinket_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo(
            {
                SPELL_DRUID_FORMS_TRINKET_BEAR,
                SPELL_DRUID_FORMS_TRINKET_CAT,
                SPELL_DRUID_FORMS_TRINKET_MOONKIN,
                SPELL_DRUID_FORMS_TRINKET_NONE,
                SPELL_DRUID_FORMS_TRINKET_TREE
            });
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            Unit* target = eventInfo.GetActor();

            switch (target->GetShapeshiftForm())
            {
                case FORM_BEAR_FORM:
                case FORM_DIRE_BEAR_FORM:
                case FORM_CAT_FORM:
                case FORM_MOONKIN_FORM:
                case FORM_NONE:
                case FORM_TREE_OF_LIFE:
                    return true;
                default:
                    break;
            }

            return false;
        }

        void HandleProc(AuraEffect* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            Unit* target = eventInfo.GetActor();
            uint32 triggerspell = 0;

            switch (target->GetShapeshiftForm())
            {
                case FORM_BEAR_FORM:
                case FORM_DIRE_BEAR_FORM:
                    triggerspell = SPELL_DRUID_FORMS_TRINKET_BEAR;
                    break;
                case FORM_CAT_FORM:
                    triggerspell = SPELL_DRUID_FORMS_TRINKET_CAT;
                    break;
                case FORM_MOONKIN_FORM:
                    triggerspell = SPELL_DRUID_FORMS_TRINKET_MOONKIN;
                    break;
                case FORM_NONE:
                    triggerspell = SPELL_DRUID_FORMS_TRINKET_NONE;
                    break;
                case FORM_TREE_OF_LIFE:
                    triggerspell = SPELL_DRUID_FORMS_TRINKET_TREE;
                    break;
                default:
                    return;
            }

            target->CastSpell(target, triggerspell, aurEff);
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_dru_forms_trinket_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_dru_forms_trinket_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_forms_trinket_AuraScript();
    }
};

// 203964 - Galactic Guardian
class spell_dru_galactic_guardian : public AuraScript
{
    PrepareAuraScript(spell_dru_galactic_guardian);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_GALACTIC_GUARDIAN_AURA });
    }

    void HandleProc(AuraEffect* /*aurEff*/, ProcEventInfo& eventInfo)
    {
        if (DamageInfo* damageInfo = eventInfo.GetDamageInfo())
        {
            Unit* target = GetTarget();

            // free automatic moonfire on target
            target->CastSpell(damageInfo->GetVictim(), SPELL_DRUID_MOONFIRE_DAMAGE, true);

            // Cast aura
            target->CastSpell(damageInfo->GetVictim(), SPELL_DRUID_GALACTIC_GUARDIAN_AURA, true);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_dru_galactic_guardian::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 210706 - Gore
class spell_dru_gore : public AuraScript
{
    PrepareAuraScript(spell_dru_gore);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_GORE_PROC, SPELL_DRUID_MANGLE });
    }

    bool CheckEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        return roll_chance_i(aurEff->GetAmount());
    }

    void HandleProc(AuraEffect* /*aurEff*/, ProcEventInfo& /*procInfo*/)
    {
        Unit* owner = GetTarget();
        owner->CastSpell(owner, SPELL_DRUID_GORE_PROC);
        owner->GetSpellHistory()->ResetCooldown(SPELL_DRUID_MANGLE, true);
    }

    void Register() override
    {
        DoCheckEffectProc += AuraCheckEffectProcFn(spell_dru_gore::CheckEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
        OnEffectProc += AuraEffectProcFn(spell_dru_gore::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 34246 - Idol of the Emerald Queen
// 60779 - Idol of Lush Moss
class spell_dru_idol_lifebloom : public SpellScriptLoader
{
public:
    spell_dru_idol_lifebloom() : SpellScriptLoader("spell_dru_idol_lifebloom") { }

    class spell_dru_idol_lifebloom_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_idol_lifebloom_AuraScript);

        void HandleEffectCalcSpellMod(AuraEffect const* aurEff, SpellModifier*& spellMod)
        {
            if (!spellMod)
            {
                spellMod = new SpellModifier(GetAura());
                spellMod->op = SpellModOp::PeriodicHealingAndDamage;
                spellMod->type = SPELLMOD_FLAT;
                spellMod->spellId = GetId();
                spellMod->mask = aurEff->GetSpellEffectInfo()->SpellClassMask;
            }
            spellMod->value = aurEff->GetAmount() / 7;
        }

        void Register() override
        {
            DoEffectCalcSpellMod += AuraEffectCalcSpellModFn(spell_dru_idol_lifebloom_AuraScript::HandleEffectCalcSpellMod, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_idol_lifebloom_AuraScript();
    }
};

// 99 - Incapacitating Roar
class spell_dru_incapacitating_roar : public spell_dru_base_transformer
{
protected:
    bool ToCatForm() const override { return false; }
};

// 29166 - Innervate
class spell_dru_innervate : public SpellScriptLoader
{
public:
    spell_dru_innervate() : SpellScriptLoader("spell_dru_innervate") { }

    class spell_dru_innervate_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_innervate_AuraScript);

        void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (!aurEff->GetTotalTicks())
            {
                amount = 0;
                return;
            }

            if (Unit* caster = GetCaster())
                amount = int32(CalculatePct(caster->GetCreatePowers(POWER_MANA), amount) / aurEff->GetTotalTicks());
            else
                amount = 0;
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_innervate_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_ENERGIZE);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_innervate_AuraScript();
    }
};

// 40442 - Druid Tier 6 Trinket
class spell_dru_item_t6_trinket : public SpellScriptLoader
{
public:
    spell_dru_item_t6_trinket() : SpellScriptLoader("spell_dru_item_t6_trinket") { }

    class spell_dru_item_t6_trinket_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_item_t6_trinket_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({
                SPELL_DRUID_BLESSING_OF_REMULOS,
                SPELL_DRUID_BLESSING_OF_ELUNE,
                SPELL_DRUID_BLESSING_OF_CENARIUS
                });
        }

        void HandleProc(AuraEffect* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            if (!spellInfo)
                return;

            uint32 spellId;
            int32 chance;

            // Starfire
            if (spellInfo->SpellFamilyFlags[0] & 0x00000004)
            {
                spellId = SPELL_DRUID_BLESSING_OF_REMULOS;
                chance = 25;
            }
            // Rejuvenation
            else if (spellInfo->SpellFamilyFlags[0] & 0x00000010)
            {
                spellId = SPELL_DRUID_BLESSING_OF_ELUNE;
                chance = 25;
            }
            // Mangle (Bear) and Mangle (Cat)
            else if (spellInfo->SpellFamilyFlags[1] & 0x00000440)
            {
                spellId = SPELL_DRUID_BLESSING_OF_CENARIUS;
                chance = 40;
            }
            else
                return;

            if (roll_chance_i(chance))
                eventInfo.GetActor()->CastSpell(nullptr, spellId, aurEff);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_dru_item_t6_trinket_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_item_t6_trinket_AuraScript();
    }
};

// 33763 - Lifebloom
class spell_dru_lifebloom : public SpellScriptLoader
{
public:
    spell_dru_lifebloom() : SpellScriptLoader("spell_dru_lifebloom") { }

    class spell_dru_lifebloom_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_lifebloom_AuraScript);

        bool Validate(SpellInfo const* /*spell*/) override
        {
            return ValidateSpellInfo({ SPELL_DRUID_LIFEBLOOM_FINAL_HEAL, SPELL_DRUID_LIFEBLOOM_ENERGIZE });
        }

        void OnRemoveEffect(Unit* target, AuraEffect const* aurEff, uint32 stack)
        {
            int32 healAmount = aurEff->GetAmount();
            if (Unit* caster = GetCaster())
            {
                healAmount = caster->SpellHealingBonusDone(GetTarget(), GetSpellInfo(), healAmount, HEAL, aurEff->GetSpellEffectInfo(), stack);
                healAmount = GetTarget()->SpellHealingBonusTaken(caster, GetSpellInfo(), healAmount, HEAL, aurEff->GetSpellEffectInfo(), stack);

                // restore mana
                std::vector<SpellPowerCost> costs = GetSpellInfo()->CalcPowerCost(caster, GetSpellInfo()->GetSchoolMask());
                auto m = std::find_if(costs.begin(), costs.end(), [](SpellPowerCost const& cost) { return cost.Power == POWER_MANA; });
                if (m != costs.end())
                {
                    CastSpellExtraArgs args(aurEff);
                    args.OriginalCaster = GetCasterGUID();
                    args.AddSpellBP0(m->Amount * stack / 2);
                    caster->CastSpell(caster, SPELL_DRUID_LIFEBLOOM_ENERGIZE, args);
                }
            }

            CastSpellExtraArgs args(aurEff);
            args.OriginalCaster = GetCasterGUID();
            args.AddSpellBP0(healAmount);
            target->CastSpell(target, SPELL_DRUID_LIFEBLOOM_FINAL_HEAL, args);
        }

        void AfterRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            // Final heal only on duration end
            if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE)
                return;

            // final heal
            OnRemoveEffect(GetTarget(), aurEff, GetStackAmount());
        }

        void HandleDispel(DispelInfo* dispelInfo)
        {
            if (Unit* target = GetUnitOwner())
                if (AuraEffect const* aurEff = GetEffect(EFFECT_1))
                    OnRemoveEffect(target, aurEff, dispelInfo->GetRemovedCharges()); // final heal
        }

        void Register() override
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_dru_lifebloom_AuraScript::AfterRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            AfterDispel += AuraDispelFn(spell_dru_lifebloom_AuraScript::HandleDispel);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_lifebloom_AuraScript();
    }
};

// -48496 - Living Seed
class spell_dru_living_seed : public SpellScriptLoader
{
public:
    spell_dru_living_seed() : SpellScriptLoader("spell_dru_living_seed") { }

    class spell_dru_living_seed_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_living_seed_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ SPELL_DRUID_LIVING_SEED_PROC });
        }

        void HandleProc(AuraEffect* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            HealInfo* healInfo = eventInfo.GetHealInfo();
            if (!healInfo || !healInfo->GetHeal())
                return;

            CastSpellExtraArgs args(aurEff);
            args.SpellValueOverrides.AddBP0(CalculatePct(healInfo->GetHeal(), aurEff->GetAmount()));
            GetTarget()->CastSpell(eventInfo.GetProcTarget(), SPELL_DRUID_LIVING_SEED_PROC, args);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_dru_living_seed_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_living_seed_AuraScript();
    }
};

// 48504 - Living Seed (Proc)
class spell_dru_living_seed_proc : public SpellScriptLoader
{
public:
    spell_dru_living_seed_proc() : SpellScriptLoader("spell_dru_living_seed_proc") { }

    class spell_dru_living_seed_proc_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_living_seed_proc_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ SPELL_DRUID_LIVING_SEED_HEAL });
        }

        void HandleProc(AuraEffect* aurEff, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();
            CastSpellExtraArgs args(aurEff);
            args.SpellValueOverrides.AddBP0(aurEff->GetAmount());
            GetTarget()->CastSpell(GetTarget(), SPELL_DRUID_LIVING_SEED_HEAL, args);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_dru_living_seed_proc_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_living_seed_proc_AuraScript();
    }
};

//  8921 - Moonfire
class spell_dru_moonfire : public SpellScriptLoader
{
public:
    spell_dru_moonfire() : SpellScriptLoader("spell_dru_moonfire") { }

    class spell_dru_moonfire_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_moonfire_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ SPELL_DRUID_MOONFIRE_DAMAGE });
        }

        void HandleOnHit(SpellEffIndex /*effIndex*/)
        {
            GetCaster()->CastSpell(GetHitUnit(), SPELL_DRUID_MOONFIRE_DAMAGE, true);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_dru_moonfire_SpellScript::HandleOnHit, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_moonfire_SpellScript();
    }
};

// 16864 - Omen of Clarity
class spell_dru_omen_of_clarity : public SpellScriptLoader
{
public:
    spell_dru_omen_of_clarity() : SpellScriptLoader("spell_dru_omen_of_clarity") { }

    class spell_dru_omen_of_clarity_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_omen_of_clarity_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ SPELL_DRUID_BALANCE_T10_BONUS, SPELL_DRUID_BALANCE_T10_BONUS_PROC });
        }

        void HandleProc(AuraEffect* /*aurEff*/, ProcEventInfo& /*eventInfo*/)
        {
            Unit* target = GetTarget();
            if (target->HasAura(SPELL_DRUID_BALANCE_T10_BONUS))
                target->CastSpell(nullptr, SPELL_DRUID_BALANCE_T10_BONUS_PROC, true);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_dru_omen_of_clarity_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_omen_of_clarity_AuraScript();
    }
};

// -16972 - Predatory Strikes
class spell_dru_predatory_strikes : public SpellScriptLoader
{
public:
    spell_dru_predatory_strikes() : SpellScriptLoader("spell_dru_predatory_strikes") { }

    class spell_dru_predatory_strikes_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_predatory_strikes_AuraScript);

        void UpdateAmount(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Player* target = GetTarget()->ToPlayer())
                target->UpdateAttackPowerAndDamage();
        }

        void Register() override
        {
            AfterEffectApply += AuraEffectApplyFn(spell_dru_predatory_strikes_AuraScript::UpdateAmount, EFFECT_ALL, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
            AfterEffectRemove += AuraEffectRemoveFn(spell_dru_predatory_strikes_AuraScript::UpdateAmount, EFFECT_ALL, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_predatory_strikes_AuraScript();
    }
};

// 5215 - Prowl
class spell_dru_prowl : public spell_dru_base_transformer
{
protected:
    bool ToCatForm() const override { return true; }
};

// 1079 - Rip
class spell_dru_rip : public SpellScriptLoader
{
public:
    spell_dru_rip() : SpellScriptLoader("spell_dru_rip") { }

    class spell_dru_rip_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_rip_AuraScript);

        bool Load() override
        {
            Unit* caster = GetCaster();
            return caster && caster->GetTypeId() == TYPEID_PLAYER;
        }

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
        {
            canBeRecalculated = false;

            if (Unit* caster = GetCaster())
            {
                // 0.01 * $AP * cp
                uint32 cp = caster->ToPlayer()->GetComboPoints();

                // Idol of Feral Shadows. Can't be handled as SpellMod due its dependency from CPs
                if (AuraEffect const* auraEffIdolOfFeralShadows = caster->GetAuraEffect(SPELL_DRUID_IDOL_OF_FERAL_SHADOWS, EFFECT_0))
                    amount += cp * auraEffIdolOfFeralShadows->GetAmount();
                // Idol of Worship. Can't be handled as SpellMod due its dependency from CPs
                else if (AuraEffect const* auraEffIdolOfWorship = caster->GetAuraEffect(SPELL_DRUID_IDOL_OF_WORSHIP, EFFECT_0))
                    amount += cp * auraEffIdolOfWorship->GetAmount();

                amount += int32(CalculatePct(caster->GetTotalAttackPowerValue(BASE_ATTACK), cp));
            }
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_rip_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_rip_AuraScript();
    }
};

// 52610 - Savage Roar
class spell_dru_savage_roar : public SpellScriptLoader
{
public:
    spell_dru_savage_roar() : SpellScriptLoader("spell_dru_savage_roar") { }

    class spell_dru_savage_roar_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_savage_roar_SpellScript);

        SpellCastResult CheckCast()
        {
            Unit* caster = GetCaster();
            if (caster->GetShapeshiftForm() != FORM_CAT_FORM)
                return SPELL_FAILED_ONLY_SHAPESHIFT;

            return SPELL_CAST_OK;
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_dru_savage_roar_SpellScript::CheckCast);
        }
    };

    class spell_dru_savage_roar_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_savage_roar_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ SPELL_DRUID_SAVAGE_ROAR });
        }

        void AfterApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();
            target->CastSpell(target, SPELL_DRUID_SAVAGE_ROAR, { aurEff, GetCasterGUID() });
        }

        void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetTarget()->RemoveAurasDueToSpell(SPELL_DRUID_SAVAGE_ROAR);
        }

        void Register() override
        {
            AfterEffectApply += AuraEffectApplyFn(spell_dru_savage_roar_AuraScript::AfterApply, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_dru_savage_roar_AuraScript::AfterRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_savage_roar_SpellScript();
    }

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_savage_roar_AuraScript();
    }
};

// 106839 - Skull Bash
class spell_dru_skull_bash : public SpellScript
{
    PrepareSpellScript(spell_dru_skull_bash);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_SKULL_BASH_CHARGE, SPELL_DRUID_SKULL_BASH_INTERRUPT });
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        GetCaster()->CastSpell(GetHitUnit(), SPELL_DRUID_SKULL_BASH_CHARGE, true);
        GetCaster()->CastSpell(GetHitUnit(), SPELL_DRUID_SKULL_BASH_INTERRUPT, true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_dru_skull_bash::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// -78892 - Stampede
class spell_dru_stampede : public SpellScriptLoader
{
public:
    spell_dru_stampede() : SpellScriptLoader("spell_dru_stampede") { }

    class spell_dru_stampede_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_stampede_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo(
            {
                SPELL_DRUID_STAMPEDE_BAER_RANK_1,
                SPELL_DRUID_STAMPEDE_CAT_RANK_1,
                SPELL_DRUID_STAMPEDE_CAT_STATE,
                SPELL_DRUID_FERAL_CHARGE_CAT,
                SPELL_DRUID_FERAL_CHARGE_BEAR
            });
        }

        void HandleEffectCatProc(AuraEffect* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            if (GetTarget()->GetShapeshiftForm() != FORM_CAT_FORM || eventInfo.GetDamageInfo()->GetSpellInfo()->Id != SPELL_DRUID_FERAL_CHARGE_CAT)
                return;

            GetTarget()->CastSpell(GetTarget(), sSpellMgr->GetSpellWithRank(SPELL_DRUID_STAMPEDE_CAT_RANK_1, GetSpellInfo()->GetRank()), aurEff);
            GetTarget()->CastSpell(GetTarget(), SPELL_DRUID_STAMPEDE_CAT_STATE, aurEff);
        }

        void HandleEffectBearProc(AuraEffect* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            if (GetTarget()->GetShapeshiftForm() != FORM_BEAR_FORM || eventInfo.GetDamageInfo()->GetSpellInfo()->Id != SPELL_DRUID_FERAL_CHARGE_BEAR)
                return;

            GetTarget()->CastSpell(GetTarget(), sSpellMgr->GetSpellWithRank(SPELL_DRUID_STAMPEDE_BAER_RANK_1, GetSpellInfo()->GetRank()), aurEff);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_dru_stampede_AuraScript::HandleEffectCatProc, EFFECT_0, SPELL_AURA_DUMMY);
            OnEffectProc += AuraEffectProcFn(spell_dru_stampede_AuraScript::HandleEffectBearProc, EFFECT_1, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_stampede_AuraScript();
    }
};

// 106898 - Stampeding Roar
class spell_dru_stampeding_roar : public spell_dru_base_transformer
{
protected:
    bool ToCatForm() const override { return false; }
};

// 50286 - Starfall (Dummy)
class spell_dru_starfall_dummy : public SpellScriptLoader
{
public:
    spell_dru_starfall_dummy() : SpellScriptLoader("spell_dru_starfall_dummy") { }

    class spell_dru_starfall_dummy_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_starfall_dummy_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            Trinity::Containers::RandomResize(targets, 2);
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            // Shapeshifting into an animal form or mounting cancels the effect
            if (caster->GetCreatureType() == CREATURE_TYPE_BEAST || caster->IsMounted())
            {
                if (SpellInfo const* spellInfo = GetTriggeringSpell())
                    caster->RemoveAurasDueToSpell(spellInfo->Id);
                return;
            }

            // Any effect which causes you to lose control of your character will supress the starfall effect.
            if (caster->HasUnitState(UNIT_STATE_CONTROLLED))
                return;

            caster->CastSpell(GetHitUnit(), uint32(GetEffectValue()), true);
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_starfall_dummy_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnEffectHitTarget += SpellEffectFn(spell_dru_starfall_dummy_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_starfall_dummy_SpellScript();
    }
};

//  93402 - Sunfire
class spell_dru_sunfire : public SpellScriptLoader
{
public:
    spell_dru_sunfire() : SpellScriptLoader("spell_dru_sunfire") { }

    class spell_dru_sunfire_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_sunfire_SpellScript);


        void HandleOnHit(SpellEffIndex /*effIndex*/)
        {
            GetCaster()->CastSpell(GetHitUnit(), SPELL_DRUID_SUNFIRE_DAMAGE, true);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_dru_sunfire_SpellScript::HandleOnHit, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_sunfire_SpellScript();
    }
};

// 61336 - Survival Instincts
class spell_dru_survival_instincts : public SpellScriptLoader
{
public:
    spell_dru_survival_instincts() : SpellScriptLoader("spell_dru_survival_instincts") { }

    class spell_dru_survival_instincts_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_survival_instincts_AuraScript);

        bool Validate(SpellInfo const* /*spell*/) override
        {
            return ValidateSpellInfo({ SPELL_DRUID_SURVIVAL_INSTINCTS });
        }

        void AfterApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();
            CastSpellExtraArgs args(aurEff);
            args.AddSpellMod(SPELLVALUE_BASE_POINT0, target->CountPctFromMaxHealth(aurEff->GetAmount()));
            target->CastSpell(target, SPELL_DRUID_SURVIVAL_INSTINCTS, args);
        }

        void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetTarget()->RemoveAurasDueToSpell(SPELL_DRUID_SURVIVAL_INSTINCTS);
        }

        void Register() override
        {
            AfterEffectApply += AuraEffectApplyFn(spell_dru_survival_instincts_AuraScript::AfterApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
            AfterEffectRemove += AuraEffectRemoveFn(spell_dru_survival_instincts_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_survival_instincts_AuraScript();
    }
};

// Swipe - 106785
class spell_dru_swipe : public SpellScript
{
    PrepareSpellScript(spell_dru_swipe);

    void HandleOnHit(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();
        if (!caster || !target)
            return;

        int32 damage = GetHitDamage();
        int32 casterLevel = caster->GetLevelForTarget(caster);

        // This prevent awarding multiple Combo Points when multiple targets hit with Swipe AoE
        if (m_awardComboPoint)
            // Awards the caster 1 Combo Point (get value from the spell data)
            caster->ModifyPower(POWER_COMBO_POINTS, sSpellMgr->GetSpellInfo(SPELL_DRUID_SWIPE_CAT, DIFFICULTY_NONE)->GetEffect(EFFECT_0)->BasePoints);

        // If caster is level >= 44 and the target is bleeding, deals 20% increased damage (get value from the spell data)
        if ((casterLevel >= 44) && target->HasAuraState(AURA_STATE_BLEED))
            AddPct(damage, sSpellMgr->GetSpellInfo(SPELL_DRUID_SWIPE_CAT, DIFFICULTY_NONE)->GetEffect(EFFECT_1)->BasePoints);

        SetHitDamage(damage);

        m_awardComboPoint = false;
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_dru_swipe::HandleOnHit, EFFECT_1, SPELL_EFFECT_DUMMY);
    }

private:
    bool m_awardComboPoint = true;
};

// 159286 Primal Fury
// @Version : 7.1.0.22908
class spell_dru_primal_fury : public SpellScriptLoader
{
public:
    spell_dru_primal_fury() : SpellScriptLoader("spell_dru_primal_fury") { }

    class spell_dru_primal_fury_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_primal_fury_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            bool _spellCanProc = (eventInfo.GetSpellInfo()->Id == SPELL_DRUID_SHRED || eventInfo.GetSpellInfo()->Id == SPELL_DRUID_RAKE || eventInfo.GetSpellInfo()->Id == SPELL_DRUID_SWIPE_CAT || eventInfo.GetSpellInfo()->Id == SPELL_DRUID_MOONFIRE_CAT);

            if ((eventInfo.GetHitMask() & PROC_HIT_CRITICAL) && _spellCanProc)
                return true;

            return false;
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_dru_primal_fury_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_primal_fury_AuraScript();
    }
};


// 40121 - Swift Flight Form (Passive)
class spell_dru_swift_flight_passive : public SpellScriptLoader
{
public:
    spell_dru_swift_flight_passive() : SpellScriptLoader("spell_dru_swift_flight_passive") { }

    class spell_dru_swift_flight_passive_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_swift_flight_passive_AuraScript);

        bool Load() override
        {
            return GetCaster()->GetTypeId() == TYPEID_PLAYER;
        }

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
        {
            if (Player* caster = GetCaster()->ToPlayer())
                if (caster->GetSkillValue(SKILL_RIDING) >= 375)
                    amount = 310;
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_swift_flight_passive_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_MOD_INCREASE_VEHICLE_FLIGHT_SPEED);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_swift_flight_passive_AuraScript();
    }
};

// 28744 - Regrowth
class spell_dru_t3_6p_bonus : public SpellScriptLoader
{
    public:
        spell_dru_t3_6p_bonus() : SpellScriptLoader("spell_dru_t3_6p_bonus") { }

        class spell_dru_t3_6p_bonus_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_t3_6p_bonus_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ SPELL_DRUID_BLESSING_OF_THE_CLAW });
            }

            void HandleProc(AuraEffect* aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();
                eventInfo.GetActor()->CastSpell(eventInfo.GetProcTarget(), SPELL_DRUID_BLESSING_OF_THE_CLAW, aurEff);
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_dru_t3_6p_bonus_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_OVERRIDE_CLASS_SCRIPTS);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_t3_6p_bonus_AuraScript();
        }
};

// 28719 - Healing Touch
class spell_dru_t3_8p_bonus : public SpellScriptLoader
{
        public:
            spell_dru_t3_8p_bonus() : SpellScriptLoader("spell_dru_t3_8p_bonus") { }

        class spell_dru_t3_8p_bonus_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_t3_8p_bonus_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ SPELL_DRUID_EXHILARATE });
            }

            void HandleProc(AuraEffect* aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();
                Spell const* spell = eventInfo.GetProcSpell();
                if (!spell)
                    return;

                Unit* caster = eventInfo.GetActor();
                std::vector<SpellPowerCost> const& costs = spell->GetPowerCost();
                auto m = std::find_if(costs.begin(), costs.end(), [](SpellPowerCost const& cost) { return cost.Power == POWER_MANA; });
                if (m == costs.end())
                    return;

                int32 amount = CalculatePct(m->Amount, aurEff->GetAmount());
                CastSpellExtraArgs args(aurEff);
                args.SpellValueOverrides.AddBP0(amount);
                caster->CastSpell(nullptr, SPELL_DRUID_EXHILARATE, args);
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_dru_t3_8p_bonus_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_t3_8p_bonus_AuraScript();
        }
};

// 37288 - Mana Restore
// 37295 - Mana Restore
class spell_dru_t4_2p_bonus : public SpellScriptLoader
{
    public:
        spell_dru_t4_2p_bonus() : SpellScriptLoader("spell_dru_t4_2p_bonus") { }

        class spell_dru_t4_2p_bonus_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_t4_2p_bonus_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ SPELL_DRUID_INFUSION });
            }

            void HandleProc(AuraEffect* aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();
                eventInfo.GetActor()->CastSpell(nullptr, SPELL_DRUID_INFUSION, aurEff);
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_dru_t4_2p_bonus_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_t4_2p_bonus_AuraScript();
        }
};

// 70723 - Item - Druid T10 Balance 4P Bonus
class spell_dru_t10_balance_4p_bonus : public SpellScriptLoader
{
public:
    spell_dru_t10_balance_4p_bonus() : SpellScriptLoader("spell_dru_t10_balance_4p_bonus") { }

    class spell_dru_t10_balance_4p_bonus_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_t10_balance_4p_bonus_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ SPELL_DRUID_LANGUISH });
        }

        void HandleProc(AuraEffect* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            DamageInfo* damageInfo = eventInfo.GetDamageInfo();
            if (!damageInfo || !damageInfo->GetDamage())
                return;

            Unit* caster = eventInfo.GetActor();
            Unit* target = eventInfo.GetProcTarget();

            SpellInfo const* spellInfo = sSpellMgr->AssertSpellInfo(SPELL_DRUID_LANGUISH, GetCastDifficulty());
            int32 amount = CalculatePct(static_cast<int32>(damageInfo->GetDamage()), aurEff->GetAmount());

            ASSERT(spellInfo->GetMaxTicks() > 0);
            amount /= spellInfo->GetMaxTicks();
            // Add remaining ticks to damage done
            amount += target->GetRemainingPeriodicAmount(caster->GetGUID(), SPELL_DRUID_LANGUISH, SPELL_AURA_PERIODIC_DAMAGE);

            CastSpellExtraArgs args(aurEff);
            args.SpellValueOverrides.AddBP0(amount);
            caster->CastSpell(target, SPELL_DRUID_LANGUISH, args);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_dru_t10_balance_4p_bonus_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_t10_balance_4p_bonus_AuraScript();
    }
};

// 70691 - Item T10 Restoration 4P Bonus
class spell_dru_t10_restoration_4p_bonus : public SpellScriptLoader
{
public:
    spell_dru_t10_restoration_4p_bonus() : SpellScriptLoader("spell_dru_t10_restoration_4p_bonus") { }

    class spell_dru_t10_restoration_4p_bonus_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_t10_restoration_4p_bonus_SpellScript);

        bool Load() override
        {
            return GetCaster()->GetTypeId() == TYPEID_PLAYER;
        }

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (!GetCaster()->ToPlayer()->GetGroup())
            {
                targets.clear();
                targets.push_back(GetCaster());
            }
            else
            {
                targets.remove(GetExplTargetUnit());
                std::list<Unit*> tempTargets;
                for (std::list<WorldObject*>::const_iterator itr = targets.begin(); itr != targets.end(); ++itr)
                    if ((*itr)->GetTypeId() == TYPEID_PLAYER && GetCaster()->IsInRaidWith((*itr)->ToUnit()))
                        tempTargets.push_back((*itr)->ToUnit());

                if (tempTargets.empty())
                {
                    targets.clear();
                    FinishCast(SPELL_FAILED_DONT_REPORT);
                    return;
                }

                Unit* target = Trinity::Containers::SelectRandomContainerElement(tempTargets);
                targets.clear();
                targets.push_back(target);
            }
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_t10_restoration_4p_bonus_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_t10_restoration_4p_bonus_SpellScript();
    }
};

// 70664 - Druid T10 Restoration 4P Bonus (Rejuvenation)
class spell_dru_t10_restoration_4p_bonus_dummy : public SpellScriptLoader
{
public:
    spell_dru_t10_restoration_4p_bonus_dummy() : SpellScriptLoader("spell_dru_t10_restoration_4p_bonus_dummy") { }

    class spell_dru_t10_restoration_4p_bonus_dummy_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_t10_restoration_4p_bonus_dummy_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ SPELL_DRUID_REJUVENATION_T10_PROC });
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            if (!spellInfo || spellInfo->Id == SPELL_DRUID_REJUVENATION_T10_PROC)
                return false;

            HealInfo* healInfo = eventInfo.GetHealInfo();
            if (!healInfo || !healInfo->GetHeal())
                return false;

            Player* caster = eventInfo.GetActor()->ToPlayer();
            if (!caster)
                return false;

            return caster->GetGroup() || caster != eventInfo.GetProcTarget();
        }

        void HandleProc(AuraEffect* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            CastSpellExtraArgs args(aurEff);
            args.SpellValueOverrides.AddBP0(eventInfo.GetHealInfo()->GetHeal());
            eventInfo.GetActor()->CastSpell(nullptr, SPELL_DRUID_REJUVENATION_T10_PROC, args);
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_dru_t10_restoration_4p_bonus_dummy_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_dru_t10_restoration_4p_bonus_dummy_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_t10_restoration_4p_bonus_dummy_AuraScript();
    }
};

// 77758 - Thrash
class spell_dru_thrash : public SpellScript
{
    PrepareSpellScript(spell_dru_thrash);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_THRASH_BEAR_AURA });
    }

    void HandleOnHitTarget(SpellEffIndex /*effIndex*/)
    {
        if (Unit* hitUnit = GetHitUnit())
        {
            Unit* caster = GetCaster();

            caster->CastSpell(hitUnit, SPELL_DRUID_THRASH_BEAR_AURA, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_dru_thrash::HandleOnHitTarget, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// 192090 - Thrash (Aura) - SPELL_DRUID_THRASH_BEAR_AURA
class spell_dru_thrash_aura : public AuraScript
{
    PrepareAuraScript(spell_dru_thrash_aura);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_BLOOD_FRENZY_AURA, SPELL_DRUID_BLOOD_FRENZY_RAGE_GAIN });
    }

    void HandlePeriodic(AuraEffect const* /*aurEff*/)
    {
        if (Unit* caster = GetCaster())
            if (caster->HasAura(SPELL_DRUID_BLOOD_FRENZY_AURA))
                caster->CastSpell(caster, SPELL_DRUID_BLOOD_FRENZY_RAGE_GAIN, true);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_thrash_aura::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
    }
};

// 1066 - Aquatic Form
// 33943 - Flight Form
// 40120 - Swift Flight Form
// 165961 - Stag Form
class spell_dru_travel_form : public SpellScriptLoader
{
public:
    spell_dru_travel_form() : SpellScriptLoader("spell_dru_travel_form") { }

    class spell_dru_travel_form_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_travel_form_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ SPELL_DRUID_FORM_STAG, SPELL_DRUID_FORM_AQUATIC_PASSIVE, SPELL_DRUID_FORM_AQUATIC, SPELL_DRUID_FORM_FLIGHT, SPELL_DRUID_FORM_SWIFT_FLIGHT });
        }

        bool Load() override
        {
            return GetCaster()->GetTypeId() == TYPEID_PLAYER;
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            // If it stays 0, it removes Travel Form dummy in AfterRemove.
            triggeredSpellId = 0;

            // We should only handle aura interrupts.
            if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_INTERRUPT)
                return;

            // Check what form is appropriate
            triggeredSpellId = spell_dru_travel_form::GetFormSpellId(GetTarget()->ToPlayer(), GetCastDifficulty(), true);

            // If chosen form is current aura, just don't remove it.
            if (triggeredSpellId == m_scriptSpellId)
                PreventDefaultAction();
        }

        void AfterRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            if (triggeredSpellId == m_scriptSpellId)
                return;

            Player* player = GetTarget()->ToPlayer();

            if (triggeredSpellId) // Apply new form
                player->CastSpell(player, triggeredSpellId, aurEff);
            else // If not set, simply remove Travel Form dummy
                player->RemoveAura(SPELL_DRUID_TRAVEL_FORM);
        }

        void Register() override
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_dru_travel_form_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_dru_travel_form_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
        }

    private:
        uint32 triggeredSpellId = 0;
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_travel_form_AuraScript();
    }

    static uint32 GetFormSpellId(Player const* player, Difficulty difficulty, bool requiresOutdoor)
    {
        // Check what form is appropriate
        if (player->HasSpell(SPELL_DRUID_FORM_AQUATIC_PASSIVE) && player->IsInWater()) // Aquatic form
            return SPELL_DRUID_FORM_AQUATIC;

        if (!player->IsInCombat() && player->GetSkillValue(SKILL_RIDING) >= 225 && CheckLocationForForm(player, difficulty, requiresOutdoor, SPELL_DRUID_FORM_FLIGHT) == SPELL_CAST_OK) // Flight form
            return player->GetSkillValue(SKILL_RIDING) >= 300 ? SPELL_DRUID_FORM_SWIFT_FLIGHT : SPELL_DRUID_FORM_FLIGHT;

        if (!player->IsInWater() && CheckLocationForForm(player, difficulty, requiresOutdoor, SPELL_DRUID_FORM_STAG) == SPELL_CAST_OK) // Stag form
            return SPELL_DRUID_FORM_STAG;

        return 0;
    }

private:
    static SpellCastResult CheckLocationForForm(Player const* targetPlayer, Difficulty difficulty, bool requireOutdoors, uint32 spell_id)
    {
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spell_id, difficulty);

        if (requireOutdoors && !targetPlayer->GetMap()->IsOutdoors(targetPlayer->GetPhaseShift(), targetPlayer->GetPositionX(), targetPlayer->GetPositionY(), targetPlayer->GetPositionZ()))
            return SPELL_FAILED_ONLY_OUTDOORS;

        return spellInfo->CheckLocation(targetPlayer->GetMapId(), targetPlayer->GetZoneId(), targetPlayer->GetAreaId(), targetPlayer);
    }
};

// 783 - Travel Form (dummy)
class spell_dru_travel_form_dummy : public SpellScriptLoader
{
public:
    spell_dru_travel_form_dummy() : SpellScriptLoader("spell_dru_travel_form_dummy") { }

    class spell_dru_travel_form_dummy_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_travel_form_dummy_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ SPELL_DRUID_FORM_AQUATIC_PASSIVE, SPELL_DRUID_FORM_AQUATIC, SPELL_DRUID_FORM_STAG });
        }

        SpellCastResult CheckCast()
        {
            Player* player = GetCaster()->ToPlayer();
            if (!player)
                return SPELL_FAILED_CUSTOM_ERROR;

            uint32 spellId = (player->HasSpell(SPELL_DRUID_FORM_AQUATIC_PASSIVE) && player->IsInWater()) ? SPELL_DRUID_FORM_AQUATIC : SPELL_DRUID_FORM_STAG;

            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId, GetCastDifficulty());
            return spellInfo->CheckLocation(player->GetMapId(), player->GetZoneId(), player->GetAreaId(), player);
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_dru_travel_form_dummy_SpellScript::CheckCast);
        }
    };

    class spell_dru_travel_form_dummy_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_travel_form_dummy_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ SPELL_DRUID_FORM_STAG, SPELL_DRUID_FORM_AQUATIC, SPELL_DRUID_FORM_FLIGHT, SPELL_DRUID_FORM_SWIFT_FLIGHT });
        }

        bool Load() override
        {
            return GetCaster()->GetTypeId() == TYPEID_PLAYER;
        }

        void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            Player* player = GetTarget()->ToPlayer();

            // Outdoor check already passed - Travel Form (dummy) has SPELL_ATTR0_OUTDOORS_ONLY attribute.
            uint32 triggeredSpellId = spell_dru_travel_form::GetFormSpellId(player, GetCastDifficulty(), false);

            player->CastSpell(player, triggeredSpellId, aurEff);
        }

        void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            // No need to check remove mode, it's safe for auras to remove each other in AfterRemove hook.
            GetTarget()->RemoveAura(SPELL_DRUID_FORM_STAG);
            GetTarget()->RemoveAura(SPELL_DRUID_FORM_AQUATIC);
            GetTarget()->RemoveAura(SPELL_DRUID_FORM_FLIGHT);
            GetTarget()->RemoveAura(SPELL_DRUID_FORM_SWIFT_FLIGHT);
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_dru_travel_form_dummy_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_dru_travel_form_dummy_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_travel_form_dummy_SpellScript();
    }

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_travel_form_dummy_AuraScript();
    }
};

// 252216 - Tiger Dash
class spell_dru_tiger_dash : public spell_dru_base_transformer
{
protected:
    bool ToCatForm() const override { return true; }
};

// 252216 - Tiger Dash (Aura)
class spell_dru_tiger_dash_aura : public AuraScript
{
    PrepareAuraScript(spell_dru_tiger_dash_aura);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        if (AuraEffect* effRunSpeed = GetEffect(EFFECT_0))
        {
            int32 reduction = aurEff->GetAmount();
            effRunSpeed->ChangeAmount(effRunSpeed->GetAmount() - reduction);
        }
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_tiger_dash_aura::HandlePeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
    }
};

// 48438 - Wild Growth
class spell_dru_wild_growth : public SpellScriptLoader
{
public:
    spell_dru_wild_growth() : SpellScriptLoader("spell_dru_wild_growth") { }

    class spell_dru_wild_growth_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_wild_growth_SpellScript);

        bool Validate(SpellInfo const* spellInfo) override
        {
            SpellEffectInfo const* effect2 = spellInfo->GetEffect(EFFECT_2);
            if (!effect2 || effect2->IsEffect() || effect2->CalcValue() <= 0)
                return false;
            return true;
        }

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if(RaidCheck(GetCaster()));

            uint32 const maxTargets = uint32(GetSpellInfo()->GetEffect(EFFECT_2)->CalcValue(GetCaster()));

            if (targets.size() > maxTargets)
            {
                targets.sort(Trinity::HealthPctOrderPred());
                targets.resize(maxTargets);
            }

            _targets = targets;
        }

        void SetTargets(std::list<WorldObject*>& targets)
        {
            targets = _targets;
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_wild_growth_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_wild_growth_SpellScript::SetTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ALLY);
        }

        std::list<WorldObject*> _targets;
    };

    class spell_dru_wild_growth_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_wild_growth_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ SPELL_DRUID_RESTORATION_T10_2P_BONUS });
        }

        void HandleTickUpdate(AuraEffect* aurEff)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            // calculate from base damage, not from aurEff->GetAmount() (already modified)
            float damage = caster->CalculateSpellDamage(GetUnitOwner(), GetSpellInfo(), aurEff->GetEffIndex());

            // Wild Growth = first tick gains a 6% bonus, reduced by 2% each tick
            float reduction = 2.f;
            if (AuraEffect* bonus = caster->GetAuraEffect(SPELL_DRUID_RESTORATION_T10_2P_BONUS, EFFECT_0))
                reduction -= CalculatePct(reduction, bonus->GetAmount());
            reduction *= (aurEff->GetTickNumber() - 1);

            AddPct(damage, 6.f - reduction);
            aurEff->SetAmount(int32(damage));
        }

        void Register() override
        {
            OnEffectUpdatePeriodic += AuraEffectUpdatePeriodicFn(spell_dru_wild_growth_AuraScript::HandleTickUpdate, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_wild_growth_SpellScript();
    }

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_wild_growth_AuraScript();
    }
};

// 102560 Incarnation : Chosen of Elune -
// @Version : 7.1.0.22908
class spell_dru_incarnation_chosen_of_elune : public SpellScriptLoader
{
public:
    spell_dru_incarnation_chosen_of_elune() : SpellScriptLoader("spell_dru_incarnation_chosen_of_elune") { }

    class spell_dru_incarnation_chosen_of_elune_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_incarnation_chosen_of_elune_SpellScript);

        void OnActivate()
        {
            if (Player* player = GetCaster()->ToPlayer())
                if (!player->HasAura(SPELL_DRUID_MOONKIN_FORM))
                    player->CastSpell(player, SPELL_DRUID_MOONKIN_FORM, true);
        }

        void Register() override
        {
            OnCast += SpellCastFn(spell_dru_incarnation_chosen_of_elune_SpellScript::OnActivate);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_incarnation_chosen_of_elune_SpellScript();
    }
};

// 102543 Incarnation : King of the Jungle -
// @Version : 7.1.0.22908
class spell_dru_incarnation_king_of_the_jungle : public SpellScriptLoader
{
public:
    spell_dru_incarnation_king_of_the_jungle() : SpellScriptLoader("spell_dru_incarnation_king_of_the_jungle") { }

    class spell_dru_incarnation_king_of_the_jungle_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_incarnation_king_of_the_jungle_SpellScript);

        void OnActivate()
        {
            if (Player* player = GetCaster()->ToPlayer())
                if (!player->HasAura(SPELL_DRUID_CAT_FORM))
                    player->CastSpell(player, SPELL_DRUID_CAT_FORM, true);
        }

        void Register() override
        {
            OnCast += SpellCastFn(spell_dru_incarnation_king_of_the_jungle_SpellScript::OnActivate);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_incarnation_king_of_the_jungle_SpellScript();
    }
};

// 197488 Balance Affinity (Feral, Guardian) -
// @Version : 7.1.0.22908
class spell_dru_balance_affinity_dps : public SpellScriptLoader
{
public:
    spell_dru_balance_affinity_dps() : SpellScriptLoader("spell_dru_balance_affinity_dps") {}

    class spell_dru_balance_affinity_dps_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_balance_affinity_dps_AuraScript);

        void LearnSpells(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (Player* player = caster->ToPlayer())
            {
                player->AddTemporarySpell(SPELL_DRUID_MOONKIN_FORM);
                player->AddTemporarySpell(SPELL_DRUID_STARSURGE);
                player->AddTemporarySpell(SPELL_DRUID_LUNAR_STRIKE);
                player->AddTemporarySpell(SPELL_DRUID_SOLAR_WRATH);
                player->AddTemporarySpell(SPELL_DRUID_SUNFIRE);
            }
        }

        void UnlearnSpells(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (Player* player = caster->ToPlayer())
            {
                player->RemoveTemporarySpell(SPELL_DRUID_MOONKIN_FORM);
                player->RemoveTemporarySpell(SPELL_DRUID_STARSURGE);
                player->RemoveTemporarySpell(SPELL_DRUID_LUNAR_STRIKE);
                player->RemoveTemporarySpell(SPELL_DRUID_SOLAR_WRATH);
                player->RemoveTemporarySpell(SPELL_DRUID_SUNFIRE);
            }
        }

        void Register() override
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_dru_balance_affinity_dps_AuraScript::UnlearnSpells, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectApply += AuraEffectApplyFn(spell_dru_balance_affinity_dps_AuraScript::LearnSpells, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_balance_affinity_dps_AuraScript();
    }
};


// 102558 Incarnation : Guardian of Ursoc -
// @Version : 7.1.0.22908
class spell_dru_incarnation_guardian_of_ursoc : public SpellScriptLoader
{
public:
    spell_dru_incarnation_guardian_of_ursoc() : SpellScriptLoader("spell_dru_incarnation_guardian_of_ursoc") { }

    class spell_dru_incarnation_guardian_of_ursoc_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_incarnation_guardian_of_ursoc_SpellScript);

        void OnActivate()
        {
            if (Player* player = GetCaster()->ToPlayer())
                if (!player->HasAura(SPELL_DRUID_BEAR_FORM))
                    player->CastSpell(player, SPELL_DRUID_BEAR_FORM, true);
        }

        void Register() override
        {
            OnCast += SpellCastFn(spell_dru_incarnation_guardian_of_ursoc_SpellScript::OnActivate);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_incarnation_guardian_of_ursoc_SpellScript();
    }
};

// 197632 Balance Affinity (Restoration) -
// @Version : 7.1.0.22908
class spell_dru_balance_affinity_resto : public SpellScriptLoader
{
public:
    spell_dru_balance_affinity_resto() : SpellScriptLoader("spell_dru_balance_affinity_resto") {}

    class spell_dru_balance_affinity_resto_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_balance_affinity_resto_AuraScript);

        void LearnSpells(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (Player* player = caster->ToPlayer())
            {
                player->AddTemporarySpell(SPELL_DRUID_MOONKIN_FORM);
                player->AddTemporarySpell(SPELL_DRUID_STARSURGE);
                player->AddTemporarySpell(SPELL_DRUID_LUNAR_STRIKE);
            }
        }

        void UnlearnSpells(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (Player* player = caster->ToPlayer())
            {
                player->RemoveTemporarySpell(SPELL_DRUID_MOONKIN_FORM);
                player->RemoveTemporarySpell(SPELL_DRUID_STARSURGE);
                player->RemoveTemporarySpell(SPELL_DRUID_LUNAR_STRIKE);
            }
        }

        void Register() override
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_dru_balance_affinity_resto_AuraScript::UnlearnSpells, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectApply += AuraEffectApplyFn(spell_dru_balance_affinity_resto_AuraScript::LearnSpells, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_balance_affinity_resto_AuraScript();
    }
};

//102359
class spell_dru_mass_entanglement : public SpellScript
{
    PrepareSpellScript(spell_dru_mass_entanglement);

    void HandleCast()
    {
        std::list<Unit*> targetList;
        GetCaster()->GetAttackableUnitListInRange(targetList, 15.0f);
        if (targetList.size())
            for (auto& targets : targetList)
            {
                GetCaster()->AddAura(SPELL_DRU_MASS_ENTANGLEMENT, targets);
            }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_dru_mass_entanglement::HandleCast);
    }
};

//274902
class spell_dru_photosynthesis : public AuraScript
{
    PrepareAuraScript(spell_dru_photosynthesis);

    void OnApply(const AuraEffect* /* aurEff */, AuraEffectHandleModes /*mode*/)
    {
        if (!GetCaster()->HasAura(SPELL_DRU_PHOTOSYNTHESIS_MOD_HEAL_TICKS))
            GetCaster()->AddAura(SPELL_DRU_PHOTOSYNTHESIS_MOD_HEAL_TICKS, NULL);
    }

    void OnRemove( const AuraEffect* /* aurEff */, AuraEffectHandleModes /*mode*/)
    {
        if (GetCaster()->HasAura(SPELL_DRU_PHOTOSYNTHESIS_MOD_HEAL_TICKS))
            GetCaster()->RemoveAura(SPELL_DRU_PHOTOSYNTHESIS_MOD_HEAL_TICKS);
    }

    void Register() 
    {
        OnEffectApply += AuraEffectApplyFn(spell_dru_photosynthesis::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_dru_photosynthesis::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

//10682, 204066
struct at_dru_lunar_beam : AreaTriggerAI
{
    at_dru_lunar_beam(AreaTrigger* at) : AreaTriggerAI(at) { }

    void OnCreate() override
    {
        at->SetPeriodicProcTimer(1000);
    }

    void OnPeriodicProc() //override ( not used )
    {
        if (at->GetCaster())
            at->GetCaster()->CastSpell(at->GetPosition(), SPELL_DRU_LUNAR_BEAM_DAMAGE_HEAL, true);
    }
};

//3020, 102793
struct at_dru_ursol_vortex : AreaTriggerAI
{
    at_dru_ursol_vortex(AreaTrigger* at) : AreaTriggerAI(at) { }

    void OnUnitEnter(Unit* target) override
    {
        VALIDATE_CASTER();

        if (caster->IsInCombatWith(target))
            caster->CastSpell(target, SPELL_DRU_URSOL_VORTEX_DEBUFF, true);
    }

    void OnUnitExit(Unit* target) override
    {
        target->RemoveAurasDueToSpell(SPELL_DRU_URSOL_VORTEX_DEBUFF);
        if (!_hasPull && target->IsValidAttackTarget(at->GetCaster()))
        {
            _hasPull = true;
            target->CastSpell(at->GetPosition(), SPELL_DRU_URSOL_VORTEX_PULL, true);
        }
    }

private:
    bool _hasPull = false;
};

// 22842 - Frenzied Regeneration
class aura_dru_frenzied_regeneration : public AuraScript
{
    PrepareAuraScript(aura_dru_frenzied_regeneration);

    void CalcAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
    {
        uint64 healAmount = CalculatePct(GetCaster()->GetDamageOverLastSeconds(5), 50);
        uint64 minHealAmount = CalculatePct(GetCaster()->GetMaxHealth(), 5);
        healAmount = std::max(healAmount, minHealAmount);

        // Divide amount over duration
        healAmount /= (GetMaxDuration() / aurEff->GetPeriodicTimer());

        amount = (int32)healAmount;
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(aura_dru_frenzied_regeneration::CalcAmount, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
    }
};

// 58180 - Infected Wounds
class spell_dru_infected_wounds : public SpellScriptLoader
{
public:
    spell_dru_infected_wounds() : SpellScriptLoader("spell_dru_infected_wounds") { }


    class spell_dru_infected_wounds_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_infected_wounds_SpellScript);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (!GetCaster())
                return;

            if (GetCaster()->HasAura(GetSpellInfo()->Id))
                GetCaster()->RemoveAurasDueToSpell(GetSpellInfo()->Id);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_dru_infected_wounds_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_infected_wounds_SpellScript();
    }
};

// 190984 - Solar Wrath | 194153 - Lunar Strike
class spell_dru_blessing_of_elune : public SpellScript
{
    PrepareSpellScript(spell_dru_blessing_of_elune);

    void HandleOnHit()
    {
        Unit* caster = GetCaster();

        if (!caster)
            return;

        uint32 power = GetHitDamage();

        if (Aura* aura = caster->GetAura(202737))
            if (AuraEffect* aurEff = aura->GetEffect(EFFECT_0))
                power += CalculatePct(power, aurEff->GetAmount());

        SetHitDamage(power);
    }

    void Register()
    {
        OnHit += SpellHitFn(spell_dru_blessing_of_elune::HandleOnHit);
    }
};

// 208253 - Essence of G'Hanir
class spell_dru_essence_of_ghanir : public SpellScriptLoader
{
public:
    spell_dru_essence_of_ghanir() : SpellScriptLoader("spell_dru_essence_of_ghanir") { }

    class spell_dru_essence_of_ghanir_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_essence_of_ghanir_AuraScript);

        void HandleEffectCalcSpellMod(AuraEffect const* aurEff, SpellModifier*& spellMod)
        {
            if (!spellMod)
            {
                spellMod = new SpellModifier(GetAura());
                spellMod->op = SpellModOp(aurEff->GetMiscValue());
                spellMod->type = SPELLMOD_PCT;
                spellMod->spellId = GetId();
                spellMod->mask = aurEff->GetSpellEffectInfo()->SpellClassMask;
            }

            spellMod->value = aurEff->GetAmount();
        }

        void Register() override
        {
            DoEffectCalcSpellMod += AuraEffectCalcSpellModFn(spell_dru_essence_of_ghanir_AuraScript::HandleEffectCalcSpellMod, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dru_essence_of_ghanir_AuraScript();
    }
};

// Brutal Slash - 202028
class spell_dru_brutal_slash : public SpellScript
{
    PrepareSpellScript(spell_dru_brutal_slash);

    void HandleOnHit()
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();
        if (!caster || !target)
            return;

        // This prevent awarding multiple Combo Points when multiple targets hit with Brutal Slash AoE
        if (m_awardComboPoint)
            // Awards the caster 1 Combo Point (get value from the spell data)
            caster->ModifyPower(POWER_COMBO_POINTS, sSpellMgr->GetSpellInfo(SPELL_DRUID_SWIPE_CAT, DIFFICULTY_NONE)->GetEffect(EFFECT_0)->BasePoints);

        m_awardComboPoint = false;
    }

    void Register() override
    {
        OnHit += SpellHitFn(spell_dru_brutal_slash::HandleOnHit);
    }

private:
    bool m_awardComboPoint = true;
};

// Maul (Bear Form) - 6807
class spell_dru_maul_bear : public SpellScript
{
    PrepareSpellScript(spell_dru_maul_bear);

    void OnHit(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();
        if (!caster || !target)
            return;
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_dru_maul_bear::OnHit, EFFECT_0, SPELL_EFFECT_DUMMY);
        OnEffectHitTarget += SpellEffectFn(spell_dru_maul_bear::OnHit, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// 157982 - Tranquility Heal
class spell_dru_tranquility_heal : public SpellScriptLoader
{
public:
    spell_dru_tranquility_heal() : SpellScriptLoader("spell_dru_tranquility_heal") { }

    class spell_dru_tranquility_heal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_tranquility_heal_SpellScript);

        void HandleHeal(SpellEffIndex /*effIndex*/)
        {
            if (!GetCaster())
                return;

            if (Unit* caster = GetCaster())
            {
                uint32 heal = CalculatePct(caster->SpellBaseHealingBonusDone(SPELL_SCHOOL_MASK_NATURE), 180);
                SetHitHeal(heal);
            }
        }

        void Register()
        {
            OnEffectHit += SpellEffectFn(spell_dru_tranquility_heal_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_dru_tranquility_heal_SpellScript();
    }
};

// 200389 - Cultivation
class spell_dru_cultivation : public SpellScriptLoader
{
public:
    spell_dru_cultivation() : SpellScriptLoader("spell_dru_cultivation") { }

    class spell_dru_cultivation_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_cultivation_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (!GetCaster())
                return;

            amount = CalculatePct(GetCaster()->SpellBaseHealingBonusDone(SPELL_SCHOOL_MASK_NATURE), 60);
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_cultivation_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_cultivation_AuraScript();
    }
};

// 197492 - Restoration Affinity
class aura_dru_restoration_affinity : public AuraScript
{
    PrepareAuraScript(aura_dru_restoration_affinity);

    const std::vector<uint32> LearnedSpells =
    {
        SPELL_DRUID_YSERA_GIFT,
        SPELL_DRUID_REJUVENATION,
        SPELL_DRUID_HEALING_TOUCH,
        SPELL_DRUID_SWIFTMEND
    };

    void AfterApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Player* target = GetTarget()->ToPlayer())
            for (uint32 spellId : LearnedSpells)
                target->LearnSpell(spellId, false);
    }

    void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Player* target = GetTarget()->ToPlayer())
            for (uint32 spellId : LearnedSpells)
                target->RemoveSpell(spellId);
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(aura_dru_restoration_affinity::AfterApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(aura_dru_restoration_affinity::AfterRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// 774 - Rejuvenation
class spell_dru_rejuvenation : public SpellScriptLoader
{
public:
    spell_dru_rejuvenation() : SpellScriptLoader("spell_dru_rejuvenation") { }

    class spell_dru_rejuvenation_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_rejuvenation_AuraScript);

        void HandleCalculateAmount(AuraEffect const* /*p_AurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* caster = GetCaster())
            {
                if (caster->HasAura(SPELL_DRUID_SOUL_OF_THE_FOREST_RESTO) && !caster->HasAura(SPELL_DRUID_REJUVENATION))
                {
                    amount *= 2;
                    caster->RemoveAura(SPELL_DRUID_SOUL_OF_THE_FOREST_RESTO);
                }
            }
        }

        void OnApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
        {
            Unit* caster = GetCaster();

            if (!caster)
                return;

            if (AuraEffect* GlyphOfRejuvenation = caster->GetAuraEffect(SPELL_DRU_GLYPH_OF_REJUVENATION, EFFECT_0))
            {
                GlyphOfRejuvenation->SetAmount(GlyphOfRejuvenation->GetAmount() + 1);
                if (GlyphOfRejuvenation->GetAmount() >= 3)
                    caster->CastSpell(caster, SPELL_DRU_GLYPH_OF_REJUVENATION_EFFECT, true);
            }
        }

        void OnPeriodic(AuraEffect const* AurEff)
        {
            if (Unit* target = GetTarget())
                if (GetCaster()->HasAura(SPELL_DRU_CULTIVATION) && !target->HasAura(SPELL_DRU_CULTIVATION_HEAL) && target->HealthBelowPct(60))
                    GetCaster()->CastSpell(target, SPELL_DRU_CULTIVATION_HEAL, true);
        }

        void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
        {
            Unit* caster = GetCaster();

            if (!caster)
                return;

            if (AuraEffect* l_GlyphOfRejuvenation = caster->GetAuraEffect(SPELL_DRU_GLYPH_OF_REJUVENATION, EFFECT_0))
            {
                l_GlyphOfRejuvenation->SetAmount(l_GlyphOfRejuvenation->GetAmount() - 1);
                if (l_GlyphOfRejuvenation->GetAmount() < 3)
                    caster->RemoveAura(SPELL_DRU_GLYPH_OF_REJUVENATION_EFFECT);
            }
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_dru_rejuvenation_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_dru_rejuvenation_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_rejuvenation_AuraScript::HandleCalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_rejuvenation_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_rejuvenation_AuraScript();
    }

    class spell_dru_rejuvenation_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_rejuvenation_SpellScript);

        int32 m_RejuvenationAura = 0;
        int32 m_RejuvenationAuraAmount = 0;

        void HandleAfterHit()
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            Unit* target = GetHitUnit();
            if (!target)
                return;

            Aura* RejuvenationAura = target->GetAura(SPELL_DRUID_REJUVENATION, caster->GetGUID());

            if (RejuvenationAura && m_RejuvenationAura > 0)
                RejuvenationAura->SetDuration(m_RejuvenationAura);

            if (AuraEffect* NewRejuvenationAuraEffect = target->GetAuraEffect(SPELL_DRUID_REJUVENATION, EFFECT_0))
            {
                if (caster->HasAura(SPELL_DRUID_SOUL_OF_THE_FOREST_RESTO))
                {
                    NewRejuvenationAuraEffect->SetAmount(NewRejuvenationAuraEffect->GetAmount() * 2);
                    caster->RemoveAura(SPELL_DRUID_SOUL_OF_THE_FOREST_RESTO);
                }
            }
        }

        void HandleBeforeHit(SpellMissInfo missInfo)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            Unit* target = GetHitUnit();
            if (!target)
                return;

            if (caster->HasAura(SPELL_DRUID_SOUL_OF_THE_FOREST_RESTO))
            {
                //NewRejuvenationAuraEffect->SetAmount(NewRejuvenationAuraEffect->GetAmount() * 2);
                SetHitHeal(GetHitHeal() * 2);
                //caster->RemoveAura(SPELL_DRUID_SOUL_OF_THE_FOREST_RESTO);
            }

            if (caster->HasAura(SPELL_DRU_GERMINATION) && target->HasAura(SPELL_DRUID_REJUVENATION, caster->GetGUID()))
            {
                Aura* RejuvenationAura = target->GetAura(SPELL_DRUID_REJUVENATION, caster->GetGUID());
                if (!RejuvenationAura)
                    return;

                if (!target->HasAura(SPELL_DRU_GERMINATION_HOT, caster->GetGUID()))
                {
                    caster->CastSpell(target, SPELL_DRU_GERMINATION_HOT, true);
                    m_RejuvenationAura = RejuvenationAura->GetDuration();
                }
                else
                {
                    Aura* GerminationAura = target->GetAura(SPELL_DRU_GERMINATION_HOT, caster->GetGUID());
                    Aura* RejuvenationAura = target->GetAura(SPELL_DRUID_REJUVENATION, caster->GetGUID());
                    if (GerminationAura && RejuvenationAura)
                    {
                        int32 GerminationDuration = GerminationAura->GetDuration();
                        int32 RejuvenationDuration = RejuvenationAura->GetDuration();
                        if (GerminationDuration > RejuvenationDuration)
                        {
                            caster->AddAura(SPELL_DRUID_REJUVENATION, target);
                        }
                        else
                        {
                            caster->CastSpell(target, SPELL_DRU_GERMINATION_HOT, true);
                            m_RejuvenationAura = RejuvenationDuration;
                        }
                    }
                }
            }
        }

        void Register()
        {
            BeforeHit += BeforeSpellHitFn(spell_dru_rejuvenation_SpellScript::HandleBeforeHit);
            AfterHit += SpellHitFn(spell_dru_rejuvenation_SpellScript::HandleAfterHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_dru_rejuvenation_SpellScript();
    }
};

// 202157 - Feral Affinity
class aura_dru_feral_affinity : public AuraScript
{
    PrepareAuraScript(aura_dru_feral_affinity);

    const std::vector<uint32> LearnedSpells =
    {
        SPELL_DRUID_FELINE_SWIFTNESS,
        SPELL_DRUID_SHRED,
        SPELL_DRUID_RAKE,
        SPELL_DRUID_RIP,
        SPELL_DRUID_FEROCIOUS_BITE,
        SPELL_DRUID_SWIPE_CAT
    };

    void AfterApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Player* target = GetTarget()->ToPlayer())
            for (uint32 spellId : LearnedSpells)
                target->LearnSpell(spellId, false);
    }

    void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Player* target = GetTarget()->ToPlayer())
            for (uint32 spellId : LearnedSpells)
                target->RemoveSpell(spellId);
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(aura_dru_feral_affinity::AfterApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(aura_dru_feral_affinity::AfterRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// 24858  Moonkin Form
// 102560 Chosen of Elune
// 197625
class aura_dru_astral_form : public AuraScript
{
    PrepareAuraScript(aura_dru_astral_form);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_GLYPH_OF_STARS });
    }

    void AfterApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* target = GetTarget();
        if (target->HasAura(SPELL_DRUID_GLYPH_OF_STARS))
        {
            target->SetDisplayId(target->GetNativeDisplayId());
            target->AddAura(SPELL_DRUID_BLUE_COLOR, target);
            target->AddAura(SPELL_DRUID_SHADOWY_GHOST, target);
            target->CastSpell(target, sSpellMgr->GetSpellInfo(SPELL_DRUID_GLYPH_OF_STARS, DIFFICULTY_NONE)->GetEffect(EFFECT_0)->BasePoints, true);
        }
    }

    void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* target = GetTarget();
        if (target->HasAura(SPELL_DRUID_MOONKIN_FORM) || target->HasAura(SPELL_DRUID_CHOSEN_OF_ELUNE))
            return;

        target->RemoveAura(sSpellMgr->GetSpellInfo(SPELL_DRUID_GLYPH_OF_STARS, DIFFICULTY_NONE)->GetEffect(EFFECT_0)->BasePoints);
        target->RemoveAura(SPELL_DRUID_BLUE_COLOR);
        target->RemoveAura(SPELL_DRUID_SHADOWY_GHOST);
    }

    void Register() override
    {
        switch (m_scriptSpellId)
        {
        case 197625:
        case 24858:
            AfterEffectApply += AuraEffectApplyFn(aura_dru_astral_form::AfterApply, EFFECT_1, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(aura_dru_astral_form::AfterRemove, EFFECT_1, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
            break;
        case 102560:
            AfterEffectApply += AuraEffectApplyFn(aura_dru_astral_form::AfterApply, EFFECT_1, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(aura_dru_astral_form::AfterRemove, EFFECT_1, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
            break;
        }
    }
};

// 80313 Pulverize
class spell_druid_pulverize : public SpellScript
{
    PrepareSpellScript(spell_druid_pulverize);

    enum Spells
    {
        SPELL_DRUID_PULVERIZE = 80313,
        SPELL_DRUID_TRASH_DOT_TWO_STACKS_MARKER = 158790,
        SPELL_DRUID_PULVERIZE_DAMAGE_REDUCTION_BUFF = 158792,
    };

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_PULVERIZE, SPELL_DRUID_TRASH_DOT_TWO_STACKS_MARKER });
    }

    void HandleHitTarget(SpellEffIndex /*effIndex*/)
    {
        if (Unit* target = GetHitUnit())
        {
            target->RemoveAurasDueToSpell(SPELL_DRUID_TRASH_DOT_TWO_STACKS_MARKER);
            GetCaster()->CastSpell(target, SPELL_DRUID_PULVERIZE_DAMAGE_REDUCTION_BUFF, true);
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_druid_pulverize::HandleHitTarget, EFFECT_2, SPELL_EFFECT_DUMMY);
    }
};
// 204053 Rend and Tear
class spell_druid_rend_and_tear : public AuraScript
{
    PrepareAuraScript(spell_druid_rend_and_tear);

    enum Spells
    {
        SPELL_DRUID_REND_AND_TEAR = 204053,
    };

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_REND_AND_TEAR, SPELL_DRUID_THRASH_BEAR_AURA });
    }

    void CalculateAmount(AuraEffect const* /*auraEffect*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        amount = -1;
    }

    void Absorb(AuraEffect* auraEffect, DamageInfo& dmgInfo, uint32& absorbAmount)
    {
        Unit* caster = GetCaster();
        Unit* attacker = dmgInfo.GetAttacker();
        absorbAmount = 0;

        if (!caster || !attacker || !HasEffect(EFFECT_1))
            return;

        if (caster->GetShapeshiftForm() == FORM_BEAR_FORM)
            if (Aura* trashDOT = attacker->GetAura(SPELL_DRUID_THRASH_BEAR_AURA, caster->GetGUID()))
                absorbAmount = CalculatePct(dmgInfo.GetDamage(), trashDOT->GetStackAmount() * GetSpellInfo()->GetEffect(EFFECT_1)->BasePoints);
    }

    void HandleEffectCalcSpellMod(AuraEffect const* aurEff, SpellModifier*& spellMod)
    {
        if (!spellMod)
            return;

        spellMod->value = GetCaster()->GetShapeshiftForm() == FORM_BEAR_FORM ? aurEff->GetAmount() : 0;
    }

    void Register()
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_druid_rend_and_tear::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
        OnEffectAbsorb += AuraEffectAbsorbFn(spell_druid_rend_and_tear::Absorb, EFFECT_0);
        DoEffectCalcSpellMod += AuraEffectCalcSpellModFn(spell_druid_rend_and_tear::HandleEffectCalcSpellMod, EFFECT_1, SPELL_AURA_ADD_FLAT_MODIFIER);
        DoEffectCalcSpellMod += AuraEffectCalcSpellModFn(spell_druid_rend_and_tear::HandleEffectCalcSpellMod, EFFECT_2, SPELL_AURA_ADD_FLAT_MODIFIER);
    }
};

//Incarnation: Tree of Life 33891
class incarnation_tree_of_life : public SpellScript
{
    PrepareSpellScript(incarnation_tree_of_life);

    void HandleAfterCast()
    {
        Unit* caster = GetCaster();
        if (Aura* tree = caster->GetAura(33891))
        {
            tree->SetDuration(30000, true);
        }
    }

    void Register() override
    {
        AfterCast += SpellCastFn(incarnation_tree_of_life::HandleAfterCast);
    }
};

//274837
class spell_feral_frenzy : public SpellScript
{
    PrepareSpellScript(spell_feral_frenzy);

    void HandleOnHit()
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();

        if (!caster || !target)
            return;

        this->strikes = 0;

        int32 strikeDamage = 100 / 20 + caster->m_unitData->AttackPower;

        caster->GetScheduler().Schedule(50ms, [caster, target, strikeDamage, this](TaskContext context)
            {
                if (caster->GetDistance2d(target) <= 5.0f)
                {
                    caster->DealDamage(target, strikeDamage, 0, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, 0, true);
                    strikes++;

                    if (this->strikes < 5)
                        context.Repeat(200ms);

                    else if (this->strikes == 5)
                    {
                        caster->CastSpell(target, SPELL_FERAL_FRENZY_BLEED, true);
                        int32 bleedDamage = 100 / 10 + caster->m_unitData->AttackPower;
                        caster->DealDamage(target, bleedDamage, 0, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, 0, true);
                    }
                }
            });
    }

    void Register() override
    {
        OnHit += SpellHitFn(spell_feral_frenzy::HandleOnHit);
    }

private:
    uint8 strikes;
};

class feral_spells : public PlayerScript
{
public:
    feral_spells() : PlayerScript("feral_spells") { }

    void OnLogin(Player* player, bool) override
    {
        if (player->GetSpecializationId() != TALENT_SPEC_DRUID_CAT)
            return;

        if (player->getLevel() >= 5 && !player->HasSpell(SPELL_DRUID_SHRED))
            player->LearnSpell(SPELL_DRUID_SHRED, false, true);

        if (player->getLevel() >= 20 && !player->HasSpell(SPELL_DRUID_RIP))
            player->LearnSpell(SPELL_DRUID_RIP, false, true);

        if (player->getLevel() >= 24 && !player->HasSpell(SPELL_DRUID_RAKE))
            player->LearnSpell(SPELL_DRUID_RAKE, false, true);

        if (player->getLevel() >= 32 && !player->HasSpell(SPELL_DRUID_FEROCIOUS_BITE))
            player->LearnSpell(SPELL_DRUID_FEROCIOUS_BITE, false, true);
    }
};

//191034
class spell_dru_starfall : public SpellScript
{
    PrepareSpellScript(spell_dru_starfall);

    void HandleOnHit()
    {
        if (GetCaster())
            if (GetCaster()->GetAuraCount(SPELL_DRU_STARLORD_BUFF) < 3)
                GetCaster()->CastSpell(nullptr, SPELL_DRU_STARLORD_BUFF, true);
    }

    void Register() override
    {
        OnHit += SpellHitFn(spell_dru_starfall::HandleOnHit);
    }
};

// 102383 Wild Charge (Moonkin) -
// @Version : 7.1.0.22908
class spell_dru_wild_charge_moonkin : public SpellScriptLoader
{
public:
    spell_dru_wild_charge_moonkin() : SpellScriptLoader("spell_dru_wild_charge_moonkin") { }

    class spell_dru_wild_charge_moonkin_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_wild_charge_moonkin_SpellScript);

        SpellCastResult CheckFight()
        {
            if (GetCaster())
            {
                if (!GetCaster()->IsInCombat())
                    return SPELL_FAILED_DONT_REPORT;
            }
            else
                return SPELL_FAILED_DONT_REPORT;

            return SPELL_CAST_OK;
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_dru_wild_charge_moonkin_SpellScript::CheckFight);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_wild_charge_moonkin_SpellScript();
    }
};

// Efflorescence
// NPC Id - 47649
class npc_dru_efflorescence : public ScriptedAI
{
public:
    npc_dru_efflorescence(Creature* creature) : ScriptedAI(creature) {}

    void Reset() override
    {
        me->CastSpell(me, SPELL_DRUID_EFFLORESCENCE_DUMMY, true);
        me->AddUnitFlag(UNIT_FLAG_NON_ATTACKABLE);
        me->AddUnitFlag(UNIT_FLAG_NOT_SELECTABLE);
        me->AddUnitFlag(UNIT_FLAG_REMOVE_CLIENT_CONTROL);
        me->SetReactState(REACT_PASSIVE);
    }
};

// Overgrowth - 203651
class spell_dru_overgrowth : public SpellScriptLoader
{
public:
    spell_dru_overgrowth() : SpellScriptLoader("spell_dru_overgrowth") { }

    class spell_dru_overgrowth_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_overgrowth_SpellScript);

        enum
        {
            SPELL_DRUID_REJUVENATION = 774,
            SPELL_DRUID_WILD_GROWTH = 48438,
            SPELL_DRUID_LIFE_BLOOM = 33763,
            SPELL_DRUID_REGROWTH = 8936
        };

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (Unit* caster = GetCaster())
            {
                if (Unit* target = GetHitUnit())
                {
                    caster->AddAura(SPELL_DRUID_REJUVENATION, target);
                    caster->AddAura(SPELL_DRUID_WILD_GROWTH, target);
                    caster->AddAura(SPELL_DRUID_LIFE_BLOOM, target);
                    caster->AddAura(SPELL_DRUID_REGROWTH, target);
                }
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_dru_overgrowth_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_overgrowth_SpellScript();
    }
};


// 61391 - Typhoon
class spell_dru_typhoon : public SpellScriptLoader
{
public:
    spell_dru_typhoon() : SpellScriptLoader("spell_dru_typhoon") { }

    class spell_dru_typhoon_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_typhoon_SpellScript);

        void HandleKnockBack(SpellEffIndex effIndex)
        {
            // Glyph of Typhoon
            if (GetCaster()->HasAura(SPELL_DRUID_GLYPH_OF_TYPHOON))
                PreventHitDefaultEffect(effIndex);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_dru_typhoon_SpellScript::HandleKnockBack, EFFECT_0, SPELL_EFFECT_KNOCK_BACK);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_typhoon_SpellScript();
    }
};


void AddSC_druid_spell_scripts()
{
    new spell_dru_typhoon();
    new spell_dru_overgrowth();
    RegisterSpellScript(spell_dru_starsurge);
    RegisterSpellScript(spell_dru_starfall);
    RegisterSpellScript(spell_feral_frenzy);
    RegisterPlayerScript(feral_spells);
    RegisterSpellScript(incarnation_tree_of_life);
    RegisterAuraScript(spell_druid_rend_and_tear);
    RegisterSpellScript(spell_druid_pulverize);
    RegisterAuraScript(aura_dru_astral_form);
    RegisterAuraScript(aura_dru_feral_affinity);
    new spell_dru_rejuvenation();
    new spell_dru_cultivation();
    RegisterAuraScript(aura_dru_frenzied_regeneration);
    RegisterAuraScript(aura_dru_restoration_affinity);
    RegisterAuraScript(spell_dru_barkskin);
    RegisterSpellScript(spell_dru_berserk);
    RegisterAuraScript(spell_dru_brambles);
    RegisterAuraScript(spell_dru_bristling_fur);
    RegisterSpellScript(spell_dru_thrash_cat);
    RegisterAuraScript(spell_dru_cat_form);
    new spell_dru_balance_affinity_dps();
    RegisterSpellScript(spell_dru_maul_bear);
    new spell_dru_essence_of_ghanir();
    new spell_dru_balance_affinity_resto();
    RegisterSpellScript(spell_dru_brutal_slash);
    new spell_dru_infected_wounds();
    new spell_dru_dash();
    new spell_dru_tranquility_heal();
    RegisterSpellScript(spell_dru_swipe);
    new spell_dru_primal_fury();
    RegisterSpellScript(spell_dru_blessing_of_elune);
    RegisterAuraScript(spell_dru_earthwarden);
    RegisterAuraScript(spell_dru_eclipse_aura);
    RegisterAuraScript(spell_dru_eclipse_dummy);
    RegisterAuraScript(spell_dru_eclipse_ooc);
    RegisterSpellScript(spell_dru_ferocious_bite);
    new spell_dru_flight_form();
    new spell_dru_forms_trinket();
    RegisterAuraScript(spell_dru_galactic_guardian);
    RegisterPlayerScript(dru_predator);
    RegisterAuraScript(spell_dru_gore);
    new spell_dru_idol_lifebloom();
    RegisterSpellScript(spell_dru_incapacitating_roar);
    new spell_dru_innervate();
    new spell_dru_item_t6_trinket();
    new spell_dru_lifebloom();
    new spell_dru_predatory_swiftness();
    new spell_dru_predatory_swiftness_aura();
    new spell_dru_living_seed();
    new spell_dru_living_seed_proc();
    new spell_dru_moonfire();
    new spell_dru_omen_of_clarity();
    RegisterSpellScript(spell_dru_mass_entanglement);
    new spell_dru_predatory_strikes();
    RegisterSpellScript(spell_dru_prowl);
    new spell_dru_rip();
    new spell_dru_savage_roar();
    RegisterSpellScript(spell_dru_skull_bash);
    new spell_dru_stampede();
    RegisterSpellScript(spell_dru_stampeding_roar);
    RegisterAuraScript(aura_dru_solar_empowerment);
    RegisterAuraScript(aura_dru_lunar_empowerment);
    new spell_dru_starfall_dummy();
    new spell_dru_sunfire();
    new spell_dru_survival_instincts();
    new spell_dru_swift_flight_passive();
    new spell_dru_t3_6p_bonus();
    new spell_dru_wild_charge_moonkin();
    RegisterAuraScript(spell_dru_photosynthesis);
    RegisterAreaTriggerAI(at_dru_lunar_beam);
    RegisterAreaTriggerAI(at_dru_ursol_vortex);
    new spell_dru_t3_8p_bonus();
    new spell_dru_t4_2p_bonus();
    new spell_dru_incarnation_chosen_of_elune();
    new spell_dru_incarnation_king_of_the_jungle();
    new spell_dru_incarnation_guardian_of_ursoc();
    new spell_dru_t10_balance_4p_bonus();
    new spell_dru_t10_restoration_4p_bonus();
    new spell_dru_t10_restoration_4p_bonus_dummy();
    RegisterSpellScript(spell_dru_thrash);
    RegisterAuraScript(spell_dru_thrash_aura);
    new spell_dru_travel_form();
    new spell_dru_travel_form_dummy();
    RegisterSpellAndAuraScriptPair(spell_dru_tiger_dash, spell_dru_tiger_dash_aura);
    new spell_dru_wild_growth();
    // NPC Scripts
    RegisterCreatureAI(npc_dru_efflorescence);
}
