//
//  Created by Nextpeer development team.
//  Copyright (c) 2014 Nextpeer, Inc. All rights reserved.
//

#include "NextpeerDelegate.h"
#include "NextpeerNotifications.h"
#include "MultiplayerGameState.h"
#include "NextpeerPlayer.h"
#include "PlayerData.h"
#include "Rand.h"
#include "GameScene.h"
#include "MainMenuScene.h"

#include "cocos2d.h"
USING_NS_CC;

using namespace nextpeer;

NextpeerDelegate::NextpeerDelegate()
{
    _currentGameState = MultiplayerGameState::create();
    _currentGameState->retain();
}

NextpeerDelegate::~NextpeerDelegate()
{
    CC_SAFE_RELEASE_NULL(_currentGameState);
}

void NextpeerDelegate::registerForEvents()
{
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
                                                                  callfuncO_selector(NextpeerDelegate::nextpeerDidStartTournament),
                                                                  NEXTPEER_NOTIFICATION_TOURNAMENT_STARTED,
                                                                  NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
                                                                  callfuncO_selector(NextpeerDelegate::nextpeerDidEndTournament),
                                                                  NEXTPEER_NOTIFICATION_TOURNAMENT_ENDED,
                                                                  NULL);
}

void NextpeerDelegate::unhookEvents()
{
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

void NextpeerDelegate::nextpeerDidStartTournament(CCObject *startData)
{
    CCLog("Nextpeer - received tournament start signal");

    _currentGameState->reset();
    
    // Add opponents
    TournamentStartData* tournamentStartData = (TournamentStartData*)startData;
    CCObject* it = NULL;
    CCARRAY_FOREACH(tournamentStartData->players, it)
    {
        NextpeerPlayer *nextpeerPlayer = static_cast<NextpeerPlayer*>(it);
        
        _currentGameState->addOpponent(PlayerData::create(nextpeerPlayer->getPlayerId(), nextpeerPlayer->getPlayerName(), nextpeerPlayer->getIsRecording()));
    }
    
    // Seed the random number generator with the seed that the server gave us
    Rand::seed(tournamentStartData->tournamentRandomSeed);
    
    // create a scene. it's an autorelease object
    CCDirector::sharedDirector()->replaceScene(GameScene::scene(_currentGameState));
}

void NextpeerDelegate::nextpeerDidEndTournament()
{
    CCLog("Nextpeer - received tournament end signal");
    
    // create a scene. it's an autorelease object
    CCDirector::sharedDirector()->replaceScene(MainMenuScene::scene());
}