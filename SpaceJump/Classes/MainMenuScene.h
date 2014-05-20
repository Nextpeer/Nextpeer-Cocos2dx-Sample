//
//  Created by Nextpeer development team.
//  Copyright (c) 2014 Nextpeer, Inc. All rights reserved.
//

#ifndef __MainMenuScene__
#define __MainMenuScene__

#include "cocos2d.h"
USING_NS_CC;

class MainMenuScene : public Layer
{
public:
    ~MainMenuScene();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(MainMenuScene);
    
private:
    Size _screenSize;
    SpriteBatchNode * _menuBatchNode;
    SpriteBatchNode *_charactersBatchNode;
    Sprite *_selectedAvatar;
    Menu *_menuAvatarSelectionRight;
    Menu *_menuAvatarSelectionLeft;
    __Array *_availableAvatars;
    unsigned int _selectedAvtarIndex;
    void createScreen();
    void menuCallbackStartGame(Ref* pSender);
    void menuCallbackChangeAvatarToRight(Ref* pSender);
    void menuCallbackChangeAvatarToLeft(Ref* pSender);
};

#endif /* defined(__MainMenuScene__) */
