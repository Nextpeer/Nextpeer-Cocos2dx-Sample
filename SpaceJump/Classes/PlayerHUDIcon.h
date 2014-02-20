//
//  Created by Nextpeer development team.
//  Copyright (c) 2014 Nextpeer, Inc. All rights reserved.
//

#ifndef __PlayerHUDIcon__
#define __PlayerHUDIcon__

#include "cocos2d.h"
USING_NS_CC;
#include "PlayerSpriteProfile.h"

class PlayerHUDIcon : public CCSprite
{
    private:
	PlayerHUDIcon(GamePlayerProfile* playerProfile);
    
    public:
    static PlayerHUDIcon* create(GamePlayerProfile* playerProfile);
};

#endif /* defined(__PlayerHUDIcon__) */