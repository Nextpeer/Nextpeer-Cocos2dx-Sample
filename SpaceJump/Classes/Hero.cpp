//
//  Created by Nextpeer development team.
//  Copyright (c) 2014 Nextpeer, Inc. All rights reserved.
//

#include "Hero.h"
#include "CCNextpeer.h"

Hero::~Hero(){
	
	CC_SAFE_RELEASE_NULL(_profile);
	CC_SAFE_RELEASE_NULL(_playerData);
}

Hero::Hero(GamePlayerProfile *profile, PlayerData *data) {
    initWithSpriteFrameName(profile->spriteFrontName());
    
    _playerData = data;
    _playerData->retain();
    
    _profile = profile;
    _profile->retain();
    
	this->setAnchorPoint(Point(0.5f, 1.0f));
}

Hero* Hero::create()
{
    // Create the Hero player data
    string playerName = nextpeer::CCNextpeer::getInstance()->getCurrentPlayerName();
    string playerId = nextpeer::CCNextpeer::getInstance()->getCurrentPlayerId();
    
    PlayerData *heroData = PlayerData::create(playerId, playerName, false);
    heroData->setProfileType(CurrentPlayerGameProfile::getCurrentUserProfile());
    GamePlayerProfile *profile = GamePlayerProfileFactory::createProfileByTypeOrNull(heroData->getProfileType());
    
    Hero *hero = new Hero(profile, heroData);
    
	if (hero != NULL) {
		hero->autorelease();
        return hero;
	}
	CC_SAFE_DELETE(hero);
	return NULL;
}

void Hero::showReadyAnimation() {
    setDisplayFrame(CCSpriteFrameCache::getInstance()->getSpriteFrameByName(_profile->spriteFrontName()));
}

void Hero::showJumpAnimation() {
    setDisplayFrame(CCSpriteFrameCache::getInstance()->getSpriteFrameByName(_profile->spriteJumpName()));
}

void Hero::showHurtAnimation() {
    setDisplayFrame(CCSpriteFrameCache::getInstance()->getSpriteFrameByName(_profile->spriteHurtName()));
}

float Hero::getWorldPositionHeight() const {
    return 0;
}