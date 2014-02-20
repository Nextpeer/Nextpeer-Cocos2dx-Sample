//
//  Created by Nextpeer development team.
//  Copyright (c) 2014 Nextpeer, Inc. All rights reserved.
//

#ifndef __MessageParser__
#define __MessageParser__

#include "MultiplayerMessage.h"
#include "MultiplayerProtocol.h"
#include "NextpeerNotifications.h"
using namespace nextpeer;

class MessageParser
{
public:
    static MultiplayerMessage* getMessageForTournamentP2PMessage(const TournamentP2PData& tournamentP2PData);
protected:
    static MultiplayerMessageType getMessageTypeForByteVector(const vector<unsigned char>& byteVector);
};

#endif /* defined(__MessageParser__) */
