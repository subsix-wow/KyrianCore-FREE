/*
 * Copyright 2021 ShadowCore
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

#include "Bag.h"
#include "BattlePayPackets.h"
#include "BattlePayMgr.h"
#include "BattlePayData.h"
#include "DatabaseEnv.h"
#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "Random.h"
#include <sstream>

auto GetBagsFreeSlots = [](Player* player) -> uint32
{
    uint32 freeBagSlots = 0;
    for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++)
        if (auto bag = player->GetBagByPos(i))
            freeBagSlots += bag->GetFreeSlots();

    uint8 inventoryEnd = INVENTORY_SLOT_ITEM_START + player->GetInventorySlotCount();
    for (uint8 i = INVENTORY_SLOT_ITEM_START; i < inventoryEnd; i++)
        if (!player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
            ++freeBagSlots;

    return freeBagSlots;
};

void WorldSession::SendStartPurchaseResponse(WorldSession* session, Battlepay::Purchase const& purchase, Battlepay::Error const& result)
{
    WorldPackets::BattlePay::StartPurchaseResponse response;
    response.PurchaseID = purchase.PurchaseID;
    response.ClientToken = purchase.ClientToken;
    response.PurchaseResult = result;
    session->SendPacket(response.Write());
};

void WorldSession::SendPurchaseUpdate(WorldSession* session, Battlepay::Purchase const& purchase, uint32 result)
{
    WorldPackets::BattlePay::PurchaseUpdate packet;
    WorldPackets::BattlePay::BattlePayPurchase data;
    data.PurchaseID = purchase.PurchaseID;
    data.UnkLong = 0;
    data.UnkLong2 = 0;
    data.Status = purchase.Status;
    data.ResultCode = result;
    data.ProductID = purchase.ProductID;
    data.UnkInt = purchase.ServerToken;
    data.WalletName = session->GetBattlePayMgr()->GetDefaultWalletName();
    packet.Purchase.emplace_back(data);
    session->SendPacket(packet.Write());
};

void WorldSession::HandleGetPurchaseListQuery(WorldPackets::BattlePay::GetPurchaseListQuery& /*packet*/)
{
    WorldPackets::BattlePay::PurchaseListResponse packet; // @TODO
    SendPacket(packet.Write());
}

void WorldSession::HandleUpdateVasPurchaseStates(WorldPackets::BattlePay::UpdateVasPurchaseStates& /*packet*/)
{
    WorldPackets::BattlePay::EnumVasPurchaseStatesResponse response;
    response.Result = 0;
    SendPacket(response.Write());
}

void WorldSession::HandleBattlePayDistributionAssign(WorldPackets::BattlePay::DistributionAssignToTarget& packet)
{
    if (!GetBattlePayMgr()->IsAvailable())
        return;

    GetBattlePayMgr()->AssignDistributionToCharacter(packet.TargetCharacter, packet.DistributionID, packet.ProductID, packet.SpecializationID, packet.ChoiceID);
}

void WorldSession::HandleGetProductList(WorldPackets::BattlePay::GetProductList& /*packet*/)
{
    if (!GetBattlePayMgr()->IsAvailable())
        return;

    GetBattlePayMgr()->SendProductList();
    GetBattlePayMgr()->SendAccountCredits();
}

void WorldSession::SendMakePurchase(ObjectGuid targetCharacter, uint32 clientToken, uint32 productID, WorldSession* session)
{
    if (!session || !session->GetBattlePayMgr()->IsAvailable())
        return;

    auto mgr = session->GetBattlePayMgr();

    auto player = session->GetPlayer();
    if (!player)
        return;

    auto accountID = session->GetAccountId();

    Battlepay::Purchase purchase;
    purchase.ProductID = productID;
    purchase.ClientToken = clientToken;
    purchase.TargetCharacter = targetCharacter;
    purchase.Status = Battlepay::UpdateStatus::Loading;
    purchase.DistributionId = mgr->GenerateNewDistributionId();

    if (!sBattlePayDataStore->ProductExist(productID))
    {
        SendStartPurchaseResponse(session, purchase, Battlepay::Error::PurchaseDenied);
        return;
    }

    auto const& product = sBattlePayDataStore->GetProduct(purchase.ProductID);
    auto displayInfo = sBattlePayDataStore->GetDisplayInfo(product.DisplayInfoID);
    purchase.CurrentPrice = product.CurrentPriceFixedPoint;

    mgr->RegisterStartPurchase(purchase);

    auto accountCredits = player->GetBattlePayCredits();
    auto purchaseData = mgr->GetPurchase();
    if (!accountCredits)
    {
        SendStartPurchaseResponse(session, *purchaseData, Battlepay::Error::InsufficientBalance);
        return;
    }

    if (accountCredits < static_cast<int64>(purchaseData->CurrentPrice))
    {
        SendStartPurchaseResponse(session, *purchaseData, Battlepay::Error::InsufficientBalance);
        return;
    }

    if (!product.Items.empty())
    {
        if (product.Items.size() > GetBagsFreeSlots(player))
        {
            player->SendBattlePayMessage(11, displayInfo->Name1);
            SendStartPurchaseResponse(session, *purchaseData, Battlepay::Error::PurchaseDenied);
            return;
        }
    }

    for (auto itr : product.Items)
    {
        if (mgr->AlreadyOwnProduct(itr.ItemID))
        {
            player->SendBattlePayMessage(12, displayInfo->Name1);
            SendStartPurchaseResponse(session, *purchaseData, Battlepay::Error::PurchaseDenied);
            return;
        }
    }

    purchaseData->PurchaseID = mgr->GenerateNewPurchaseID();
    purchaseData->ServerToken = urand(0, 0xFFFFFFF);

    SendStartPurchaseResponse(session, *purchaseData, Battlepay::Error::Ok);
    SendPurchaseUpdate(session, *purchaseData, Battlepay::Error::Ok);

    WorldPackets::BattlePay::ConfirmPurchase confirmPurchase;
    confirmPurchase.PurchaseID = purchaseData->PurchaseID;
    confirmPurchase.ServerToken = purchaseData->ServerToken;
    session->SendPacket(confirmPurchase.Write());
};

