//
//  Created by Nextpeer development team.
//  Copyright (c) 2014 Nextpeer, Inc. All rights reserved.
//

#ifndef __MainMenuScene__
#define __MainMenuScene__

#include "cocos2d.h"
USING_NS_CC;

class MainMenuScene : public CCLayer
{
public:
    ~MainMenuScene();
    
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(MainMenuScene);
    
private:
    CCSize _screenSize;
    CCSpriteBatchNode * _menuBatchNode;
    CCSpriteBatchNode *_charactersBatchNode;
    CCSprite *_selectedAvatar;
    CCMenu *_menuAvatarSelectionRight;
    CCMenu *_menuAvatarSelectionLeft;
    CCArray *_availableAvatars;
    unsigned int _selectedAvtarIndex;
    void createScreen();
    void menuCallbackStartGame(CCObject* pSender);
    void menuCallbackChangeAvatarToRight(CCObject* pSender);
    void menuCallbackChangeAvatarToLeft(CCObject* pSender);
};


#endif /* defined(__MainMenuScene__) */
