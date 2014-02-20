//
//  Created by Nextpeer development team.
//  Copyright (c) 2014 Nextpeer, Inc. All rights reserved.
//

#ifndef __PlayerSpriteProfile__
#define __PlayerSpriteProfile__

#include "cocos2d.h"
USING_NS_CC;

typedef enum
{
    kGamePlayerProfileBlue = 0,
    kGamePlayerProfilePink,
    kGamePlayerProfileGreen, 
    kGamePlayerProfileBeige,
    kGamePlayerProfileYellow,
    GamePlayerProfileTypeMax // max sentinal
    
} GamePlayerProfileType;


class GamePlayerProfile : public CCObject {
public:
    virtual GamePlayerProfileType profileType() = 0;
    virtual const char* assetName() = 0;
    virtual const char* spriteHUDName() = 0;
    virtual const char* spriteFrontName() = 0;
    virtual const char* spriteJumpName() = 0;
    virtual const char* spriteHurtName() = 0;
};

class BluePlayerProfile : public GamePlayerProfile {
public:
    GamePlayerProfileType profileType();
    const char* assetName();
    const char* spriteHUDName();
    const char* spriteFrontName();
    const char* spriteJumpName();
    const char* spriteHurtName();
};

class PinkPlayerProfile : public GamePlayerProfile {
public:
    GamePlayerProfileType profileType();
    const char* assetName();
    const char* spriteHUDName();
    const char* spriteFrontName();
    const char* spriteJumpName();
    const char* spriteHurtName();
};

class GreenPlayerProfile : public GamePlayerProfile {
public:
    GamePlayerProfileType profileType();
    const char* assetName();
    const char* spriteHUDName();
    const char* spriteFrontName();
    const char* spriteJumpName();
    const char* spriteHurtName();
};

class BeigePlayerProfile : public GamePlayerProfile {
public:
    GamePlayerProfileType profileType();
    const char* assetName();
    const char* spriteHUDName();
    const char* spriteFrontName();
    const char* spriteJumpName();
    const char* spriteHurtName();
};

class YellowPlayerProfile : public GamePlayerProfile {
public:
    GamePlayerProfileType profileType();
    const char* assetName();
    const char* spriteHUDName();
    const char* spriteFrontName();
    const char* spriteJumpName();
    const char* spriteHurtName();
};

class GamePlayerProfileFactory {
public:
    static GamePlayerProfile* createProfileByTypeOrNull(GamePlayerProfileType profileType);
    static CCArray* allGamePlayerProfiles();
};

class CurrentPlayerGameProfile {
public:
    static GamePlayerProfileType getCurrentUserProfile();
    static void setCurrentUserProfile(GamePlayerProfileType profileType);
};

#endif /* defined(__PlayerSpriteProfile__) */


