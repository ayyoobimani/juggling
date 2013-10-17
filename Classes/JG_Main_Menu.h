#ifndef __MainMenu_SCENE_H__
#define __MainMenu_SCENE_H__

#include "cocos2d.h"

class JG_Main_Menu : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

	void menuPauseCallBack(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(JG_Main_Menu);
};

#endif // __MainMenu_SCENE_H__
