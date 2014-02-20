//
//  Created by Nextpeer development team.
//  Copyright (c) 2014 Nextpeer, Inc. All rights reserved.
//

#include "MultiplayerGameState.h"
#include "MessageParser.h"

#include "CCNextpeer.h"

#include "Hero.h"
#include "PlayerData.h"
#include "Opponent.h"

#include "OpponentIsReadyMessage.h"
#include "OpponentPositionHeightUpdate.h"

using namespace nextpeer;

// Public methods

MultiplayerGameState* MultiplayerGameState::create()
{
    MultiplayerGameState* state = new MultiplayerGameState();
    state->autorelease();
    
    return state;
}

void MultiplayerGameState::reset()
{
    if (_opponentsData -> count() > 0) {
        _opponentsData->removeAllObjects();
    }
    if (_opponentsSprite -> count() > 0) {
        _opponentsSprite->removeAllObjects();
    }
    
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

void MultiplayerGameState::listenForIncomingPackets()
{
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
                                                                  callfuncO_selector(MultiplayerGameState::incomingNextpeerDataPacket),
                                                                  NEXTPEER_NOTIFICATION_INCOMING_DATA_PACKET,
                                                                  NULL);
}

void MultiplayerGameState::addOpponent(PlayerData *opponent) {
    _opponentsData->setObject(opponent, opponent->getPlayerId());
}

CCArray* MultiplayerGameState::getAllOpponents()
{
    if (_opponentsSprite == NULL || _opponentsSprite->count() <= 0) return NULL;
    CCArray* opponents = CCArray::createWithCapacity(_opponentsSprite->count());

    CCDictElement* dictElement = NULL;
    CCDICT_FOREACH(_opponentsSprite, dictElement)
    {
        opponents->addObject((Opponent*)dictElement->getObject());
    }

    return opponents;
}

void MultiplayerGameState::dispatchUpdateForHero(Hero* hero)
{
    // Generate an opponent update and then send it out
    OpponentPositionHeightUpdate* msg = OpponentPositionHeightUpdate::createWithHero(hero);
    vector<unsigned char>& byteVector = msg->toByteVector();
    
    CCNextpeer::getInstance()->unreliablePushDataToOtherPlayers(&byteVector[0], byteVector.size());
}

void MultiplayerGameState::dispatchIsReadyForHero(Hero* hero)
{
    OpponentIsReadyMessage* msg = OpponentIsReadyMessage::createWithHero(hero);
    vector<unsigned char>& byteVector = msg->toByteVector();
    
    CCNextpeer::getInstance()->pushDataToOtherPlayers(&byteVector[0], byteVector.size());
}

void MultiplayerGameState::reportCurrentScore(unsigned int score)
{
    CCNextpeer::getInstance()->reportScoreForCurrentTournament(score);
}

// Private methods

MultiplayerGameState::MultiplayerGameState()
{
    _opponentsData = CCDictionary::create();
    _opponentsData->retain();
    _opponentsSprite = CCDictionary::create();
    _opponentsSprite->retain();
}

MultiplayerGameState::~MultiplayerGameState()
{
    this->reset();
    
    CC_SAFE_RELEASE_NULL(_opponentsData);
    CC_SAFE_RELEASE_NULL(_opponentsSprite);
}

void MultiplayerGameState::applyOpponentPositionHeightUpdate(OpponentPositionHeightUpdate *update)
{
    string id = update->getSenderId();
    Opponent* opponent = static_cast<Opponent*>(_opponentsSprite->objectForKey(id));
    
    if (!opponent) {
        // This is weird, we don't know this opponent. This update will be ignored
        return;
    }
    
    opponent->applyOpponentPositionHeightUpdate(update);
}

void MultiplayerGameState::applyOpponentIsReady(OpponentIsReadyMessage *ready)
{
    string opponentId = ready->getSenderId();
    if (!_opponentsData->objectForKey(opponentId)) {
        CCLog(">> Can't find opponent data for id %s", opponentId.c_str());
        return; // this shouldn't happen
    }
    if (_opponentsSprite->objectForKey(opponentId)) {
        CCLog(">> Found opponent sprite for id %s", opponentId.c_str());
        return; // this shouldn't happen either
    }
    
    PlayerData* playerData = static_cast<PlayerData*>(_opponentsData->objectForKey(opponentId));
    playerData->setProfileType(ready->getProfileType());
    
    Opponent *opponent = Opponent::create(playerData);
    _opponentsSprite->setObject(opponent, opponentId);
}

void MultiplayerGameState::incomingNextpeerDataPacket(CCObject *packet)
{
    if (!CCNextpeer::getInstance()->isCurrentlyInTournament()) return;
    
    TournamentP2PData* p2pData = (TournamentP2PData*)packet;
    
    MultiplayerMessage* message = MessageParser::getMessageForTournamentP2PMessage(*p2pData);
    
    // Must be an unknown message type
    if (!message) return;
    
    if (message->getType() == MULTIPLAYER_MESSAGE_TYPE_OPPONENT_POS_HEIGHT_UPDATE) {
        OpponentPositionHeightUpdate* update = (OpponentPositionHeightUpdate*)message;
        this->applyOpponentPositionHeightUpdate(update);
    }
}