//
//  Created by Nextpeer development team.
//  Copyright (c) 2014 Nextpeer, Inc. All rights reserved.
//

#include "MultiplayerMessage.h"

MultiplayerMessage::MultiplayerMessage(MultiplayerMessageType type)
{
    mType = type;
    mProtocolVersion = PROTOCOL_VERSION;
}

MultiplayerMessageHeader MultiplayerMessage::getHeaderForDispatch()
{
    MultiplayerMessageHeader header;
    header.protocolVersion = mProtocolVersion;
    header.messageType = mType;
    
    return header;
}

void MultiplayerMessage::setDefaultFields(const TournamentP2PData& tournamentP2PData)
{
    this->setSenderId(tournamentP2PData.playerId);
    this->setSenderName(tournamentP2PData.playerName);
}