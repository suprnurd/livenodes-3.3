// Copyright (c) 2014-2016 The Dash developers
// Copyright (c) 2016-2018 The PIVX developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SPORK_H
#define SPORK_H

#include "base58.h"
#include "key.h"
#include "main.h"
#include "net.h"
#include "sync.h"
#include "util.h"
#include "hashblock.h"

#include "obfuscation.h"
#include "protocol.h"

using namespace std;
using namespace boost;

/*
    Don't ever reuse these IDs for other sporks
    - This would result in old clients getting confused about which spork is for what

    Sporks 11,12, and 16 to be removed with 1st zerocoin release
*/
#define SPORK_START 10001
#define SPORK_END 10008

#define SPORK_1_SWIFTTX 10001
#define SPORK_2_SWIFTTX_BLOCK_FILTERING 10002
#define SPORK_3_MAX_VALUE 10003
#define SPORK_4_MASTERNODE_PAYMENT_ENFORCEMENT 10004
#define SPORK_5_RECONSIDER_BLOCKS 10005
#define SPORK_6_MN_WINNER_MINIMUM_AGE 10006
#define SPORK_7_MN_REBROADCAST_ENFORCEMENT 10007
#define SPORK_8_NEW_PROTOCOL_ENFORCEMENT 10008

#define SPORK_1_SWIFTTX_DEFAULT 978307200                         //2001-1-1
#define SPORK_2_SWIFTTX_BLOCK_FILTERING_DEFAULT 1424217600        //2015-2-18
#define SPORK_3_MAX_VALUE_DEFAULT 1000                            //1000 LivenodesCoin
#define SPORK_4_MASTERNODE_PAYMENT_ENFORCEMENT_DEFAULT 4529244393 //OFF at start to prevent nodes autoban
#define SPORK_5_RECONSIDER_BLOCKS_DEFAULT 0
#define SPORK_6_MN_WINNER_MINIMUM_AGE_DEFAULT 8000               // Age in seconds. This should be > MASTERNODE_REMOVAL_SECONDS to avoid

//#define SPORK_11_LOCK_INVALID_UTXO_DEFAULT 4070908800             //OFF - NOTE: this is block height not time!
#define SPORK_7_MN_REBROADCAST_ENFORCEMENT_DEFAULT 4529244393
#define SPORK_8_NEW_PROTOCOL_ENFORCEMENT_DEFAULT 4529244393

class CSporkMessage;
class CSporkManager;

extern std::map<uint256, CSporkMessage> mapSporks;
extern std::map<int, CSporkMessage> mapSporksActive;
extern CSporkManager sporkManager;

void ProcessSpork(CNode* pfrom, std::string& strCommand, CDataStream& vRecv);
int64_t GetSporkValue(int nSporkID);
bool IsSporkActive(int nSporkID);
void ExecuteSpork(int nSporkID, int nValue);
void ReprocessBlocks(int nBlocks);

//
// Spork Class
// Keeps track of all of the network spork settings
//

class CSporkMessage
{
public:
    std::vector<unsigned char> vchSig;
    int nSporkID;
    int64_t nValue;
    int64_t nTimeSigned;

    uint256 GetHash()
    {
        uint256 n = Hash9(BEGIN(nSporkID), END(nTimeSigned));
        return n;
    }

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion)
    {
        READWRITE(nSporkID);
        READWRITE(nValue);
        READWRITE(nTimeSigned);
        READWRITE(vchSig);
    }
};


class CSporkManager
{
private:
    std::vector<unsigned char> vchSig;
    std::string strMasterPrivKey;

public:
    CSporkManager()
    {
    }

    std::string GetSporkNameByID(int id);
    int GetSporkIDByName(std::string strName);
    bool UpdateSpork(int nSporkID, int64_t nValue);
    bool SetPrivKey(std::string strPrivKey);
    bool CheckSignature(CSporkMessage& spork);
    bool Sign(CSporkMessage& spork);
    void Relay(CSporkMessage& msg);
};

#endif
