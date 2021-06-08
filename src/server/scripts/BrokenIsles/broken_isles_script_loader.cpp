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

// This is where scripts' loading functions should be declared:
void AddSC_orderhall_warrior();
void AddSC_zone_mardum();
void AddSC_zone_vault_of_wardens();
void AddSC_valsharah();
void AddSC_suramar();
void AddSC_stormheim();
void AddSC_highmountain();
void AddSC_azsuna();
void AddSC_dalaran_legion();
void AddSC_broken_shore();
void AddSC_zone_argus();
void AddSC_zone_argus_antoran_wastes();
void AddSC_zone_argus_krokuun();
void AddSC_zone_argus_macaree();
void AddSC_legion_portal_at_scripts();

// Halls of Valor
void AddSC_instance_halls_of_valor();
void AddSC_halls_of_valor();
void AddSC_boss_odyn_hov();
void AddSC_boss_hyrja();
void AddSC_boss_hymdall();
void AddSC_boss_godking_skovald();
void AddSC_boss_fenryr();

// The name of this function should match:
// void Add${NameOfDirectory}Scripts()
void AddBrokenIslesScripts()
{
    AddSC_orderhall_warrior();
    AddSC_zone_mardum();
    AddSC_zone_vault_of_wardens();
    AddSC_valsharah();
    AddSC_suramar();
    AddSC_stormheim();
    AddSC_highmountain();
    AddSC_azsuna();
    AddSC_dalaran_legion();
    AddSC_broken_shore();
    AddSC_zone_argus();
    AddSC_zone_argus_antoran_wastes();
    AddSC_zone_argus_krokuun();
    AddSC_zone_argus_macaree();
    AddSC_legion_portal_at_scripts();

    // Halls of Valor
    AddSC_instance_halls_of_valor();
    AddSC_halls_of_valor();
    AddSC_boss_odyn_hov();
    AddSC_boss_hyrja();
    AddSC_boss_hymdall();
    AddSC_boss_godking_skovald();
    AddSC_boss_fenryr();
}
