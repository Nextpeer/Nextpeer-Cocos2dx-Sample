//
//  Created by Nextpeer development team.
//  Copyright (c) 2014 Nextpeer, Inc. All rights reserved.
//

#include "GameScene.h"
#include "MainMenuScene.h"
#include "CCNextpeer.h"
#include "MultiplayerGameState.h"
#include "Rand.h"
#include "Hero.h"
#include "Opponent.h"
#include "PlayerData.h"
#include "PlayerHUDIcon.h"
#include "Globals.h"
#include "Rand.h"
#include "PlayerSpriteProfile.h"

enum
{
	kCloudsStartTag = 100,
	kPlatformsStartTag = 200,
	kBonusStartTag = 300
};

#define kNumClouds			12

// First platform is 30 pixels from the bottom of the screen
#define kStartingCurrentPlatformY	30
#define kMinPlatformStep	50
#define kMaxPlatformStep	300		// This is the top of the screen (SHOULD BE RESOLUTION INDEPENDANT)
#define kNumPlatforms		10		// Number of platforms
#define kPlatformTopPadding 0
#define kMinBonusStep		20
#define kMaxBonusStep		40
#define HUD_ITEMS_SPACING 10.0f

CCScene* GameScene::scene(MultiplayerGameState *gameState)
{
	CCScene * scene = NULL;
	do
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);
        
		// 'layer' is an autorelease object
        GameScene *layer = GameScene::create(gameState);
		CC_BREAK_IF(! layer);
        
		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);
    
	// return the scene
	return scene;
}

GameScene::~GameScene()
{
    CC_SAFE_RELEASE_NULL(_hero);
    CC_SAFE_RELEASE_NULL(_multiplayerGameState);
}

// on "init" you need to initialize your instance
bool GameScene::init(MultiplayerGameState *gameState)
{
	bool bRet = false;
	do
	{
        //////////////////////////////
        // 1. super init first
        
        CC_BREAK_IF(! CCLayerColor::initWithColor(ccc4(176.0, 226.0, 255.0, 200.0)));
        
        _multiplayerGameState = gameState;
        _multiplayerGameState->retain();
        _multiplayerGameState->listenForIncomingPackets();
        
        createScreen();
        
		bRet = true;
	} while (0);
    
	return bRet;
}

void GameScene::createScreen () {
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ingame.plist");
    _gameBatchNode = CCSpriteBatchNode::create("ingame.png", 200);
    this->addChild(_gameBatchNode);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("characters.plist");
    _charactersBatchNode = CCSpriteBatchNode::create("characters.png", 200);
    this->addChild(_charactersBatchNode);
    
    //get screen size
    _screenSize = CCDirector::sharedDirector()->getWinSize();
    _hero = Hero::create();
    _hero->retain();
    _charactersBatchNode->addChild(_hero);
    
    // Initialze the clouds
    initClouds();
    
    // Initialize the platforms
    initPlatforms();
    
    // Create the bonus sprites
    CCSprite *bonus = CCSprite::createWithSpriteFrameName("power_up_box.png");
    bonus->setTag(kBonusStartTag);
    _gameBatchNode->addChild(bonus);
    bonus->setVisible(false);
    
    // Create the Score Label
    _scoreLabel = CCLabelBMFont::create("0",  "font.fnt");
    this->addChild(_scoreLabel);
    
    // Center the label
    _scoreLabel->setPosition(ccp(_screenSize.width/2, 30));
    
    CCNode* hudX = CCSprite::createWithSpriteFrameName("hud_x.png");
    CCMenuItemSprite* endGame = CCMenuItemSprite::create(hudX, hudX, this, menu_selector(GameScene::menuCallbackEndGame));
    CCMenu *leaveGameHudMenu = CCMenu::create(endGame, NULL);
    leaveGameHudMenu->setPosition(ccp(_screenSize.width - hudX->getContentSize().width/2 - HUD_ITEMS_SPACING, _screenSize.height - hudX->getContentSize().height/2 - HUD_ITEMS_SPACING));
    addChild(leaveGameHudMenu);
    
    //create main loop
    this->schedule(schedule_selector(GameScene::update));
    
    // Enable the touch events
    setTouchEnabled(true);
    // Enable accelerometer events
    setAccelerometerEnabled(true);
    
    // Let the other clients know that this player is in the game
    _multiplayerGameState->dispatchIsReadyForHero(_hero);
    
    // Start the game
    startGame();
}

