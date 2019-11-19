#ifndef TUTORIALAPPLICATION_H
#define TUTORIALAPPLICATION_H

#include <OgreRoot.h>
#include <OgreWindowEventUtilities.h>

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h> //추가함
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <OgreException.h>
#include<OgreMeshManager.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <SdkTrays.h>
#include <SdkCameraMan.h>

#include"Sound.h"

class TutorialApplication
  : public Ogre::WindowEventListener,
    public Ogre::FrameListener,
	public OgreBites::SdkTrayListener,
	public OIS::KeyListener, public OIS::MouseListener
{
public:
  TutorialApplication();
  virtual ~TutorialApplication();

  bool go();

//기본 함수(최소한의 필요기능)
protected:

  virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);

  virtual void windowResized(Ogre::RenderWindow* rw);
  virtual void windowClosed(Ogre::RenderWindow* rw);

  virtual void createScene(void);


//기본 변수(최소한 필요한 변수)
protected:
  Ogre::Root* mRoot;
  Ogre::RenderWindow* mWindow;
  Ogre::SceneManager* mSceneMgr; //메인씬
  Ogre::Camera* mCamera;
  Ogre::String mResourcesCfg;
  Ogre::String mPluginsCfg;

  OIS::InputManager* mInputMgr;
  OIS::Keyboard* mKeyboard;
  OIS::Mouse* mMouse;

//추가한 변수
protected:

	OgreBites::InputContext     mInputContext;
	OgreBites::SdkCameraMan*    mCameraMan;     	// Basic camera controller
	OgreBites::SdkTrayManager*	mTrayMgr;
	OgreBites::SdkTrayManager*	itemTrayMgr;
	OgreBites::SdkTrayManager*	titleTrayMgr;
	OgreBites::SdkTrayManager*	gameOverTrayMgr;
	OgreBites::SdkTrayManager*	gameclearTrayMgr;

	OgreBites::ParamsPanel*     mDetailsPanel;   	// Sample details panel

	Ogre::OverlaySystem*        mOverlaySystem;
	Ogre::OverlaySystem*        mOverlaySystem2;
	bool                        mCursorWasVisible;	// Was cursor visible before dialog appeared?

	bool mShutDown;
