//
//  Created by Nextpeer development team.
//  Copyright (c) 2014 Nextpeer, Inc. All rights reserved.
//

#include "OpponentPositionHeightUpdate.h"

OpponentPositionHeightUpdate::OpponentPositionHeightUpdate() : MultiplayerMessage(MULTIPLAYER_MESSAGE_TYPE_OPPONENT_POS_HEIGHT_UPDATE)
{
}

OpponentPositionHeightUpdate* OpponentPositionHeightUpdate::createWithP2PData(const TournamentP2PData& tournamentP2PData)
{
    OpponentPositionHeightUpdate* msg = new OpponentPositionHeightUpdate();
    if (msg->extractDataFromByteVector(tournamentP2PData.message)) {
        msg->setDefaultFields(tournamentP2PData);
        msg->autorelease();
        
        return msg;
    }
    
    return NULL;
}

bool OpponentPositionHeightUpdate::extractDataFromByteVector(const vector<unsigned char>& byteVector)
{
    if (byteVector.size() < sizeof(OpponentPositionHeightUpdateMessageStruct)) {
        // This can't be a valid message
        return false;
    }
    
    OpponentPositionHeightUpdateMessageStruct* structPtr = (OpponentPositionHeightUpdateMessageStruct*)&byteVector[0];
    
    this->_worldPositionHeight = structPtr->worldPositionHeight;
    
    return true;
}

OpponentPositionHeightUpdate* OpponentPositionHeightUpdate::createWithHero(const Hero* hero)
{
    OpponentPositionHeightUpdate* update = new OpponentPositionHeightUpdate();
    update->_worldPositionHeight = hero->getWorldPositionHeight();
    
    update->autorelease();
    
    return update;
}

vector<unsigned char>& OpponentPositionHeightUpdate::toByteVector()
{
    OpponentPositionHeightUpdateMessageStruct messageStruct;
    
    messageStruct.header = this->getHeaderForDispatch();
    
    messageStruct.worldPositionHeight = this->_worldPositionHeight;
    
    static vector<unsigned char> byteVector = vector<unsigned char>(sizeof(messageStruct));
    memcpy(&byteVector[0], &messageStruct, sizeof(messageStruct));
    
    return byteVector;
}