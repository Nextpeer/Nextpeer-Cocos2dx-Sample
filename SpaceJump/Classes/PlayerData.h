//
//  Created by Nextpeer development team.
//  Copyright (c) 2014 Nextpeer, Inc. All rights reserved.
//

#ifndef __PlayerData__
#define __PlayerData__

#include "cocos2d.h"
#include "PlayerSpriteProfile.h"

USING_NS_CC;
using namespace std;

class PlayerData : public Ref {
private:
    string _playerId;
    string _playerName;
    bool _isRecording;
    PlayerData(string playerId, string playerName, bool isRecording);
public:
    static PlayerData* create(string playerId, string playerName, bool isRecording);
    
    string getPlayerId() const{
        return _playerId;
    }
    
    string getPlayerName() const {
        return _playerName;
    }
    
    bool getIsRecording() const {
        return _isRecording;
    }
    
    CC_SYNTHESIZE(GamePlayerProfileType, _profileType, ProfileType);
};

#endif /* defined(__PlayerData__) */