void GameScene::update(float dt)
{
	// Update the player x position based on velocity and delta time
	_heroPosition.x += _heroVelocity.x * dt;
	
	// Flip the player based on it's x velocity and current looking direction
	if(_heroVelocity.x < -30.0f && _heroLookingRight)
	{
		_heroLookingRight = false;
		_hero->setScaleX(-1.0f);
	}
	else if (_heroVelocity.x > 30.0f && !_heroLookingRight)
	{
		_heroLookingRight = true;
		_hero->setScaleX(1.0f);
	}
    
	// Calculate the max and min x values for the hero
	// based on the screen and hero widths
	CCSize hero_size = _hero->getContentSize();
	float max_x = (float)_screenSize.width - hero_size.width/2;
	float min_x = hero_size.width/2;
	
	// Limit the hero position based on max and min values allowed
	if(_heroPosition.x>max_x) _heroPosition.x = max_x;
	if(_heroPosition.x<min_x) _heroPosition.x = min_x;
    
	// Update the hero velocity based on acceleration and time
	_heroVelocity.y += _heroAcceleration.y * dt;
    
	// Update the hero y position based on velocity and time
	_heroPosition.y += _heroVelocity.y * dt;
	
	////////////////////////////////////////////////////////////////////////////
	// Handle the bonus scoring
	CCSprite *bonus = (CCSprite*)_gameBatchNode->getChildByTag(kBonusStartTag);
    
	// If bonus is visible then see if the hero is within range to get the bonus
	if(bonus->isVisible())
	{
		// If the player is within range of the bonus value then award the prize
        if(_hero->boundingBox().intersectsRect(bonus->boundingBox()))
		{
			// Update score based on bonus
            _score += 5000;
            
			// Build the score string to display
            updateScore();
            
			// Highlight the score with some actions to celebrate the bonus win
			CCActionInterval* a1 = CCScaleTo::create(0.2f, 1.5f, 0.8f);
			CCActionInterval* a2 = CCScaleTo::create(0.2f, 1.0f, 1.0f);
			_scoreLabel->runAction(CCSequence::create(a1, a2, a1, a2, a1, a2, NULL));
            
			// Reset the bonus to another platform
			resetBonus();
		}
	}
    
	// If the hero has stopped moving then make it jump from the platform it is on
	if(_heroVelocity.y < 0)
	{
		// Search through all the platforms and compare the hero's position with the platfor position
		for(int t = kPlatformsStartTag; t < kPlatformsStartTag + kNumPlatforms; t++)
		{
			CCSprite *platform = (CCSprite*)_gameBatchNode->getChildByTag(t);
            
			if(_hero->boundingBox().intersectsRect(platform->boundingBox()))
			{
				jump();
				break;	// Can only jump from one platform at a time to break out of the loop
			}
		}
        
		// If the hero has fallen below the screen then game over
		if(_heroPosition.y < - hero_size.height/2)
		{
            menuCallbackEndGame(this);
		}
	}
	else if ( _heroPosition.y > ((float)_screenSize.height / 2))
	{
		// If hero position is greater than the middle of the screen then move the platforms
		// the difference between the hero y position and middle point of the screen
		float delta = _heroPosition.y - ((float)_screenSize.height / 2);
        
		// Set the hero y position to the middle of the screen
		_heroPosition.y = (float)_screenSize.height / 2;
        
		// Move the current platform y by the delta amount
		_currentPlatformY -= delta;
        
		// Move the clouds vertically and reset if necessary
		for (int t = kCloudsStartTag; t < kCloudsStartTag + kNumClouds; t++)
		{
			CCSprite *cloud = (CCSprite*)_gameBatchNode->getChildByTag(t);
            
			CCPoint pos = cloud->getPosition();
            
			// Calculate new position for cloud
			pos.y -= delta * cloud->getScaleY() * 0.8f;
            
			// If the cloud is off the screen then need to reset this cloud else set its new position
			if (pos.y < -cloud->getContentSize().height/2)
			{
				_currentCloudTag = t;
				resetCloud();
			}
			else
			{	// Update the new y position for the cloud.
				cloud->setPosition(pos);
			}
		}
        
		// Move the platforms vertically and reset if necessary
		for (int t = kPlatformsStartTag; t < kPlatformsStartTag + kNumPlatforms; t++)
		{
			CCSprite *platform = (CCSprite*)_gameBatchNode->getChildByTag(t);
			
			CCPoint pos = platform->getPosition();
            
			// Calculate new position for platform
			pos = ccp(pos.x, pos.y - delta);
            
			// If the platform is off the screen then reset the platform else set its new position
			if(pos.y < - platform->getContentSize().height/2)
			{
				_currentPlatformTag = t;
				resetPlatform();
			}
			else
			{
				platform->setPosition(pos);
			}
		}
        
		// If the bonus is visible then adjust it's y position
		if(bonus->isVisible())
		{
			CCPoint pos = bonus->getPosition();
            
			// Calculate new position of bonus
			pos.y -= delta;
			
			// If the bonus is off the screen then reset the bonus else set its new position
			if(pos.y < -bonus->getContentSize().height/2 )
			{
				resetBonus();
			}
			else
			{
				bonus->setPosition(pos);
			}
		}
		
		// Update score based on how much the hero has moved
		_score += (int)delta;
        
		// Display the new score value
		updateScore();
	}
    
	// Set the hero's position
	_hero->setPosition(_heroPosition);
    
    // Update the other clients on the update for this player
    if (_multiplayerUpdateSentCounter++ % 2 == 0) {
        _multiplayerGameState->dispatchUpdateForHero(_hero);
    }
}

