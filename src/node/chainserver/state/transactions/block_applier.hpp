#pragma once
#include "crypto/address.hpp"
#include "../../transaction_ids.hpp"
class ChainDB;
class Headerchain;
class BodyView;
class BlockId;

namespace chainserver {
struct Preparation;
struct BlockApplier {
    BlockApplier(ChainDB& db, const Headerchain& hc, const std::set<TransactionId, ByPinHeight>& baseTxIds, bool fromStage)
        : preparer { db, hc, baseTxIds, {} }
        , db(db)
        , fromStage(fromStage)
    {
    }
    TransactionIds&& move_new_txids() { return std::move(preparer.newTxIds); };
    void apply_block(const BodyView& bv, NonzeroHeight height, BlockId blockId);

private: // private methods
    struct Preparer {
        const ChainDB& db; // preparer cannot modify db!
        const Headerchain& hc;
        const std::set<TransactionId, ByPinHeight>& baseTxIds;
        TransactionIds newTxIds;
        Preparation prepare(const BodyView& bv, const NonzeroHeight height) const;
    };

private: // private data
    Preparer preparer;
    ChainDB& db;
    bool fromStage;
};
}
