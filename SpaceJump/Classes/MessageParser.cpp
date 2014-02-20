//
//  Created by Nextpeer development team.
//  Copyright (c) 2014 Nextpeer, Inc. All rights reserved.
//

#include "MessageParser.h"
#include "OpponentPositionHeightUpdate.h"
#include "OpponentIsReadyMessage.h"

MultiplayerMessageType MessageParser::getMessageTypeForByteVector(const vector<unsigned char>& byteVector)
{
    if (byteVector.size() < sizeof(MultiplayerMessageHeader)) return MULTIPLAYER_MESSAGE_TYPE_UNKNOWN;
    
    MultiplayerMessageHeader* header = (MultiplayerMessageHeader*)&byteVector[0];
    
    // For now, this is the only messages we know
    if (header->messageType != MULTIPLAYER_MESSAGE_TYPE_OPPONENT_POS_HEIGHT_UPDATE &&
        header->messageType != MULTIPLAYER_MESSAGE_TYPE_OPPONENT_IS_READY)
    {
        return MULTIPLAYER_MESSAGE_TYPE_UNKNOWN;
    }
    
    // If this message comes from a newer client, then we can't really parse it
    if (header->protocolVersion > PROTOCOL_VERSION) {
        return MULTIPLAYER_MESSAGE_TYPE_UNKNOWN;
    }
    
    return (MultiplayerMessageType)header->messageType;
}

MultiplayerMessage* MessageParser::getMessageForTournamentP2PMessage(const nextpeer::TournamentP2PData &tournamentP2PData)
{
    MultiplayerMessageType type = getMessageTypeForByteVector(tournamentP2PData.message);
    
    if (type == MULTIPLAYER_MESSAGE_TYPE_UNKNOWN) return NULL; // unknown message
    
    switch (type) {
        case MULTIPLAYER_MESSAGE_TYPE_OPPONENT_POS_HEIGHT_UPDATE:
            return OpponentPositionHeightUpdate::createWithP2PData(tournamentP2PData);
        case MULTIPLAYER_MESSAGE_TYPE_OPPONENT_IS_READY:
            return OpponentIsReadyMessage::createWithP2PData(tournamentP2PData);
            
        default:
            return NULL; // Unknown message
    }
}