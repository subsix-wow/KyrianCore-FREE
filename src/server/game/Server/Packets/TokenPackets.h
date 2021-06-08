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

#ifndef TokenPackets_h__
#define TokenPackets_h__

#include "Packet.h"
#include "PacketUtilities.h"

namespace WorldPackets
{
    namespace Token
    {
        class CommerceTokenGetLog final : public ClientPacket
        {
        public:
            CommerceTokenGetLog(WorldPacket&& packet) : ClientPacket(CMSG_COMMERCE_TOKEN_GET_LOG, std::move(packet)) { }

            void Read() override;

            uint32 UnkInt   = 0;
        };

        class CommerceTokenGetLogResponse final : public ServerPacket
        {
        public:
            CommerceTokenGetLogResponse() : ServerPacket(SMSG_COMMERCE_TOKEN_GET_LOG_RESPONSE, 12) { }

            WorldPacket const* Write() override;

            struct AuctionableTokenInfo
            {
                uint64 UnkInt1      = 0;
                Timestamp<> UnkInt2;
                int32 Owner         = 0;
                uint64 BuyoutPrice  = 0;
                uint32 DurationLeft = 0;
            };

            uint32 UnkInt           = 0; // send CMSG_UPDATE_WOW_TOKEN_AUCTIONABLE_LIST
            uint32 Result           = 0;
            std::vector<AuctionableTokenInfo> AuctionableTokenAuctionableList;
        };

        class CommerceTokenGetMarketPrice final : public ClientPacket
        {
        public:
            CommerceTokenGetMarketPrice(WorldPacket&& packet) : ClientPacket(CMSG_COMMERCE_TOKEN_GET_MARKET_PRICE, std::move(packet)) { }

            void Read() override;

            uint32 UnkInt = 0;
        };

        class CommerceTokenGetMarketPriceResponse final : public ServerPacket
        {
        public:
            CommerceTokenGetMarketPriceResponse() : ServerPacket(SMSG_COMMERCE_TOKEN_GET_MARKET_PRICE_RESPONSE, 20) { }

            WorldPacket const* Write() override;

            uint64 CurrentMarketPrice   = 0;
            uint32 UnkInt               = 0; // send CMSG_REQUEST_WOW_TOKEN_MARKET_PRICE
            uint32 Result               = 0;
            uint32 AuctionDuration      = 0; // preset auction duration enum
        };

        class ConsumableTokenCanVeteranBuy final : public ClientPacket
        {
        public:
            ConsumableTokenCanVeteranBuy(WorldPacket&& packet) : ClientPacket(CMSG_CONSUMABLE_TOKEN_CAN_VETERAN_BUY, std::move(packet)) { }

            void Read() override;

            uint32 UnkInt = 0;
        };

        class ConsumableTokenCanVeteranBuyResponse final : public ServerPacket
        {
        public:
            ConsumableTokenCanVeteranBuyResponse() : ServerPacket(SMSG_CONSUMABLE_TOKEN_CAN_VETERAN_BUY_RESPONSE, 8 + 4 + 4) { }

            WorldPacket const* Write() override;

            uint64 UnkLong = 0;
            uint32 UnkInt = 0;
            uint32 UnkInt2 = 0;
        };
    }
}

#endif // TokenPackets_h__
