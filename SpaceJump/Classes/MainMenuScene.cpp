#include "MainMenuScene.h"

#include "PlayerSpriteProfile.h"
#include "CCNextpeer.h"

#define HUD_ITEMS_SPACING 10.0f

USING_NS_CC;

Scene* MainMenuScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainMenuScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

MainMenuScene::~MainMenuScene()
{
    CC_SAFE_RELEASE_NULL(_availableAvatars);
}

// on "init" you need to initialize your instance
bool MainMenuScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    createScreen();
    
    return true;
}

void MainMenuScene::createScreen () {
    
    //get screen size
    _screenSize = Director::getInstance()->getWinSize();
    _selectedAvtarIndex = 0;
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("menu.plist");
    _menuBatchNode = SpriteBatchNode::create("menu.png", 200);
    this->addChild(_menuBatchNode);
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("characters.plist");
    _charactersBatchNode = SpriteBatchNode::create("characters.png", 200);
    this->addChild(_charactersBatchNode);
    
    // Place the logo on the top-center of the screen
    Sprite *logo = Sprite::createWithSpriteFrameName("menu_logo.png");
    logo->setPosition(Point(_screenSize.width * 0.5f, _screenSize.height * 0.85));
    _menuBatchNode->addChild(logo);
    
    // Place the moon on the right-bottom corner of the screen
    Sprite *moon = Sprite::createWithSpriteFrameName("menu_moon.png");
    moon->setPosition(Point(_screenSize.width - moon->getContentSize().width/2, moon->getContentSize().height/3));
    addChild(moon);
    
    // Place the earth on the left-bottom corner of the screen
    Sprite *earth = Sprite::createWithSpriteFrameName("menu_earth.png");
    earth->setPosition(Point(earth->getContentSize().width/3, earth->getContentSize().height/3));
    addChild(earth);
    
    // Try to extract the current user profile avatar, if manage to place the avatar selection utils and animate the earth, moon out of the way.
    GamePlayerProfile *currentUserProfile = GamePlayerProfileFactory::createProfileByTypeOrNull(CurrentPlayerGameProfile::getCurrentUserProfile());
    if (currentUserProfile != NULL) {
        _availableAvatars = GamePlayerProfileFactory::allGamePlayerProfiles();
        _availableAvatars->retain();
        
        // Find the selected avatar index
        Ref* it = NULL;
        _selectedAvtarIndex = 0;
        CCARRAY_FOREACH(_availableAvatars, it) {
            GamePlayerProfile *profile = static_cast<GamePlayerProfile*>(it);
            if (profile->profileType() == currentUserProfile->profileType()) {
                break;
            }
            _selectedAvtarIndex++;
        }
        
        _selectedAvatar = Sprite::createWithSpriteFrameName(currentUserProfile->spriteFrontName());
        _selectedAvatar->setPosition(Point(_screenSize.width*0.5, _selectedAvatar->getContentSize().height * 2));
        _selectedAvatar->setOpacity(0);
        _charactersBatchNode->addChild(_selectedAvatar);
        
        // Select Avatar -> Right
        Node* avatarSelectionRightNode = Sprite::createWithSpriteFrameName("menu_select_right.png");
        
        MenuItemSprite* avatarSelectionRightMenuItem = MenuItemSprite::create(avatarSelectionRightNode, avatarSelectionRightNode, CC_CALLBACK_1(MainMenuScene::menuCallbackChangeAvatarToRight, this));
        _menuAvatarSelectionRight = CCMenu::create(avatarSelectionRightMenuItem, NULL);
        _menuAvatarSelectionRight->setPosition(Point(_selectedAvatar->getPositionX() + avatarSelectionRightNode->getContentSize().width + HUD_ITEMS_SPACING, _selectedAvatar->getPositionY()));
        _menuAvatarSelectionRight->setOpacity(0);
        this->addChild(_menuAvatarSelectionRight);
        
        // Select Avatar -> Left
        Node* avatarSelectionLeftNode = Sprite::createWithSpriteFrameName("menu_select_left.png");
        MenuItemSprite* avatarSelectionLeftMenuItem = MenuItemSprite::create(avatarSelectionLeftNode, avatarSelectionLeftNode, CC_CALLBACK_1(MainMenuScene::menuCallbackChangeAvatarToLeft, this));
        _menuAvatarSelectionLeft = CCMenu::create(avatarSelectionLeftMenuItem, NULL);
        _menuAvatarSelectionLeft->setPosition(Point(_selectedAvatar->getPositionX() - avatarSelectionLeftNode->getContentSize().width - HUD_ITEMS_SPACING, _selectedAvatar->getPositionY()));
        _menuAvatarSelectionLeft->setOpacity(0);
        this->addChild(_menuAvatarSelectionLeft);
        
        // Animation to hide the moon/earth and show the avatar selection controls
        earth->runAction(Spawn::create(MoveTo::create(0.5f, Point(-earth->getContentSize().width, earth->getPositionY())), FadeOut::create(1.4f), NULL));
        moon->runAction(Spawn::create(MoveTo::create(0.5f, Point(_screenSize.width + moon->getContentSize().width, moon->getPositionY())), FadeOut::create(1.4f), NULL));
        _selectedAvatar->runAction(CCSequence::create(CCDelayTime::create(0.1), CCFadeIn::create(0.4f), NULL));
        _menuAvatarSelectionRight->runAction(CCSequence::create(CCDelayTime::create(0.2), CCFadeIn::create(0.3f), NULL));
        _menuAvatarSelectionLeft->runAction(CCSequence::create(CCDelayTime::create(0.2), CCFadeIn::create(0.3f), NULL));
    }
    
    // Open Nextpeer
    Node* openNextpeerButtonNode = Sprite::createWithSpriteFrameName("menu_play_normal.png");
    Node* openNextpeerSelectedButtonNode = Sprite::createWithSpriteFrameName("menu_play_selected.png");
    MenuItemSprite* openNextpeerMenuItem = MenuItemSprite::create(openNextpeerButtonNode, openNextpeerSelectedButtonNode, CC_CALLBACK_1(MainMenuScene::menuCallbackStartGame, this));
    Menu *openNextpeerMenu = Menu::create(openNextpeerMenuItem, NULL);
    openNextpeerMenu->setPosition(Point(_screenSize.width*0.5, _screenSize.height*0.5));
    this->addChild(openNextpeerMenu);
}

