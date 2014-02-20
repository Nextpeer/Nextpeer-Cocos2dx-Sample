//
//  Created by Nextpeer development team.
//  Copyright (c) 2014 Nextpeer, Inc. All rights reserved.
//

#include "OpponentIsReadyMessage.h"

OpponentIsReadyMessage::OpponentIsReadyMessage() : MultiplayerMessage(MULTIPLAYER_MESSAGE_TYPE_OPPONENT_IS_READY)
{
}

OpponentIsReadyMessage* OpponentIsReadyMessage::createWithP2PData(const nextpeer::TournamentP2PData &tournamentP2PData)
{
    OpponentIsReadyMessage* msg = new OpponentIsReadyMessage();
    if (msg->extractDataFromByteVector(tournamentP2PData.message)) {
        msg->setDefaultFields(tournamentP2PData);
        msg->autorelease();
        
        return msg;
    }
    
    return NULL;
}

OpponentIsReadyMessage* OpponentIsReadyMessage::createWithHero(const Hero* hero)
{
    OpponentIsReadyMessage* msg = new OpponentIsReadyMessage();
    msg->mProfileType = hero->getPlayerData()->getProfileType();
    msg->autorelease();
    
    return msg;
}

vector<unsigned char>& OpponentIsReadyMessage::toByteVector()
{    
    OpponentIsReadyMessageStruct messageStruct;
    
    messageStruct.header = this->getHeaderForDispatch();
    messageStruct.playerProfileType = this->mProfileType;
    static vector<unsigned char> byteVector = vector<unsigned char>(sizeof(messageStruct));
    memcpy(&byteVector[0], &messageStruct, sizeof(messageStruct));
    
    return byteVector;
}

bool OpponentIsReadyMessage::extractDataFromByteVector(const vector<unsigned char>& byteVector)
{
    if (byteVector.size() < sizeof(OpponentIsReadyMessageStruct)) {
        // This can't be a valid message
        return false;
    }
    
    OpponentIsReadyMessageStruct* structPtr = (OpponentIsReadyMessageStruct*)&byteVector[0];
    
    this->mProfileType = (GamePlayerProfileType)structPtr->playerProfileType;
    
    return true;
}
