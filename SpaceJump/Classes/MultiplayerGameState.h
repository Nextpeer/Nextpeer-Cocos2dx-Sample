//
//  Created by Nextpeer development team.
//  Copyright (c) 2014 Nextpeer, Inc. All rights reserved.
//

#ifndef __MultiplayerGameState__
#define __MultiplayerGameState__

#include "cocos2d.h"
USING_NS_CC;

class Hero;
class Opponent;
class PlayerData;
class OpponentPositionHeightUpdate;
class OpponentIsReadyMessage;

/* Flow:
 tournament start -> loading level -> loading opponents sprites -> letting the other opponents know i'm ready (waiting for their ready call backs)
 */
class MultiplayerGameState : public CCObject
{
public:
    static MultiplayerGameState* create();
    
    void reset();
    void listenForIncomingPackets();
    void addOpponent(PlayerData *opponent);
    CCArray* getAllOpponents();

    void dispatchUpdateForHero(Hero* hero);
    void dispatchIsReadyForHero(Hero* hero);
    
    void reportCurrentScore(unsigned int score);
    
private:
    MultiplayerGameState();
    virtual ~MultiplayerGameState();
    
    void applyOpponentPositionHeightUpdate(OpponentPositionHeightUpdate* update);
    void applyOpponentIsReady(OpponentIsReadyMessage *ready);
    
    void incomingNextpeerDataPacket(CCObject* packet);
    
    CCDictionary* _opponentsData; // The Opponent raw data (as we get it from Nextpeer)
    CCDictionary* _opponentsSprite; // Opponent sprites
 };


#endif /* defined(__MultiplayerGameState__) */
