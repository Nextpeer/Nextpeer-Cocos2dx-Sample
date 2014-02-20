//
//  Created by Nextpeer development team.
//  Copyright (c) 2014 Nextpeer, Inc. All rights reserved.
//

#include "PlayerHUDIcon.h"

PlayerHUDIcon* PlayerHUDIcon::create(GamePlayerProfile* playerProfile) {
    
    PlayerHUDIcon *playerHUDIcon = new PlayerHUDIcon(playerProfile);

	if (playerHUDIcon) {
		playerHUDIcon->autorelease();
        return playerHUDIcon;
	}
	CC_SAFE_DELETE(playerHUDIcon);
	return NULL;
}

PlayerHUDIcon::PlayerHUDIcon(GamePlayerProfile* playerProfile) {
    initWithSpriteFrameName(playerProfile->spriteHUDName());
}