void GameScene::updateScore() {
    char scoreStr[10] = {0};
    sprintf(scoreStr, "%d", _score);
    _scoreLabel->setString(scoreStr);
}

void GameScene::menuCallbackEndGame(CCObject* pSender) {
    
    _hero->showHurtAnimation();
    
    if (CCNextpeer::getInstance()->isCurrentlyInTournament()) {
        CCNextpeer::getInstance()->reportControlledTournamentOverWithScore(_score);
    }
    else {
        CCNextpeer::getInstance()->reportForfeitForCurrentTournament();
    }
    
    // Switch back to the main menu
    CCDirector::sharedDirector()->replaceScene(MainMenuScene::scene());
}

// Initializes everything and then starts the game by setting the gameSuspend
void GameScene::startGame()
{
	_score = 0;
    _multiplayerUpdateSentCounter = 0;
	   
	resetClouds();
	resetPlatforms();
	resetHero();
	resetBonus();
}

// Initialize the platforms that the hero jumps on
void GameScene::initPlatforms()
{
	_currentPlatformTag = kPlatformsStartTag;
	while(_currentPlatformTag < kPlatformsStartTag + kNumPlatforms)
	{
		initPlatform();
		_currentPlatformTag++;
	}
	
	resetPlatforms();
}

// Init a random platform. Because we use Nextpeer's random function the cloud will be the same on all devices
void GameScene::initPlatform()
{
    char szName[100] = {0};
    sprintf(szName, PLATFORMS_FILENAME_FORMAT, (int)Rand::generate(0, PLATFORMS_NUMBER));
    CCSprite *platform = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(szName));
    _gameBatchNode->addChild(platform, 3, _currentPlatformTag);
}

// Create kNumClouds
void GameScene::initClouds() {
	_currentCloudTag = kCloudsStartTag;
    
	// Create the clouds and add them to the layer
	while(_currentCloudTag < kCloudsStartTag + kNumClouds)
	{
		initCloud();
		_currentCloudTag++;
	}
    
	// randomly size and place each cloud
	resetClouds();
}

// Init a random cloud. Because we use Nextpeer's random function the cloud will be the same on all devices
void GameScene::initCloud() {
    
    char szName[100] = {0};
    sprintf(szName, CLOUDS_FILENAME_FORMAT, (int)Rand::generate(0, CLOUDS_NUMBER));
    CCSprite *cloud = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(szName));
	_gameBatchNode->addChild(cloud, 3, _currentCloudTag);
	cloud->setOpacity(128);
}

// Loop through and reset the clouds on initialization
void GameScene::resetClouds() {
	_currentCloudTag = kCloudsStartTag;
	
	while(_currentCloudTag < kCloudsStartTag + kNumClouds)
	{
		resetCloud();
        
		CCSprite *cloud = (CCSprite*)_gameBatchNode->getChildByTag(_currentCloudTag);
		CCPoint pos = cloud->getPosition();
		pos.y -= (float)CCDirector::sharedDirector()->getWinSize().height;
		cloud->setPosition(pos);
		_currentCloudTag++;
	}
}

// For each cloud, randomly size and place them
void GameScene::resetCloud() {
	CCSprite *cloud = (CCSprite*)_gameBatchNode->getChildByTag(_currentCloudTag);
	
	// Calculate a random distance for this cloud
	float distance = Rand::generate() % 20 + 5;
	
	// Scale the cloud based on the distance
	float scale = 5.0f / distance;
    
	// Scale the X and Y equally for the cloud
	cloud->setScaleX(scale);
	cloud->setScaleY(scale);
    
	// randomly invert the X scale for some of the clouds
	if(Rand::generate() % 2 == 1) cloud->setScaleX(-cloud->getScaleX());
	
	CCSize size = cloud->getContentSize();
    
	float scaled_width = size.width * scale;
    
	// Randomly place each cloud within our view
	float x = Rand::generate() % (int)(CCDirector::sharedDirector()->getWinSize().width +  (int)scaled_width) - scaled_width/2;
	float y = Rand::generate() % (int)(CCDirector::sharedDirector()->getWinSize().height - (int)scaled_width) + scaled_width/2 + CCDirector::sharedDirector()->getWinSize().height;
    
	cloud->setPosition(ccp(x,y));
}