void WorldSession::HandleBattlePayStartPurchase(WorldPackets::BattlePay::StartPurchase& packet)
{
    SendMakePurchase(packet.TargetCharacter, packet.ClientToken, packet.ProductID, this);
}

void WorldSession::HandleBattlePayConfirmPurchase(WorldPackets::BattlePay::ConfirmPurchaseResponse& packet)
{
    if (!GetBattlePayMgr()->IsAvailable())
        return;

    packet.ClientCurrentPriceFixedPoint /= Battlepay::g_CurrencyPrecision;

    auto purchase = GetBattlePayMgr()->GetPurchase();
    auto const& product = sBattlePayDataStore->GetProduct(purchase->ProductID);
    if (!purchase)
        return;

    if (purchase->Lock)
    {
        SendPurchaseUpdate(this, *purchase, Battlepay::Error::PurchaseDenied);
        return;
    }

    if (purchase->ServerToken != packet.ServerToken || !packet.ConfirmPurchase || purchase->CurrentPrice != packet.ClientCurrentPriceFixedPoint)
    {
        SendPurchaseUpdate(this, *purchase, Battlepay::Error::PurchaseDenied);
        return;
    }

    auto player = GetPlayer();
    auto accountBalance = player->GetBattlePayCredits();
    if (accountBalance < static_cast<int64>(purchase->CurrentPrice))
    {
        SendPurchaseUpdate(this, *purchase, Battlepay::Error::PurchaseDenied);
        return;
    }

    if (product.WebsiteType == Battlepay::BattlePet && player->HasSpell(product.CustomValue))
    {
        SendPurchaseUpdate(this, *purchase, Battlepay::Error::TooManyTokens);
        return;
    }

    purchase->Lock = true;
    purchase->Status = Battlepay::UpdateStatus::Finish;

    auto displayInfo = sBattlePayDataStore->GetDisplayInfo(product.DisplayInfoID);

    if (!product.Items.empty())
    {
        if (product.Items.size() > GetBagsFreeSlots(player))
        {
            player->SendBattlePayMessage(11, displayInfo->Name1);
            SendStartPurchaseResponse(this, *purchase, Battlepay::Error::PurchaseDenied);
            return;
        }
    }

    for (auto itr : product.Items)
    {
        if (GetBattlePayMgr()->AlreadyOwnProduct(itr.ItemID))
        {
            player->SendBattlePayMessage(12, displayInfo->Name1);
            SendStartPurchaseResponse(this, *purchase, Battlepay::Error::PurchaseDenied);
            return;
        }
    }

    SendPurchaseUpdate(this, *purchase, Battlepay::Error::Other);

    GetBattlePayMgr()->SavePurchase(purchase);
    GetBattlePayMgr()->ProcessDelivery(purchase);

    player->UpdateBattlePayCredits(purchase->CurrentPrice);
    player->SendBattlePayMessage(1, displayInfo->Name1);
    GetBattlePayMgr()->SendProductList();
}

void WorldSession::HandleBattlePayAckFailedResponse(WorldPackets::BattlePay::BattlePayAckFailedResponse& /*packet*/)
{
}

void WorldSession::SendDisplayPromo(int32 promotionID /*= 0*/)
{
    SendPacket(WorldPackets::BattlePay::DisplayPromotion(promotionID).Write());

    if (!GetBattlePayMgr()->IsAvailable())
        return;

    auto player = GetPlayer();
    auto const& product = sBattlePayDataStore->GetProduct(109);
    WorldPackets::BattlePay::DistributionListResponse packet;
    packet.Result = Battlepay::Error::Ok;

    WorldPackets::BattlePay::BattlePayDistributionObject data;
    data.TargetPlayer;
    data.DistributionID = GetBattlePayMgr()->GenerateNewDistributionId();
    data.PurchaseID = GetBattlePayMgr()->GenerateNewPurchaseID();
    data.Status = Battlepay::DistributionStatus::BATTLE_PAY_DIST_STATUS_AVAILABLE;
    data.ProductID = 109;
    data.TargetVirtualRealm = 0;
    data.TargetNativeRealm = 0;
    data.Revoked = false;

    WorldPackets::BattlePay::BattlePayProduct pProduct;
    pProduct.ProductID = product.ProductID;
    pProduct.Flags = product.Flags;
    pProduct.Type = product.Type;

    auto dataP = GetBattlePayMgr()->WriteDisplayInfo(product.DisplayInfoID, GetSessionDbLocaleIndex());
    if (std::get<0>(dataP))
    {
        pProduct.DisplayInfo = boost::in_place();
        pProduct.DisplayInfo = std::get<1>(dataP);
    }

    data.Product = boost::in_place();
    data.Product = pProduct;

    packet.DistributionObject.emplace_back(data);

    SendPacket(packet.Write());
}

void WorldSession::SendSyncWowEntitlements()
{
    WorldPackets::BattlePay::SyncWowEntitlements packet;
    SendPacket(packet.Write());
}

