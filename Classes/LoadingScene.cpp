//
//  LoadingScene.cpp
//  Moon3d
//
//  Created by Jacky on 3/10/14.
//
//

#include "LoadingScene.h"
#include "HelloWorldScene.h"
#include "3d/Sprite3D.h"
#include "AirCraft.h"
#include "Enemies.h"
#include "GameControllers.h"
#include "consts.h"

int LoadingScene::updatecount=0;

LoadingScene::~LoadingScene()
{
    NotificationCenter::getInstance()->destroyInstance();
}

Scene* LoadingScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LoadingScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LoadingScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	InitBk();
    InitCoco();
    LoadingResource();
    
    NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(LoadingScene::GotoNextScene),"GotoNextScene",NULL);
    scheduleUpdate();
    
    return true;
}

void LoadingScene::InitBk()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    //bk
    auto loading_bk=Sprite::create("loading_bk.png");
    loading_bk->setPosition(Point(visibleSize.width/2, visibleSize.height/2));
    addChild(loading_bk,0);
    
    
    //LabelPercent
    m_pPercent=LabelBMFont::create("0%", "num.fnt");
    m_pPercent->setPosition(Point(visibleSize.width/2,visibleSize.height/2+170));
    this->addChild(m_pPercent,1);
    
    //ControlSlider
	m_pSlider = ControlSlider::create("loading_progress_bk.png","loading_progress_progress.png" ,"loading_progress_thumb.png");
	m_pSlider->setAnchorPoint(Point(0.5f, 1.0f));
	m_pSlider->setMinimumValue( 0 );
	m_pSlider->setMaximumValue(100);
	m_pSlider->setPosition(Point(visibleSize.width/2, visibleSize.height/2+300));
	m_pSlider->setValue(0);
	addChild(m_pSlider,1);
}

void LoadingScene::InitCoco()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto coco = Sprite3D::create("coconut.obj", "coco.png");
    if(coco)
    {
        coco->setPosition(Point(visibleSize.width/2, visibleSize.height/2-150));
        coco->setOutline(10,Color3B(0,0,0));
        addChild(coco,1);
        coco->runAction(RepeatForever::create(RotateBy::create(0.8f,Vertex3F(0,360,0))));
    }
}

void LoadingScene::LoadingResource()
{
    //Loading Music
    LoadingMusic();
    
    //Loading Picture
    LoadingPic();
}

void LoadingScene::LoadingMusic()
{
    auto Audio = CocosDenshion::SimpleAudioEngine::getInstance();
    Audio->preloadEffect("explodeEffect.mp3");
    Audio->preloadEffect("hit.mp3");
    Audio->preloadEffect("boom2.mp3");
    // Music By Matthew Pable (http://www.matthewpablo.com/)
    // Licensed under CC-BY 3.0 (http://creativecommons.org/licenses/by/3.0/)
    Audio->playBackgroundMusic("Flux.mp3");
}