// Reset the platforms as they move off the screen
void GameScene::resetPlatforms()
{
	_currentPlatformY = -1;						// Set the current platform y to -1 so it gets initialized
	_currentPlatformTag = kPlatformsStartTag;	// set starting platform
	_currentMaxPlatformStep = 60.0f;				// maximum space between platforms
	_currentBonusPlatformIndex = 0;
	_platformCount = 0;
    
	while(_currentPlatformTag < kPlatformsStartTag + kNumPlatforms)
	{
		resetPlatform();
		_currentPlatformTag++;
	}
}


void GameScene::resetPlatform()
{
	// We set this to -1 to initialize the first platform y coordinate
	if(_currentPlatformY < 0)
	{
		_currentPlatformY = (float) kStartingCurrentPlatformY;
	}
	else
	{
		// If not the first one then randomly determine a y coordinate for the platform
		_currentPlatformY += Rand::generate() % (int)(_currentMaxPlatformStep - kMinPlatformStep) + kMinPlatformStep;
        
		// If the current platofmr step is less than the max (top of screen) then increment it
		if(_currentMaxPlatformStep < kMaxPlatformStep)
		{
			_currentMaxPlatformStep += 0.5f;
		}
	}
	
	CCSprite *platform = (CCSprite*)_gameBatchNode->getChildByTag(_currentPlatformTag);
    
	if ( Rand::generate() % 2 == 1) platform->setScaleX(-1.0f);
    
	float x;
	CCSize size = platform->getContentSize();
    
	// If the current platform is the first one initialized then just center it
	if (_currentPlatformY == (float)kStartingCurrentPlatformY)
	{
		x = (float) _screenSize.width/2;
	}
	else
	{
		x = Rand::generate() % (int)(_screenSize.width -(int)size.width) + size.width/2;
	}
	
	platform->setPosition(ccp(x, _currentPlatformY));
	_platformCount++;
    
	// If the platform is to have to bonus then put it there.
	if (_platformCount == _currentBonusPlatformIndex)
	{
		CCSprite *bonus = (CCSprite*)_gameBatchNode->getChildByTag(kBonusStartTag);
		bonus->setPosition(ccp(x, _currentPlatformY+platform->getContentSize().height/2 + bonus->getContentSize().height/2));
		bonus->setVisible(true);
	}
}

// Reset the hero to its home location
void GameScene::resetHero()
{
	// Place the hero in center
	_heroPosition.x = (float) CCDirector::sharedDirector()->getWinSize().width/2;
	_heroPosition.y = (float) CCDirector::sharedDirector()->getWinSize().width/2;
	_hero->setPosition(_heroPosition);
	
	_heroVelocity.x = 0;
	_heroVelocity.y = 0;
	
	_heroAcceleration.x = 0;
	_heroAcceleration.y = -450.0f;
	
	_heroLookingRight = true;
	_hero->setScaleX(1.0f);
}

// Reset the bonus types based on the current score
void GameScene::resetBonus()
{
	CCSprite *bonus = (CCSprite*)_gameBatchNode->getChildByTag(kBonusStartTag);
    
	// Set the bonus to not be visible
	bonus->setVisible(false);
    
	// Randomly determine which platform will get the bonus next by adding a random amount
	_currentBonusPlatformIndex += Rand::generate() % ( kMaxBonusStep - kMinBonusStep ) + kMinBonusStep;
}

/////////////////////////////////////////////////////////
// Update the player y velocity for jumping
// Base the y velocity on the x velocity
void GameScene::jump()
{
	_heroVelocity.y = 350.0f + fabsf(_heroVelocity.x);
    
    _hero->showJumpAnimation();
}

void GameScene::registerWithTouchDispatcher(void)
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority + 1, true);
}

bool GameScene::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	return true;
}

/////////////////////////////////////////////////////////
// Touch on left side of screen moves player left
// Touch on right side of screen moves player right
void GameScene::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	CCPoint touchLocation = touch->getLocationInView(  );
	touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
	touchLocation =  convertToNodeSpace(touchLocation);
    
	float touchCenter =  CCDirector::sharedDirector()->getWinSize().width/2 - touchLocation.x;
	float accel_filter = 0.1f;
	_heroVelocity.x = _heroVelocity.x * accel_filter - touchCenter;
}

/////////////////////////////////////////////////////////
// Acceleramoter routine to move the player object
void GameScene::didAccelerate(CCAcceleration* pAccelerationValue)
{
	float accel_filter = 0.1f;
	_heroVelocity.x = _heroVelocity.x * accel_filter + pAccelerationValue->x * (1.0f - accel_filter) * 500.0f;
}
