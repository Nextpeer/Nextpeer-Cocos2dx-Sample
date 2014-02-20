//
//  Created by Nextpeer development team.
//  Copyright (c) 2014 Nextpeer, Inc. All rights reserved.
//

#ifndef __OpponentPositionHeightUpdate__
#define __OpponentPositionHeightUpdate__

#include "MultiplayerMessage.h"
#include "MessageParser.h"
#include "Hero.h"

using namespace nextpeer;


typedef struct __attribute__ ((__packed__)) {
    MultiplayerMessageHeader header;
    
    float worldPositionHeight;
    
} OpponentPositionHeightUpdateMessageStruct;

class OpponentPositionHeightUpdate : public MultiplayerMessage
{
protected:
    OpponentPositionHeightUpdate();
    bool extractDataFromByteVector(const vector<unsigned char>& byteVector);
    
public:
    // Creation
    static OpponentPositionHeightUpdate* createWithP2PData(const TournamentP2PData& tournamentP2PData);
    static OpponentPositionHeightUpdate* createWithHero(const Hero* hero);

    // Serialziation
    virtual vector<unsigned char>& toByteVector();
    
    CC_SYNTHESIZE_READONLY(float, _worldPositionHeight, WorldPositionHeight);
};

#endif /* defined(__OpponentPositionHeightUpdate__) */
