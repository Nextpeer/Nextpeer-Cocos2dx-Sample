//
//  Created by Nextpeer development team.
//  Copyright (c) 2014 Nextpeer, Inc. All rights reserved.
//

#include "Opponent.h"

Opponent::~Opponent(){
	
	CC_SAFE_RELEASE_NULL(_profile);
	CC_SAFE_RELEASE_NULL(_playerData);
}

Opponent::Opponent(GamePlayerProfile *profile, PlayerData *data) {
    _lastKnownWorldPostion = 0;
    
    _playerData = data;
    _playerData->retain();
    
    _profile = profile;
    _profile->retain();
}

Opponent* Opponent::create(PlayerData *data)
{
    GamePlayerProfile *profile = GamePlayerProfileFactory::createProfileByTypeOrNull(data->getProfileType());
    
    Opponent *opponent = new Opponent(profile, data);
    if (opponent != NULL) {
		opponent->autorelease();
        return opponent;
	}
	CC_SAFE_DELETE(opponent);
	return NULL;
}

void Opponent::applyOpponentPositionHeightUpdate(OpponentPositionHeightUpdate* update) {
    _lastKnownWorldPostion = update->getWorldPositionHeight();
}