//
//  Created by Nextpeer development team.
//  Copyright (c) 2014 Nextpeer, Inc. All rights reserved.
//

#ifndef __MultiplayerMessage__
#define __MultiplayerMessage__

#include "cocos2d.h"
USING_NS_CC;

#include <vector>
#include <string>
using namespace std;

#include "MultiplayerProtocol.h"

#include "NextpeerNotifications.h"
using namespace nextpeer;


class MultiplayerMessage : public Ref
{
public:
    CC_SYNTHESIZE(MultiplayerMessageType, mType, Type);
    CC_SYNTHESIZE(int, mProtocolVersion, ProtocolVersion);
    CC_SYNTHESIZE(string, mSenderId, SenderId);
    CC_SYNTHESIZE(string, mSenderName, SenderName);
    
    MultiplayerMessage(MultiplayerMessageType type);
    
    virtual vector<unsigned char>& toByteVector() = 0;
    
protected:
    MultiplayerMessageHeader getHeaderForDispatch();
    virtual void setDefaultFields(const TournamentP2PData& tournamentP2PData);
};

#endif /* defined(__MultiplayerMessage__) */
