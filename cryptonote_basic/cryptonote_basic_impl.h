// Copyright (c) 2014-2019, The Monero Project
// 
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once

#include "cryptonote_basic.h"
#include "crypto/crypto.h"
#include "crypto/hash.h"


namespace cryptonote {
  class BlockAddedHook
  {
  public:
    virtual bool block_added(const block& block, const std::vector<transaction>& txs, struct checkpoint_t const *checkpoint) = 0;
  };

  class BlockchainDetachedHook
  {
  public:
    virtual void blockchain_detached(uint64_t height, bool by_pop_blocks) = 0;
  };

  class InitHook
  {
  public:
    virtual void init() = 0;
  };

  class ValidateMinerTxHook
  {
  public:
    virtual bool validate_miner_tx(cryptonote::block const &block, struct block_reward_parts const &reward_parts) const = 0;
  };

  class AltBlockAddedHook
  {
  public:
    virtual bool alt_block_added(const block &block, const std::vector<transaction>& txs, struct checkpoint_t const *checkpoint) = 0;
  };

#pragma pack(push, 1)
  struct public_address_outer_blob
  {
    uint8_t m_ver;
    account_public_address m_address;
    uint8_t check_sum;
  };
  struct public_integrated_address_outer_blob
  {
    uint8_t m_ver;
    account_public_address m_address;
    crypto::hash8 payment_id;
    uint8_t check_sum;
  };
#pragma pack (pop)

  inline std::string return_first_address(const std::string_view url, const std::vector<std::string> &addresses, bool dnssec_valid)
  {
    if (addresses.empty())
      return {};
    return addresses[0];
  }

  struct address_parse_info
  {
    account_public_address address;
    bool is_subaddress;
    bool has_payment_id;
    crypto::hash8 payment_id;

    std::string as_str(network_type nettype) const;

    KV_MAP_SERIALIZABLE
  };

  /************************************************************************/
  /* Cryptonote helper functions                                          */
  /************************************************************************/
  bool block_header_has_POS_components(block_header const &blk_header);
  bool block_has_POS_components(block const &blk);
  size_t get_min_block_weight(uint8_t version);
  size_t get_max_tx_size();
  uint64_t block_reward_unpenalized_formula_v7(uint64_t already_generated_coins, uint64_t height);
  uint64_t block_reward_unpenalized_formula_v8(uint64_t height);
  bool get_base_block_reward(size_t median_weight, size_t current_block_weight, uint64_t already_generated_coins, uint64_t &reward, uint64_t &reward_unpenalized, uint8_t version, uint64_t height);
  uint8_t get_account_address_checksum(const public_address_outer_blob& bl);
  uint8_t get_account_integrated_address_checksum(const public_integrated_address_outer_blob& bl);

  std::string get_account_address_as_str(
      network_type nettype
    , bool subaddress
    , const account_public_address& adr
    );

  std::string get_account_integrated_address_as_str(
      network_type nettype
    , const account_public_address& adr
    , const crypto::hash8& payment_id
    );

  inline std::string address_parse_info::as_str(network_type nettype) const
  {
    if (has_payment_id)
      return get_account_integrated_address_as_str(nettype, address, payment_id);
    else
      return get_account_address_as_str(nettype, is_subaddress, address);
  }

  bool get_account_address_from_str(
      address_parse_info& info
    , network_type nettype
    , const std::string_view str
    );
#ifndef BELDEX_CORE_CUSTOM
  bool get_account_address_from_str_or_url(
      address_parse_info& info
    , network_type nettype
    , const std::string_view str_or_url
    , std::function<std::string(const std::string_view, const std::vector<std::string>&, bool)> dns_confirm = return_first_address
    );
#endif // BELDEX_CORE_CUSTOM
  bool is_coinbase(const transaction& tx);

  bool operator ==(const cryptonote::transaction& a, const cryptonote::transaction& b);
  bool operator ==(const cryptonote::block& a, const cryptonote::block& b);
}