//추가한 변수
private:
	bool isTitleScene;
	bool isMainScene;
	bool isGameClearScene;
	bool isGameOverScene;
	bool isOpenInventory;

	bool isPlayerMove;
	bool isPlayerMoveLeft;
	bool isPlayerMoveRight;
	bool isPlayerMoveUp;
	bool isPlayerMoveDown;

	bool isCanPlantSeed;
	bool isPlantedPlow1;
	bool isPlantedPlow2;
	bool isPlantedPlow3;

	bool isCameraLookAtPlayer;
	

	Ogre::SceneNode * player; //플레이어(주인공) 노드
	Ogre::SceneNode * water1; //호수1 노드
	Ogre::SceneNode * house;

	Ogre::SceneNode * plow1; //경작지1,2,3...
	Ogre::SceneNode * plow2;
	Ogre::SceneNode * plow3;

	//벽노드1,2,3,4
	Ogre::SceneNode * wall1;
	Ogre::SceneNode * wall2;
	Ogre::SceneNode * wall3;
	Ogre::SceneNode * wall4;

	//주인공와 물의 충돌
	Ogre::AxisAlignedBox aab_PlayerToWater;

	Ogre::AxisAlignedBox aab_PlayerToPlow1;
	Ogre::AxisAlignedBox aab_PlayerToPlow2;
	Ogre::AxisAlignedBox aab_PlayerToPlow3;

	Ogre::AxisAlignedBox aab_PlayerToWall1;
	Ogre::AxisAlignedBox aab_PlayerToWall2;
	Ogre::AxisAlignedBox aab_PlayerToWall3;
	Ogre::AxisAlignedBox aab_PlayerToWall4;

	Ogre::AxisAlignedBox aab_PlayerToHouse;

	Ogre::AnimationState * playerAnimationState1;
	Ogre::AnimationState * playerAnimationState2;
	Ogre::AnimationState * playerAnimationState3;
	float playerWalkSpeed;

	Ogre::Vector3 playerDirection;

	//씬매니저
	Ogre::SceneManager * TitleSceneMgr; //스타트씬
	Ogre::SceneManager * GameOverSceneMgr; // 게임오버씬
	Ogre::SceneManager * GameClearSceneMgr; // 클리어씬

	//Ogre::Image backgroundImage;
	Ogre::Overlay* GameClearbackground_overlay;
	Ogre::Overlay* GameOverbackground_overlay;
	Ogre::Overlay* background_overlay;
	Ogre::Overlay* titleText_overlay;
	Ogre::Overlay* inventory_overlay;

	//Ogre::Overlay* seed1_Lackwater_overlay;
	//Ogre::OverlayElement* seed1_Lackwater_Text;
	//Ogre::OverlayContainer* seed1_Lackwater_overlay_Container;

	//3인칭 카메라용 노드
	Ogre::SceneNode *tpsCameraNode;
	Ogre::SceneNode *sightNode;

	//버튼
	OgreBites::Button * titleStartButton;
	OgreBites::Button * waterItemButton;
	OgreBites::Button * gameOverReStartButton;
	OgreBites::Button * gameClearReStartButton;

	//기타 UI변수
	OgreBites::ParamsPanel * CropNumberPanel;
	OgreBites::ParamsPanel * WaterNumberPanel;

	//게임 남은시간 관련 변수
	int LeftTime;
	int UpdatedLeftTime;
	OgreBites::ParamsPanel * leftTimePanel;
	Ogre::Timer gameLimitTimer;

	//씨앗관련타이머
	Ogre::Timer seed1_waterTimer;
	Ogre::Timer seed1_waterTimer2;
	bool isSeed1WaterLackOneMoreTime;
	bool isSeed2WaterLackOneMoreTime;
	bool isSeed3WaterLackOneMoreTime;
	Ogre::Timer seed2_waterTimer;
	Ogre::Timer seed2_waterTimer2;
	Ogre::Timer seed3_waterTimer;
	Ogre::Timer seed3_waterTimer2;

	Ogre::Timer seed_weedRandomTimer;
	Ogre::Timer seed1_weedTimer;
	Ogre::Timer seed2_weedTimer;
	Ogre::Timer seed3_weedTimer;

	Ogre::Timer seed1_growTimer;
	Ogre::Timer seed2_growTimer;
	Ogre::Timer seed3_growTimer;

	Ogre::Timer plantWaterTimer; //씨앗에 물주기애니메이션용 타이머
	
	//파티클 타이머 및 제어변수
	Ogre::Timer seed1_destoyParticleTimer;
	Ogre::Timer seed2_destoyParticleTimer;
	Ogre::Timer seed3_destoyParticleTimer;
	Ogre::ParticleSystem* destroyParticle1;
	Ogre::ParticleSystem* destroyParticle2;
	Ogre::ParticleSystem* destroyParticle3;
	Ogre::SceneNode* destroyparticleNode1;
	Ogre::SceneNode* destroyparticleNode2;
	Ogre::SceneNode* destroyparticleNode3;
	bool isSeed1Destroy;
	bool isSeed2Destroy;
	bool isSeed3Destroy;

	Ogre::Timer seed1_matureParticleTimer;
	Ogre::Timer seed2_matureParticleTimer;
	Ogre::Timer seed3_matureParticleTimer;
	Ogre::ParticleSystem* matureParticle1;
	Ogre::ParticleSystem* matureParticle2;
	Ogre::ParticleSystem* matureParticle3;
	Ogre::SceneNode* matureparticleNode1;
	Ogre::SceneNode* matureparticleNode2;
	Ogre::SceneNode* matureparticleNode3;
	bool isSeed1Mature;
	bool isSeed2Mature;
	bool isSeed3Mature;
	bool isPlantedMaturePlow1;
	bool isPlantedMaturePlow2;
	bool isPlantedMaturePlow3;


	//씨앗노드 & 엔티티
	Ogre::Entity* cropEntity;
	Ogre::Entity* cropEntity2;
	Ogre::Entity* cropEntity3;
	Ogre::SceneNode * cropNode;
	Ogre::SceneNode * cropNode2;
	Ogre::SceneNode * cropNode3;
	
	//다자란 작물노드 & 엔티티
	Ogre::Entity* cropMatureEntity;
	Ogre::Entity* cropMatureEntity2;
	Ogre::Entity* cropMatureEntity3;
	Ogre::SceneNode * cropMatureNode;
	Ogre::SceneNode * cropMatureNode2;
	Ogre::SceneNode * cropMatureNode3;

	//적
	Ogre::Entity* enemyEntity;
	Ogre::SceneNode * enemyNode;

	//사운드 변수
	CSound sound;

	//아이템 숫자변수
	int waterCanUseNumber;
	int matureCropNumber;

	//씨앗의 상태변수
	enum SeedState{Normal,Thirsty,Dead};
	int Seed1State;
	int Seed2State;
	int Seed3State;

	
//추가한 함수
protected:
	virtual void createCamera(void);
	virtual void createViewports(void);
	virtual void createFrameListener(void);

	virtual bool keyPressed( const OIS::KeyEvent &arg ); //키보드 버튼누름 동작
	virtual bool keyReleased( const OIS::KeyEvent &arg ); //키보드 버튼땜 동작
	virtual bool mouseMoved(const OIS::MouseEvent &arg);
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	

//추가한 함수
private:
	void InitializeAxisAlignedBox();
	void CheckBoundingBox();
	void CheckPlantSeed();
	void CheckPlantWater();
	void CheckWaterTimer();
	void CheckGrowSeedTimer();
	void CreateBackground();
	void CreateGameClearBackground();
	void CreateGameOverBackground();
	void CreateInventory();
	void CreateOverlay();
	void DestroyBackground();
	void MovePlayer();
	void ItemCheckWater();

	void GameClear();
	void GameOver();
	
	void buttonHit(OgreBites::Button *button);


	virtual void setupViewport(Ogre::SceneManager *curr);
	virtual void dualViewport(Ogre::SceneManager *primarySceneMgr, Ogre::SceneManager *secondarySceneMgr);

	//텍스트 오버레이
	bool setOverlayCaption(const Ogre::String &elementName, const Ogre::String &caption);
	bool showOverlay(const Ogre::String &overlayName);
	bool isShowText;
	Ogre::Overlay* textOverlay;

	//디렉셔널라이트 이용한 낮밤효과
	void DayNightChange(const Ogre::FrameEvent& evt);
	Ogre::Light* directionlight;
	Ogre::Timer directionlight_Timer;
	float dlightDirection;
	bool isD_end;
	bool isD_start;
	Ogre::ParticleSystem *dayParticle;
	Ogre::SceneNode* dayParticleNode;
	Ogre::ParticleSystem *nightParticle;
	Ogre::SceneNode* nightParticleNode;

};

#endif