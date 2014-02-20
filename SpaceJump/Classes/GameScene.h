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

class GameScene : public CCLayerColor
{
public:
    ~GameScene();
    
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init(MultiplayerGameState *gameState);
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene(MultiplayerGameState *gameState);
    
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
    
	virtual void registerWithTouchDispatcher(void);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    virtual void didAccelerate(CCAcceleration* pAccelerationValue);
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);

private:
    CCSize _screenSize;
    CCSpriteBatchNode * _charactersBatchNode;
    CCSpriteBatchNode * _gameBatchNode;
    MultiplayerGameState* _multiplayerGameState;
    CCLabelBMFont* _scoreLabel;
    Hero *_hero;
    CCPoint _heroPosition;
	ccVertex2F _heroVelocity;
	ccVertex2F _heroAcceleration;
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
    void menuCallbackEndGame(CCObject* pSender);
};

#endif /* defined(__GameScene__) */
