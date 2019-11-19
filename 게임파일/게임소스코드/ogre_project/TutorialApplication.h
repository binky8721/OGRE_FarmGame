#ifndef TUTORIALAPPLICATION_H
#define TUTORIALAPPLICATION_H

#include <OgreRoot.h>
#include <OgreWindowEventUtilities.h>

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h> //�߰���
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

//�⺻ �Լ�(�ּ����� �ʿ���)
protected:

  virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);

  virtual void windowResized(Ogre::RenderWindow* rw);
  virtual void windowClosed(Ogre::RenderWindow* rw);

  virtual void createScene(void);


//�⺻ ����(�ּ��� �ʿ��� ����)
protected:
  Ogre::Root* mRoot;
  Ogre::RenderWindow* mWindow;
  Ogre::SceneManager* mSceneMgr; //���ξ�
  Ogre::Camera* mCamera;
  Ogre::String mResourcesCfg;
  Ogre::String mPluginsCfg;

  OIS::InputManager* mInputMgr;
  OIS::Keyboard* mKeyboard;
  OIS::Mouse* mMouse;

//�߰��� ����
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
//�߰��� ����
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
	

	Ogre::SceneNode * player; //�÷��̾�(���ΰ�) ���
	Ogre::SceneNode * water1; //ȣ��1 ���
	Ogre::SceneNode * house;

	Ogre::SceneNode * plow1; //������1,2,3...
	Ogre::SceneNode * plow2;
	Ogre::SceneNode * plow3;

	//�����1,2,3,4
	Ogre::SceneNode * wall1;
	Ogre::SceneNode * wall2;
	Ogre::SceneNode * wall3;
	Ogre::SceneNode * wall4;

	//���ΰ��� ���� �浹
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

	//���Ŵ���
	Ogre::SceneManager * TitleSceneMgr; //��ŸƮ��
	Ogre::SceneManager * GameOverSceneMgr; // ���ӿ�����
	Ogre::SceneManager * GameClearSceneMgr; // Ŭ�����

	//Ogre::Image backgroundImage;
	Ogre::Overlay* GameClearbackground_overlay;
	Ogre::Overlay* GameOverbackground_overlay;
	Ogre::Overlay* background_overlay;
	Ogre::Overlay* titleText_overlay;
	Ogre::Overlay* inventory_overlay;

	//Ogre::Overlay* seed1_Lackwater_overlay;
	//Ogre::OverlayElement* seed1_Lackwater_Text;
	//Ogre::OverlayContainer* seed1_Lackwater_overlay_Container;

	//3��Ī ī�޶�� ���
	Ogre::SceneNode *tpsCameraNode;
	Ogre::SceneNode *sightNode;

	//��ư
	OgreBites::Button * titleStartButton;
	OgreBites::Button * waterItemButton;
	OgreBites::Button * gameOverReStartButton;
	OgreBites::Button * gameClearReStartButton;

	//��Ÿ UI����
	OgreBites::ParamsPanel * CropNumberPanel;
	OgreBites::ParamsPanel * WaterNumberPanel;

	//���� �����ð� ���� ����
	int LeftTime;
	int UpdatedLeftTime;
	OgreBites::ParamsPanel * leftTimePanel;
	Ogre::Timer gameLimitTimer;

	//���Ѱ���Ÿ�̸�
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

	Ogre::Timer plantWaterTimer; //���ѿ� ���ֱ�ִϸ��̼ǿ� Ÿ�̸�
	
	//��ƼŬ Ÿ�̸� �� �����
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


	//���ѳ�� & ��ƼƼ
	Ogre::Entity* cropEntity;
	Ogre::Entity* cropEntity2;
	Ogre::Entity* cropEntity3;
	Ogre::SceneNode * cropNode;
	Ogre::SceneNode * cropNode2;
	Ogre::SceneNode * cropNode3;
	
	//���ڶ� �۹���� & ��ƼƼ
	Ogre::Entity* cropMatureEntity;
	Ogre::Entity* cropMatureEntity2;
	Ogre::Entity* cropMatureEntity3;
	Ogre::SceneNode * cropMatureNode;
	Ogre::SceneNode * cropMatureNode2;
	Ogre::SceneNode * cropMatureNode3;

	//��
	Ogre::Entity* enemyEntity;
	Ogre::SceneNode * enemyNode;

	//���� ����
	CSound sound;

	//������ ���ں���
	int waterCanUseNumber;
	int matureCropNumber;

	//������ ���º���
	enum SeedState{Normal,Thirsty,Dead};
	int Seed1State;
	int Seed2State;
	int Seed3State;

	
//�߰��� �Լ�
protected:
	virtual void createCamera(void);
	virtual void createViewports(void);
	virtual void createFrameListener(void);

	virtual bool keyPressed( const OIS::KeyEvent &arg ); //Ű���� ��ư���� ����
	virtual bool keyReleased( const OIS::KeyEvent &arg ); //Ű���� ��ư�� ����
	virtual bool mouseMoved(const OIS::MouseEvent &arg);
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	

//�߰��� �Լ�
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

	//�ؽ�Ʈ ��������
	bool setOverlayCaption(const Ogre::String &elementName, const Ogre::String &caption);
	bool showOverlay(const Ogre::String &overlayName);
	bool isShowText;
	Ogre::Overlay* textOverlay;

	//�𷺼ųζ���Ʈ �̿��� ����ȿ��
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