void LoadingScene::LoadingPic()
{
	auto TexureCache=Director::getInstance()->getTextureCache();
    TexureCache->addImageAsync("groundLevel.jpg", CC_CALLBACK_1(LoadingScene::LoadingCallback, this));
	TexureCache->addImageAsync("bullets.png", CC_CALLBACK_1(LoadingScene::LoadingCallback, this));
	//TexureCache->addImageAsync("car00.png", CC_CALLBACK_1(LoadingScene::LoadingCallback, this));
	//TexureCache->addImageAsync("CloseNormal.png", CC_CALLBACK_1(LoadingScene::LoadingCallback, this));
	//TexureCache->addImageAsync("CloseSelected.png", CC_CALLBACK_1(LoadingScene::LoadingCallback, this));
	//TexureCache->addImageAsync("cloud.png", CC_CALLBACK_1(LoadingScene::LoadingCallback, this));
	TexureCache->addImageAsync("daodan_32.png", CC_CALLBACK_1(LoadingScene::LoadingCallback, this));
	//TexureCache->addImageAsync("daodan_512.png", CC_CALLBACK_1(LoadingScene::LoadingCallback, this));
	TexureCache->addImageAsync("diji02_v002_128.png", CC_CALLBACK_1(LoadingScene::LoadingCallback, this));
	TexureCache->addImageAsync("hp_empty.png", CC_CALLBACK_1(LoadingScene::LoadingCallback, this));
	TexureCache->addImageAsync("dijiyuanv001.png", CC_CALLBACK_1(LoadingScene::LoadingCallback, this));
	//TexureCache->addImageAsync("Done1.png", CC_CALLBACK_1(LoadingScene::LoadingCallback, this));
	//TexureCache->addImageAsync("fighter.png", CC_CALLBACK_1(LoadingScene::LoadingCallback, this));
	TexureCache->addImageAsync("fog.png", CC_CALLBACK_1(LoadingScene::LoadingCallback, this));
	//TexureCache->addImageAsync("HelloWorld.png", CC_CALLBACK_1(LoadingScene::LoadingCallback, this));
	//TexureCache->addImageAsync("leftTop.png", CC_CALLBACK_1(LoadingScene::LoadingCallback, this));
	TexureCache->addImageAsync("playerv002_256.png", CC_CALLBACK_1(LoadingScene::LoadingCallback, this));
	TexureCache->addImageAsync("score_right_top.png", CC_CALLBACK_1(LoadingScene::LoadingCallback, this));
	TexureCache->addImageAsync("right_top_ui.png", CC_CALLBACK_1(LoadingScene::LoadingCallback, this));
	TexureCache->addImageAsync("streak.png", CC_CALLBACK_1(LoadingScene::LoadingCallback, this));
}

void LoadingScene::LoadingCallback(Ref* pObj)
{
    ++currentNum;
    char tmp[10];
	int percent=(int)(((float)currentNum / totalNum) * 100);
    sprintf(tmp, "%d%%", percent);
    m_pPercent->setString(tmp);
    m_pSlider->setValue(percent);
    

    if (currentNum == totalNum)
    {
        NotificationCenter::getInstance()->postNotification("GotoNextScene",NULL);
    }
}

void LoadingScene::GotoNextScene(Ref* pObj)
{
    
    this->removeAllChildren();
    
    //goto next scene.
    auto helloworldScene=HelloWorld::createScene();
    Director::getInstance()->replaceScene(TransitionFlipY::create(1.0f,helloworldScene,TransitionScene::Orientation::DOWN_OVER));
}

void LoadingScene::update(float dt)
{
    ++updatecount;
    log("updateCount:%d........",updatecount);
    
    if (m_curPreload_fodder_count<PRELOAD_FODDER_COUNT) {
        LoadingEnemy(kEnemyFodder);
        m_curPreload_fodder_count++;
        log("update_create_enmey_fodder");
    }
    else if(m_curPreload_fodderL_count<PRELOAD_FODDERL_COUNT)
    {
        LoadingEnemy(kEnemyFodderL);
        m_curPreload_fodderL_count++;
        log("update_create_enmey_fodderL");
    }
    else if (m_curPreload_BigDude_count<PRELOAD_BIGDUDE_COUBR)
    {
        LoadingEnemy(kEnemyBigDude);
        m_curPreload_BigDude_count++;
        log("update_create_enmey_BigDude");
    }
    else
    {
        unscheduleUpdate();
    }
}

void LoadingScene::LoadingEnemy(int type)
{
    switch(type)
    {
        case kEnemyFodder:
        {
            auto enemy_fodder = Fodder::create();
            enemy_fodder->retain();
            EnemyController::_fodderPool.pushBack(enemy_fodder);
        }
            break;
        case kEnemyFodderL:
        {
            auto enemy_fodderL= FodderLeader::create();
            enemy_fodderL->retain();
            EnemyController::_fodderLPool.pushBack(enemy_fodderL);
        }
            break;
        case kEnemyBigDude:
        {
            auto enmey_bigdude= BigDude::create();
            enmey_bigdude->retain();
            EnemyController::_bigDudePool.pushBack(enmey_bigdude);
        }
            break;
        default:
            break;
    }
}