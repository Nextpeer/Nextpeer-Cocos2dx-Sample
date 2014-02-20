//
//  Created by Nextpeer development team.
//  Copyright (c) 2014 Nextpeer, Inc. All rights reserved.
//

#ifndef __OpponentIsReadyMessage__
#define __OpponentIsReadyMessage__

#include "MultiplayerMessage.h"
#include "MessageParser.h"
#include "PlayerSpriteProfile.h"
#include "Hero.h"

typedef struct __attribute__ ((__packed__)) {
    MultiplayerMessageHeader header;
    
    uint32_t playerProfileType;
    
} OpponentIsReadyMessageStruct;

class OpponentIsReadyMessage : public MultiplayerMessage
{
protected:
    OpponentIsReadyMessage();
    bool extractDataFromByteVector(const vector<unsigned char>& byteVector);
    
public:
    
    static OpponentIsReadyMessage* createWithP2PData(const TournamentP2PData& tournamentP2PData);
    static OpponentIsReadyMessage* createWithHero(const Hero* hero);
    
    virtual vector<unsigned char>& toByteVector();
    
    CC_SYNTHESIZE_READONLY(GamePlayerProfileType, mProfileType, ProfileType);
};

#endif /* defined(__OpponentIsReadyMessage__) */
