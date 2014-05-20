//
//  Created by Nextpeer development team.
//  Copyright (c) 2014 Nextpeer, Inc. All rights reserved.
//

#ifndef __Opponent__
#define __Opponent__

#include "cocos2d.h"
USING_NS_CC;

#include "PlayerSpriteProfile.h"
#include "PlayerData.h"
#include "OpponentPositionHeightUpdate.h"

class Opponent : public Ref
{
private:
    PlayerData *_playerData;
    GamePlayerProfile *_profile;
    float _lastKnownWorldPostion;
protected:
    ~Opponent();
    Opponent(GamePlayerProfile *profile, PlayerData *data);
    
public:
    static Opponent* create(PlayerData *data);
    
    void applyOpponentPositionHeightUpdate(OpponentPositionHeightUpdate* update);
    
    PlayerData* getPlayerData() const{
    	return _playerData;
	}
    
    GamePlayerProfile* getPlayerProfile() const{
    	return _profile;
	}
    
    float getWorldPositionHeight() const{
    	return _lastKnownWorldPostion;
	}
};


#endif /* defined(__Opponent__) */