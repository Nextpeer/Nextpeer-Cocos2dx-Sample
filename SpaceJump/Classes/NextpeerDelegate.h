//
//  Created by Nextpeer development team.
//  Copyright (c) 2014 Nextpeer, Inc. All rights reserved.
//

#ifndef __NextpeerDelegate__
#define __NextpeerDelegate__

#include "cocos2d.h"
USING_NS_CC;

#include "MultiplayerGameState.h"

class NextpeerDelegate : public Ref
{
public:
    void registerForEvents();
    void unhookEvents();
    
    // Event calls
    void nextpeerDidStartTournament(Ref* startData);
    void nextpeerDidEndTournament(Ref *endData);
    
    NextpeerDelegate();
    virtual ~NextpeerDelegate();
    
protected:
    MultiplayerGameState* _currentGameState;
};

#endif /* defined(__NextpeerDelegate__) */
