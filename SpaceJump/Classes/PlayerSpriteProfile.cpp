//
//  Created by Nextpeer development team.
//  Copyright (c) 2014 Nextpeer, Inc. All rights reserved.
//

#include "PlayerSpriteProfile.h"

// BluePlayerProfile
GamePlayerProfileType BluePlayerProfile::profileType() {
    return kGamePlayerProfileBlue;
}

const char* BluePlayerProfile::assetName() {
    return "Blue";
}

const char* BluePlayerProfile::spriteHUDName() {
    return "hud_p2Alt.png";
}

const char* BluePlayerProfile::spriteFrontName() {
    return "p2_front.png";
}

const char* BluePlayerProfile::spriteJumpName() {
    return "p2_jump.png";
}

const char* BluePlayerProfile::spriteHurtName() {
    return "p2_hurt.png";
}

// PinkPlayerProfile
GamePlayerProfileType PinkPlayerProfile::profileType() {
    return kGamePlayerProfilePink;
}

const char* PinkPlayerProfile::assetName() {
    return "Pinky";
}

const char* PinkPlayerProfile::spriteHUDName() {
    return "hud_p3Alt.png";
}

const char* PinkPlayerProfile::spriteFrontName() {
    return "p3_front.png";
}

const char* PinkPlayerProfile::spriteJumpName() {
    return "p3_jump.png";
}

const char* PinkPlayerProfile::spriteHurtName() {
    return "p3_hurt.png";
}

// GreenPlayerProfile
GamePlayerProfileType GreenPlayerProfile::profileType() {
    return kGamePlayerProfileGreen;
}

const char* GreenPlayerProfile::assetName() {
    return "Greeny";
}

const char* GreenPlayerProfile::spriteHUDName() {
    return "hud_p1Alt.png";
}

const char* GreenPlayerProfile::spriteFrontName() {
    return "p1_front.png";
}

const char* GreenPlayerProfile::spriteJumpName() {
    return "p1_jump.png";
}

const char* GreenPlayerProfile::spriteHurtName() {
    return "p1_hurt.png";
}

// BeigePlayerProfile
GamePlayerProfileType BeigePlayerProfile::profileType() {
    return kGamePlayerProfileBeige;
}

const char* BeigePlayerProfile::assetName() {
    return "Dr. Beige";
}

const char* BeigePlayerProfile::spriteHUDName() {
    return "hud_p4Alt.png";
}

const char* BeigePlayerProfile::spriteFrontName() {
    return "p4_front.png";
}

const char* BeigePlayerProfile::spriteJumpName() {
    return "p4_jump.png";
}

const char* BeigePlayerProfile::spriteHurtName() {
    return "p4_hurt.png";
}

// YellowPlayerProfile
GamePlayerProfileType YellowPlayerProfile::profileType() {
    return kGamePlayerProfileYellow;
}

const char* YellowPlayerProfile::assetName() {
    return "YOLO";
}

const char* YellowPlayerProfile::spriteHUDName() {
    return "hud_p5Alt.png";
}

const char* YellowPlayerProfile::spriteFrontName() {
    return "p5_front.png";
}

const char* YellowPlayerProfile::spriteJumpName() {
    return "p5_jump.png";
}

const char* YellowPlayerProfile::spriteHurtName() {
    return "p5_hurt.png";
}

// GamePlayerProfileFactory
GamePlayerProfile* GamePlayerProfileFactory::createProfileByTypeOrNull(GamePlayerProfileType profileType) {
    GamePlayerProfile *profile = NULL;
    
    switch (profileType) {
        case kGamePlayerProfileGreen:
            profile = new GreenPlayerProfile();
            break;
        case kGamePlayerProfilePink:
            profile = new PinkPlayerProfile();
            break;
        case kGamePlayerProfileBlue:
            profile = new BluePlayerProfile();
            break;
        case kGamePlayerProfileBeige:
            profile = new BeigePlayerProfile();
            break;
        case kGamePlayerProfileYellow:
            profile = new YellowPlayerProfile();
            break;
            
        default:
            break;
    }
    
    if (profile != NULL) {
        profile->autorelease();
        return profile;
    }
    
    return NULL;
}

__Array* GamePlayerProfileFactory::allGamePlayerProfiles() {
    __Array* profiles = __Array::create();
    profiles->addObject(createProfileByTypeOrNull(kGamePlayerProfileGreen));
    profiles->addObject(createProfileByTypeOrNull(kGamePlayerProfilePink));
    profiles->addObject(createProfileByTypeOrNull(kGamePlayerProfileBlue));
    profiles->addObject(createProfileByTypeOrNull(kGamePlayerProfileBeige));
    profiles->addObject(createProfileByTypeOrNull(kGamePlayerProfileYellow));
    return profiles;
}

// CurrentPlayerGameProfile
#define KEY_OF_GAME_USER_PROFILE   "game-player-profile"
#define GAME_USER_PROFILE_NOT_FOUND   -999
GamePlayerProfileType CurrentPlayerGameProfile::getCurrentUserProfile() {
    
    int profileTypeAsInt = UserDefault::getInstance()->getIntegerForKey(KEY_OF_GAME_USER_PROFILE, GAME_USER_PROFILE_NOT_FOUND);
    
    GamePlayerProfileType profileType;
    
    // Profile was not found -> randomize one & persist it.
    if (profileTypeAsInt == GAME_USER_PROFILE_NOT_FOUND) {
        profileTypeAsInt = cocos2d::random(0, RAND_MAX)%GamePlayerProfileTypeMax;
        profileType = static_cast<GamePlayerProfileType>(profileTypeAsInt);
        setCurrentUserProfile(profileType);
    }
    else {
        profileType = static_cast<GamePlayerProfileType>(profileTypeAsInt);
    }
    
    return profileType;
}

void CurrentPlayerGameProfile::setCurrentUserProfile(GamePlayerProfileType profileType) {
    UserDefault *userDefaults = UserDefault::getInstance();
    userDefaults->setIntegerForKey(KEY_OF_GAME_USER_PROFILE, profileType);
    userDefaults->flush();
}
