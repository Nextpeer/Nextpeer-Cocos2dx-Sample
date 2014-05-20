//
//  Created by Nextpeer development team.
//  Copyright (c) 2014 Nextpeer, Inc. All rights reserved.
//

#ifndef __Hero__
#define __Hero__

#include "PlayerSpriteProfile.h"
#include "PlayerData.h"

class Hero : public Sprite
{
private:
    PlayerData *_playerData;
    GamePlayerProfile *_profile;
    
protected:
    ~Hero();
	Hero(GamePlayerProfile *profile, PlayerData *data);
    
public:
    
    static Hero* create();
    float getWorldPositionHeight() const;
    
    void showReadyAnimation();
    void showJumpAnimation();
    void showHurtAnimation();
    
    PlayerData* getPlayerData() const{
    	return _playerData;
	}
    
    GamePlayerProfile* getPlayerProfile() const{
    	return _profile;
	}
};

#endif /* defined(__Hero__) */