void MainMenuScene::menuCallbackStartGame(Ref* pSender) {
    nextpeer::CCNextpeer::getInstance()->launchDashboard();
}

void MainMenuScene::menuCallbackChangeAvatarToRight(Ref* pSender) {
    if (_selectedAvtarIndex + 1 >= _availableAvatars->count()) {
        _selectedAvtarIndex = 0;
    }
    else {
        _selectedAvtarIndex++;
    }
    
    GamePlayerProfile *profile = (GamePlayerProfile *)_availableAvatars->getObjectAtIndex(_selectedAvtarIndex);
    
    CurrentPlayerGameProfile::setCurrentUserProfile(profile->profileType());
    _selectedAvatar->setDisplayFrame(CCSpriteFrameCache::getInstance()->getSpriteFrameByName(profile->spriteFrontName()));
}

void MainMenuScene::menuCallbackChangeAvatarToLeft(Ref* pSender) {
    if (_selectedAvtarIndex == 0) {
        _selectedAvtarIndex = _availableAvatars->count() - 1;
    }
    else {
        _selectedAvtarIndex--;
    }
    
    GamePlayerProfile *profile = (GamePlayerProfile *)_availableAvatars->getObjectAtIndex(_selectedAvtarIndex);
    
    CurrentPlayerGameProfile::setCurrentUserProfile(profile->profileType());
    _selectedAvatar->setDisplayFrame(CCSpriteFrameCache::getInstance()->getSpriteFrameByName(profile->spriteFrontName()));
}

