//
//  Created by Nextpeer development team.
//  Copyright (c) 2014 Nextpeer, Inc. All rights reserved.
//

#ifndef __GameScene__
#define __GameScene__

#include "cocos2d.h"
USING_NS_CC;

class MultiplayerGameState;
class Hero;
class PlayerData;
class GamePlayerProfile;

class GameScene : public LayerColor
{
public:
    ~GameScene();
    
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init(MultiplayerGameState *gameState);
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::Scene* createScene(MultiplayerGameState *gameState);
    
    static GameScene* create(MultiplayerGameState* gameState)
    {
        GameScene *pRet = new GameScene();
        if (pRet && pRet->init(gameState)) {
            pRet->autorelease();
            return pRet;
        }
        else {
            delete pRet;
            pRet = NULL;
            return NULL;
        }
    }
    
    virtual void onEnter() override;
    virtual bool onTouchBegan(Touch* touch, Event* unused_event);
    virtual void onTouchEnded(Touch *touch, Event* unused_event);
    virtual void onAcceleration(Acceleration* acc, Event* unused_event);

private:
    Size _screenSize;
    SpriteBatchNode * _charactersBatchNode;
    SpriteBatchNode * _gameBatchNode;
    MultiplayerGameState* _multiplayerGameState;
    Label* _scoreLabel;
    Hero *_hero;
    Point _heroPosition;
	Vertex2F _heroVelocity;
	Vertex2F _heroAcceleration;
    Label *_waitForPlayersLabel;
	bool _heroLookingRight;

	float _currentCloudY;
    int _currentCloudTag;
	float _currentPlatformY;
	int _currentPlatformTag;
	float _currentMaxPlatformStep;
	int _currentBonusPlatformIndex;
	int _platformCount;
	int _score;
	
    unsigned int _multiplayerUpdateSentCounter;
    
    void createScreen();
    void startGame();
    void update (float dt);
    void updateScore();
	void initClouds();
	void initCloud();
	void resetClouds();
	void resetCloud();
	void initPlatforms();
	void initPlatform();
	void resetPlatforms();
	void resetPlatform();
	void resetHero();
	void resetBonus();
	void jump();
    void menuCallbackEndGame(Ref* pSender);
    void nextpeerDidReceiveSynchronizedEvent(Ref* eventName);
};

#endif /* defined(__GameScene__) */
