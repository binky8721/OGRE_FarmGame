#include "TutorialApplication.h"

#define CAMERA_NAME "SceneCamera"

#define LEFT_TIME 5000000000
//#define LEFT_TIME 100000000


TutorialApplication::TutorialApplication()
	: mRoot(0),
	mResourcesCfg(Ogre::StringUtil::BLANK),
	mPluginsCfg(Ogre::StringUtil::BLANK),
	mWindow(0),mSceneMgr(0),mCamera(0),TitleSceneMgr(0),GameOverSceneMgr(0),GameClearSceneMgr(0),
	mInputMgr(0),mMouse(0),mKeyboard(0),
	mCameraMan(0),mTrayMgr(0),mDetailsPanel(0),titleTrayMgr(0),itemTrayMgr(0),gameclearTrayMgr(0),gameOverTrayMgr(0),
	mShutDown(false),mCursorWasVisible(false),mOverlaySystem(0),mOverlaySystem2(0),
	isCameraLookAtPlayer(true),isCanPlantSeed(false),isPlayerMoveUp(false),isPlayerMoveDown(false),isPlayerMoveLeft(false),isPlayerMoveRight(false),
	isPlantedPlow1(false),isPlantedPlow2(false),isPlantedPlow3(false),isOpenInventory(false),
	isPlayerMove(false),isTitleScene(true),	isMainScene(false),isGameClearScene(false),isGameOverScene(false),
	LeftTime(LEFT_TIME),waterCanUseNumber(5), isSeed1Destroy(false),isSeed2Destroy(false),isSeed3Destroy(false), isSeed1Mature(false),isSeed2Mature(false),isSeed3Mature(),
	isPlantedMaturePlow1(false),isPlantedMaturePlow2(false),isPlantedMaturePlow3(false),
	isSeed1WaterLackOneMoreTime(false),isSeed2WaterLackOneMoreTime(false),isSeed3WaterLackOneMoreTime(false),matureCropNumber(0),dlightDirection(1.0f),isD_end(false),isD_start(true),
	Seed1State(Normal),Seed2State(Normal),Seed3State(Normal)
{
}

TutorialApplication::~TutorialApplication()
{
	if (mTrayMgr) delete mTrayMgr;
	if(titleTrayMgr) delete titleTrayMgr;
	if(itemTrayMgr) delete itemTrayMgr;
	if(gameclearTrayMgr) delete gameclearTrayMgr;
	if(gameOverTrayMgr) delete gameOverTrayMgr;
	if (mCameraMan) delete mCameraMan;
	if (mOverlaySystem) delete mOverlaySystem;
	if(mOverlaySystem2) delete mOverlaySystem2; 

	// Remove ourself as a Window listener
	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);
	delete mRoot;
}

void TutorialApplication::createCamera(void)
{
	// Create the camera
	//Ogre::String camName = mSceneMgr->getName();	
	TitleSceneMgr->createCamera(TitleSceneMgr->getName()+"camera");
	GameOverSceneMgr->createCamera(GameOverSceneMgr->getName()+"camera");
	GameClearSceneMgr->createCamera(GameClearSceneMgr->getName()+"camera");

	mCamera = mSceneMgr->createCamera(mSceneMgr->getName()+"camera");
	mCamera->setPosition(Ogre::Vector3(6.6, 563, 800));
	//mCamera->setPosition(Ogre::Vector3(6.6, 563, 1486));
	mCamera->lookAt(Ogre::Vector3(0, 0, 0));
	mCamera->setNearClipDistance(5);
	mCameraMan = new OgreBites::SdkCameraMan(mCamera);   // Create a default camera controller
}
void TutorialApplication::setupViewport(Ogre::SceneManager *curr)
{
	mWindow->removeAllViewports();
	Ogre::Camera *cam = curr->getCamera(curr->getName()+"camera"); // The Camera
	Ogre::Viewport *vp = mWindow->addViewport(cam); // Our Viewport linked to the camera
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
	cam->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}

void TutorialApplication::createViewports() 
{
	setupViewport(TitleSceneMgr);
	sound.PlayTitle_BGM();
	//soundManager->PlayTitle_BGM(); 
}

void TutorialApplication::dualViewport(Ogre::SceneManager *primarySceneMgr, Ogre::SceneManager *secondarySceneMgr)
{
	mWindow->removeAllViewports();

	Ogre::Viewport *vp = 0;
	Ogre::Camera * cam = primarySceneMgr->getCamera(primarySceneMgr->getName()+"camera");
	vp = mWindow->addViewport(cam, 0, 0, 0, 0.5, 1);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
	cam->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	cam = secondarySceneMgr->getCamera(secondarySceneMgr->getName()+"camera");
	vp = mWindow->addViewport(cam, 1, 0.5, 0, 0.5, 1);
	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
	cam->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}

static void swapViewport(Ogre::SceneManager *&first, Ogre::SceneManager *&second)
{
	Ogre::SceneManager *tmp = first;
	first = second;
	second = tmp;
}

void TutorialApplication::DestroyBackground()
{
	background_overlay->hide();
	//titleText_overlay->hide();
}



void TutorialApplication::CreateBackground()
{	
	mTrayMgr->hideAll(); //��� Ʈ���� ����
	itemTrayMgr->hideAll();
	titleStartButton = titleTrayMgr->createButton(OgreBites::TrayLocation::TL_CENTER,"TitleStartButton","Game Start!");
	//titleStartButton->show(); //��ŸƮ��ư ������

	Ogre::OverlayContainer* panel;
	Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();

	//��׶��� �̹���
	panel = static_cast<Ogre::OverlayContainer*>(overlayManager.createOverlayElement("Panel", "PanelName"));
	panel->setMetricsMode(Ogre::GMM_PIXELS);
	panel->setPosition(0, 0);
	//panel->setDimensions(100,100);
	panel->setDimensions(mWindow->getWidth(),mWindow->getHeight()); //�������� ���μ��� ũ�⸸ŭ �÷��� ����Ѵ�.

	Ogre::MaterialPtr backgroundmaterial = Ogre::MaterialManager::getSingleton().create("Background", "General");
	backgroundmaterial->getTechnique(0)->getPass(0)->createTextureUnitState("TitleImage.jpg");
	backgroundmaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
	backgroundmaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
	backgroundmaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	panel->setMaterialName("Examples/TitleImage");

	background_overlay = overlayManager.create("background_overlay");
	background_overlay->add2D(panel);
	background_overlay->show();

	//�ؽ�Ʈ �̹���
	/*
	Ogre::OverlayContainer* panel2;
	//Ogre::OverlayManager& overlayManager2 = Ogre::OverlayManager::getSingleton();
	panel2 = static_cast<Ogre::OverlayContainer*>(overlayManager.createOverlayElement("Panel", "PanelName2"));
	panel2->setMetricsMode(Ogre::GMM_PIXELS);
	panel2->setPosition(300, 300);
	panel2->setDimensions(200,75); 

	Ogre::MaterialPtr TitleTextmaterial = Ogre::MaterialManager::getSingleton().create("TitleText", "General");
	TitleTextmaterial->getTechnique(0)->getPass(0)->createTextureUnitState("TitleUI_Text1.jpg");
	TitleTextmaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
	TitleTextmaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
	TitleTextmaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	panel2->setMaterialName("Examples/TitleUI_Text1");

	titleText_overlay = overlayManager.create("titleText_overlay");
	titleText_overlay->add2D(panel2);
	titleText_overlay->show();
	*/


	/*
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create("Background","General");//Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	material->getTechnique(0)->getPass(0)->createTextureUnitState("rockwall.tga");
	material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
	material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
	material->getTechnique(0)->getPass(0)->setLightingEnabled(false);

	// Create background rectangle covering the whole screen
	Ogre::Rectangle2D* rect = new Ogre::Rectangle2D(true);
	rect->setCorners(-1.0f, 1.0f, 1.0f, -1.0f);
	rect->setMaterial("Background");

	// Render the background before everything else
	rect->setRenderQueueGroup(Ogre::RenderQueueGroupID::RENDER_QUEUE_BACKGROUND);

	// Use infinite AAB to always stay visible
	Ogre::AxisAlignedBox aabInf;
	aabInf.setInfinite();
	rect->setBoundingBox(aabInf);

	// Attach background to the scene
	Ogre::SceneNode* background_node = TitleSceneMgr->getRootSceneNode()->createChildSceneNode("Background");
	background_node->attachObject(rect);

	// Example of background scrolling
	material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setScrollAnimation(-0.25, 0.0);

	// Don't forget to delete the Rectangle2D in the destructor of your application:
	delete rect;
	*/
}

void TutorialApplication::CreateGameClearBackground()
{	
	Ogre::OverlayContainer* panel;
	Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();

	//��׶��� �̹���
	panel = static_cast<Ogre::OverlayContainer*>(overlayManager.createOverlayElement("Panel", "GameClearPanel"));
	panel->setMetricsMode(Ogre::GMM_PIXELS);
	panel->setPosition(0, 0);
	panel->setDimensions(mWindow->getWidth(),mWindow->getHeight()); //�������� ���μ��� ũ�⸸ŭ �÷��� ����Ѵ�.

	panel->setMaterialName("Examples/GameClear");

	GameClearbackground_overlay = overlayManager.create("GameClearbackground_overlay");
	GameClearbackground_overlay->add2D(panel);
	GameClearbackground_overlay->hide();

	gameClearReStartButton = gameclearTrayMgr->createButton(OgreBites::TrayLocation::TL_BOTTOM,"GameClearReStartButton","ReStart!");
}

void TutorialApplication::CreateGameOverBackground()
{	
	Ogre::OverlayContainer* panel;
	Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();

	//��׶��� �̹���
	panel = static_cast<Ogre::OverlayContainer*>(overlayManager.createOverlayElement("Panel", "GameOverPanel"));
	panel->setMetricsMode(Ogre::GMM_PIXELS);
	panel->setPosition(0, 0);
	panel->setDimensions(mWindow->getWidth(),mWindow->getHeight()); //�������� ���μ��� ũ�⸸ŭ �÷��� ����Ѵ�.

	panel->setMaterialName("Examples/GameOver");

	GameOverbackground_overlay = overlayManager.create("GameOverbackground_overlay");
	GameOverbackground_overlay->add2D(panel);
	GameOverbackground_overlay->hide();

	gameOverReStartButton = gameOverTrayMgr->createButton(OgreBites::TrayLocation::TL_BOTTOM,"GameOverReStartButton","ReStart!");
}

void TutorialApplication::CreateInventory()
{	

	Ogre::OverlayContainer* panel;
	Ogre::OverlayManager& overlayManager2 = Ogre::OverlayManager::getSingleton();
	//Ogre::MaterialPtr empty_material;

	//�κ��丮 ��׶��� �̹���
	panel = static_cast<Ogre::OverlayContainer*>(overlayManager2.createOverlayElement("Panel", "PanelName2"));
	panel->setMetricsMode(Ogre::GMM_PIXELS);
	panel->setPosition(0, mWindow->getHeight()-100);
	//panel->setDimensions(100,100);
	panel->setDimensions(mWindow->getWidth(),100); //�������� ���μ��� ũ�⸸ŭ �÷��� ����Ѵ�.

	Ogre::MaterialPtr Inventorymaterial = Ogre::MaterialManager::getSingleton().create("Inventory", "General");
	Inventorymaterial->getTechnique(0)->getPass(0)->createTextureUnitState("MRAMOR6X6.jpg");
	Inventorymaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
	Inventorymaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
	Inventorymaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	panel->setMaterialName("Examples/InventoryBase");

	inventory_overlay = overlayManager2.create("Inventory_overlay");
	inventory_overlay->add2D(panel);
	inventory_overlay->show();
	//inventory_overlay->setZOrder(50);
	//�� �̹��� ����


	//�� ��ư ����
	waterItemButton = itemTrayMgr->createButton(OgreBites::TrayLocation::TL_BOTTOM,"WaterButton","water");
	itemTrayMgr->hideAll();
	itemTrayMgr->hideCursor();
}

void TutorialApplication::createScene(void)
{
	//Ÿ��Ʋ ��׶���
	//CreateBackground();
	CreateInventory();
	//��ī�̹ڽ� �߰�
	mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox"); //���ξ� ��ī�̹ڽ�
	//TitleSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);

	//�𷺼ų� ����Ʈ
	directionlight = mSceneMgr->createLight("directionlight");
	directionlight->setType(Ogre::Light::LT_DIRECTIONAL);
	directionlight->setDiffuseColour(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
	directionlight->setDirection(0,1,0);

	//���� ���� �ð� Ÿ�̸� ����
	gameLimitTimer.reset(); 

	//�����Ʈ
	mSceneMgr->setAmbientLight(Ogre::ColourValue(1, 1, 1));

	// Create a plane (�⺻��)
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, -10);

	Ogre::MeshManager::getSingleton().createPlane("plane",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
		5000,5000,200,200,true,1,5,5,Ogre::Vector3::UNIT_Z);

	Ogre::Entity* ent = mSceneMgr->createEntity("LightPlaneEntity", "plane");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ent);
	ent->setMaterialName("Examples/Rockwall");
	Ogre::SceneNode* node = mSceneMgr->createSceneNode("Node1");
	mSceneMgr->getRootSceneNode()->addChild(node);
	ent->setCastShadows(false); //���� �׸��ڰ� ������ �ʿ䰡 ����

	//������1
	Ogre::Plane harvestPlane1((Ogre::Vector3::UNIT_Y), -9.9);
	Ogre::MeshManager::getSingleton().createPlane("harvestPlane1",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, harvestPlane1,
		300,300,200,200,true,1,5,5,Ogre::Vector3::UNIT_Z);

	Ogre::Entity* LP_ent1 = mSceneMgr->createEntity("LightPlaneEntity1", Ogre::SceneManager::PT_CUBE);
	Ogre::SceneNode* harvestPlane1node = mSceneMgr->getRootSceneNode()->createChildSceneNode("harvestPlane1Node");
	harvestPlane1node->attachObject(LP_ent1);
	harvestPlane1node->setPosition(-500,-10,300);
	harvestPlane1node->setScale(3.0f,0.01f,3.0f);
	LP_ent1->setMaterialName("Examples/GrassFloor");
	LP_ent1->setCastShadows(false); //���� �׸��ڰ� ������ �ʿ䰡 ����
	plow1 = harvestPlane1node;

	//������2
	Ogre::Plane harvestPlane2(Ogre::Vector3::UNIT_Y, -9.9);
	Ogre::MeshManager::getSingleton().createPlane("harvestPlane2",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, harvestPlane2,
		800,800,200,200,true,1,5,5,Ogre::Vector3::UNIT_Z);

	Ogre::Entity* LP_ent2 = mSceneMgr->createEntity("LightPlaneEntity2", Ogre::SceneManager::PT_CUBE);
	Ogre::SceneNode* harvestPlane2node = mSceneMgr->getRootSceneNode()->createChildSceneNode("harvestPlane2Node");
	harvestPlane2node->attachObject(LP_ent2);
	harvestPlane2node->setPosition(0,-10,300);
	harvestPlane2node->setScale(3.0f,0.01f,3.0f);
	LP_ent2->setMaterialName("Examples/GrassFloor");
	LP_ent2->setCastShadows(false); //���� �׸��ڰ� ������ �ʿ䰡 ����
	plow2 = harvestPlane2node;

	//������3
	Ogre::Plane harvestPlane3(Ogre::Vector3::UNIT_Y, -9.9);
	Ogre::MeshManager::getSingleton().createPlane("harvestPlane3",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, harvestPlane3,
		300,300,200,200,true,1,5,5,Ogre::Vector3::UNIT_Z);

	Ogre::Entity* LP_ent3 = mSceneMgr->createEntity("LightPlaneEntity3", Ogre::SceneManager::PT_CUBE);
	Ogre::SceneNode* harvestPlane3node = mSceneMgr->getRootSceneNode()->createChildSceneNode("harvestPlane3Node");
	harvestPlane3node->attachObject(LP_ent3);
	harvestPlane3node->setPosition(500,-10,300);
	harvestPlane3node->setScale(3.0f,0.01f,3.0f);
	LP_ent3->setMaterialName("Examples/GrassFloor");
	LP_ent3->setCastShadows(false); //���� �׸��ڰ� ������ �ʿ䰡 ����
	plow3 = harvestPlane3node;

	//�� plane
	Ogre::Plane waterPlane1(Ogre::Vector3::UNIT_Y,-9.9);
	Ogre::MeshManager::getSingleton().createPlane("waterPlane1",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, waterPlane1,
		300,300,200,200,true,1,5,5,Ogre::Vector3::UNIT_Z);

	Ogre::Entity* LP_ent4 = mSceneMgr->createEntity("waterPlaneEntity1", Ogre::SceneManager::PT_SPHERE);
	Ogre::SceneNode* waterPlane1node = mSceneMgr->getRootSceneNode()->createChildSceneNode("waterPlane1Node");
	waterPlane1node->attachObject(LP_ent4);
	waterPlane1node->setPosition(0,-10,-300);
	waterPlane1node->setScale(3.0f,0.01f,3.0f);
	LP_ent4->setMaterialName("Examples/TextureEffect3");
	LP_ent4->setCastShadows(false); //���� �׸��ڰ� ������ �ʿ䰡 ����
	water1 = waterPlane1node;

	//���� ����Ʈ
	// Create a spot light
	Ogre::Light* light = mSceneMgr->createLight("Light1");
	light->setType(Ogre::Light::LT_SPOTLIGHT);
	light->setDirection(Ogre::Vector3(0, -1, 0));
	light->setDiffuseColour(Ogre::ColourValue(0.0f, 1.0f, 0.0f));

	//light->setSpotlightInnerAngle(Ogre::Degree(5.0f));
	//light->setSpotlightOuterAngle(Ogre::Degree(90.0f));
	//light->setSpotlightFalloff(0.0f);

	Ogre::SceneNode* node2 = node->createChildSceneNode("node2");
	node2->setPosition(800, 3000, 0);
	node2->attachObject(light);
	Ogre::Entity* lightEnt = mSceneMgr->createEntity("MyEntity","sphere.mesh");
	Ogre::SceneNode* node3 = node2->createChildSceneNode("node3");
	node3->setScale(0.1f, 0.1f, 0.1f);
	node3->attachObject(lightEnt);

	//��
	Ogre::Entity* houseEntity = mSceneMgr->createEntity("house","tudorhouse.mesh");
	Ogre::SceneNode *houseNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("houseNode");
	houseNode->setPosition(-800,200,-800);
	houseNode->setScale(0.5f,0.5f,0.5f);
	houseNode->attachObject(houseEntity);
	house = houseNode;

	//��Ÿ��
	//������
	Ogre::Entity* wallEntity1 = mSceneMgr->createEntity("Wall1", "WoodPallet.mesh"); 
	Ogre::SceneNode *wallnode1 = mSceneMgr->getRootSceneNode()->createChildSceneNode("wallnode1");
	wallnode1->setPosition(2500,0,0);
	wallnode1->setScale(100.0f,50.0f,700.0f);
	wallnode1->attachObject(wallEntity1);
	wall1 = wallnode1;
	wall1->roll(Ogre::Degree(-90.0f));
	//����
	Ogre::Entity* wallEntity2 = mSceneMgr->createEntity("Wall2", "WoodPallet.mesh"); 
	Ogre::SceneNode *wallnode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode("wallnode2");
	wallnode2->setPosition(-2500,0,0);
	wallnode2->setScale(100.0f,100.0f,700.0f);
	wallnode2->attachObject(wallEntity2);
	wall2 = wallnode2;
	wall2->roll(Ogre::Degree(90.0f));
	//����
	Ogre::Entity* wallEntity3 = mSceneMgr->createEntity("Wall3", "WoodPallet.mesh"); 
	Ogre::SceneNode *wallnode3 = mSceneMgr->getRootSceneNode()->createChildSceneNode("wallnode3");
	wallnode3->setPosition(0,0,2500);
	wallnode3->setScale(700.0f,100.0f,100.0f);
	wallnode3->attachObject(wallEntity3);
	wall3 = wallnode3;
	wall3->pitch(Ogre::Degree(90.0f));
	//����
	Ogre::Entity* wallEntity4 = mSceneMgr->createEntity("Wall4", "WoodPallet.mesh"); 
	Ogre::SceneNode *wallnode4 = mSceneMgr->getRootSceneNode()->createChildSceneNode("wallnode4");
	wallnode4->setPosition(0,0,-2500);
	wallnode4->setScale(700.0f,50.0f,100.0f);
	wallnode4->attachObject(wallEntity4);
	wall4 = wallnode4;
	wall4->pitch(Ogre::Degree(-90.0f));

	//���ΰ�
	Ogre::Entity* playerEntity = mSceneMgr->createEntity("Ninja", "ninja.mesh"); 
	Ogre::SceneNode *playernode = mSceneMgr->getRootSceneNode()->createChildSceneNode("PlayerNode");
	playernode->setPosition(0,-30,0);
	playernode->attachObject(playerEntity);
	playerEntity->setCastShadows(true);
	player = playernode;

	playerAnimationState1 = playerEntity->getAnimationState("Idle1");
	playerAnimationState1->setLoop(true);
	playerAnimationState1->setEnabled(true);
	playerAnimationState2 = playerEntity->getAnimationState("Walk");
	playerAnimationState2->setLoop(true);
	playerAnimationState2->setEnabled(false);
	playerAnimationState3 = playerEntity->getAnimationState("Backflip");
	playerAnimationState3->setLoop(true);
	playerAnimationState3->setEnabled(false);

	//3��Ī ī�޶�
	tpsCameraNode = player->createChildSceneNode("TpsCamera");
	tpsCameraNode->attachObject(mCamera);
	//���̶���Ʈ�� �÷��̾��� �ڽ����� ���� ����ٴϵ��� �ϰ��ϱ�
	node3 = player->createChildSceneNode();


	//�׸���
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
	//mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE);

	mCamera->lookAt(mSceneMgr->getSceneNode("PlayerNode")->getPosition());

	//�����ð� �г�
	Ogre::StringVector time;
	time.push_back("Left Time : ");
	leftTimePanel = mTrayMgr->createParamsPanel(OgreBites::TL_TOP,"leftTimePanel",200,time);
	//���� �۹����� �г�
	Ogre::StringVector CropNumber;
	CropNumber.push_back("GetCropNumber : ");
	CropNumberPanel = mTrayMgr->createParamsPanel(OgreBites::TL_TOPLEFT,"CropNumberPanel",200,CropNumber);
	//���� �� ����
	Ogre::StringVector WaterNumber;
	WaterNumber.push_back("RemainWater : ");
	WaterNumberPanel = mTrayMgr->createParamsPanel(OgreBites::TL_LEFT,"WaterNumberPanel",200,WaterNumber);


	//�ı�����Ʈ ��ƼŬ
	destroyParticle1 = mSceneMgr->createParticleSystem("GreenyNimbus", "Examples/GreenyNimbus");
	destroyparticleNode1 = mSceneMgr->getRootSceneNode()->createChildSceneNode("DetroyParticle1");
	destroyparticleNode1->attachObject(destroyParticle1);
	destroyparticleNode1->setPosition(plow1->getPosition().x,plow1->getPosition().y,plow1->getPosition().z);
	destroyparticleNode1->setVisible(false);

	destroyParticle2 = mSceneMgr->createParticleSystem("GreenyNimbus2", "Examples/GreenyNimbus");
	destroyparticleNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode("DetroyParticle2");
	destroyparticleNode2->attachObject(destroyParticle2);
	destroyparticleNode2->setPosition(plow2->getPosition().x,plow2->getPosition().y,plow2->getPosition().z);
	destroyparticleNode2->setVisible(false);

	destroyParticle3 = mSceneMgr->createParticleSystem("GreenyNimbus3", "Examples/GreenyNimbus");
	destroyparticleNode3 = mSceneMgr->getRootSceneNode()->createChildSceneNode("DetroyParticle3");
	destroyparticleNode3->attachObject(destroyParticle3);
	destroyparticleNode3->setPosition(plow3->getPosition().x,plow3->getPosition().y,plow3->getPosition().z);
	destroyparticleNode3->setVisible(false);

	matureParticle1 = mSceneMgr->createParticleSystem("PurpleFountain", "Examples/Aureola");
	matureparticleNode1 = mSceneMgr->getRootSceneNode()->createChildSceneNode("MatureParticle1");
	matureparticleNode1->attachObject(matureParticle1);
	matureparticleNode1->setPosition(plow1->getPosition().x,plow1->getPosition().y,plow1->getPosition().z);
	matureparticleNode1->setVisible(false);

	matureParticle2 = mSceneMgr->createParticleSystem("PurpleFountain2", "Examples/Aureola");
	matureparticleNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode("MatureParticle2");
	matureparticleNode2->attachObject(matureParticle2);
	matureparticleNode2->setPosition(plow2->getPosition().x,plow2->getPosition().y,plow2->getPosition().z);
	matureparticleNode2->setVisible(false);

	matureParticle3 = mSceneMgr->createParticleSystem("PurpleFountain3", "Examples/Aureola");
	matureparticleNode3 = mSceneMgr->getRootSceneNode()->createChildSceneNode("MatureParticle3");
	matureparticleNode3->attachObject(matureParticle3);
	matureparticleNode3->setPosition(plow3->getPosition().x,plow3->getPosition().y,plow3->getPosition().z);
	matureparticleNode3->setVisible(false);

	dayParticle = mSceneMgr->createParticleSystem("dayParticle", "Examples/Rain");
	dayParticleNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("dayParticleNode");
	dayParticleNode->attachObject(dayParticle);
	dayParticleNode->setPosition(0,0,0);
	dayParticleNode->setVisible(false);

	nightParticle = mSceneMgr->createParticleSystem("nightParticle", "Examples/Snow");
	nightParticleNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("nightParticleNode");
	nightParticleNode->attachObject(nightParticle);
	nightParticleNode->setPosition(0,0,0);
	nightParticleNode->setVisible(false);

	//���ӹ�� �ؽ�Ʈ
	showOverlay("App/Profile");
	setOverlayCaption("Profile/Text","NPC:\nHello World!!");
	//setOverlayCaption("Profile/Text","Penguin:\nYour UFO is over there!!\nIf you want to cross the river,\nbring me 3 fishes!!!!\n\n**If you have 0 bullet,** \n**game will be over.**");
}

void TutorialApplication::DayNightChange(const Ogre::FrameEvent& evt)
{
	//��
	if(directionlight_Timer.getMilliseconds()>10000 && isD_start)
	{
		sound.PlayMorning_Sound();
		directionlight->setDirection(0.0f,-1.0f,0.0f);
		isD_start=!isD_start;
		directionlight_Timer.reset();
		nightParticleNode->setVisible(false);
		dayParticleNode->setVisible(true);
	}
	if(directionlight_Timer.getMilliseconds()>10000 && !isD_start)
	{
		directionlight->setDirection(0.0f,1.0f,0.0f);
		isD_start=!isD_start;
		directionlight_Timer.reset();
		nightParticleNode->setVisible(true);
		dayParticleNode->setVisible(false);
	}
}

bool TutorialApplication::go()
{
#ifdef _DEBUG
	mResourcesCfg = "resources_d.cfg";
	mPluginsCfg = "plugins_d.cfg";
#else
	mResourcesCfg = "resources.cfg";
	mPluginsCfg = "plugins.cfg";
#endif

	mRoot = new Ogre::Root(mPluginsCfg);

	Ogre::ConfigFile cf;
	cf.load(mResourcesCfg);

	Ogre::String name, locType;
	Ogre::ConfigFile::SectionIterator secIt = cf.getSectionIterator();

	while (secIt.hasMoreElements())
	{
		Ogre::ConfigFile::SettingsMultiMap* settings = secIt.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator it;

		for (it = settings->begin(); it != settings->end(); ++it)
		{
			locType = it->first;
			name = it->second;

			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(name, locType);
		}
	}


	if (!(mRoot->restoreConfig() || mRoot->showConfigDialog()))
		return false;
	mRoot->showConfigDialog();
	mWindow = mRoot->initialise(true, "TutorialApplication Render Window");

	//******chooseSceneManager***********///

	mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC,"MainScene");
	TitleSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "TitleScene");
	GameOverSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC ,"GameOverScene");
	GameClearSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC ,"GameClearScene");

	// Initialize the OverlaySystem (changed for Ogre 1.9)
	mOverlaySystem = new Ogre::OverlaySystem();
	//mOverlaySystem2 = new Ogre::OverlaySystem();
	TitleSceneMgr->addRenderQueueListener(mOverlaySystem);
	mSceneMgr->addRenderQueueListener(mOverlaySystem);
	GameOverSceneMgr->addRenderQueueListener(mOverlaySystem);
	GameClearSceneMgr->addRenderQueueListener(mOverlaySystem);


	//******chooseSceneManager***********///

	//******Create Camera******//
	createCamera();

	// Load resources
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	//******Create Viewport******//

	//TrayMgr �ʱ�ȭ
	mInputContext.mKeyboard = mKeyboard;
	mInputContext.mMouse = mMouse;

	titleTrayMgr = new OgreBites::SdkTrayManager("TitleTray", mWindow, mInputContext, this);
	mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, mInputContext, this);
	itemTrayMgr = new OgreBites::SdkTrayManager("ItemTray", mWindow, mInputContext, this);
	gameclearTrayMgr = new OgreBites::SdkTrayManager("gameClearTray", mWindow, mInputContext, this);
	gameOverTrayMgr = new OgreBites::SdkTrayManager("gameOverTray", mWindow, mInputContext, this);

	CreateBackground();
	CreateGameClearBackground();
	CreateGameOverBackground();
	createViewports();

	// Set default mipmap level (NB some APIs ignore this)
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	//Create Background
	//CreateBackground();

	//Create Scene
	createScene();

	createFrameListener();

	mRoot->startRendering();

	return true;
}

void TutorialApplication::createFrameListener(void)
{
	// OIS
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");

	OIS::ParamList pl; //creates a ParamList object. This is basically a typedef for a std::map. It is used to pass parameters to classes (similar to the Ogre::NameValuePairList).
	size_t windowHandle = 0; //creates a size_t object that we will use to retrieve a window handle from our render window.
	std::ostringstream windowHandleStr; //creates a std::ostringstream - we use this to "stringify" the size_t object created in line 2.
	mWindow->getCustomAttribute("WINDOW", &windowHandle); //populates the size_t object with the window handle. 'm_win' in this example is a pointer to an initialised Ogre::RenderWindow
	windowHandleStr << windowHandle; //performs the stringification - we need a string representation of the window handle.
	pl.insert(std::make_pair(std::string("WINDOW"), windowHandleStr.str())); // inserts into our parameter list the string "WINDOW", and the string object we created above.
	mInputMgr = OIS::InputManager::createInputSystem(pl); //actually creates the OIS input system. Note: m_InputManager is a pointer to an OIS::InputManager object.

	mKeyboard = static_cast<OIS::Keyboard*>(mInputMgr->createInputObject(OIS::OISKeyboard, true));
	mMouse = static_cast<OIS::Mouse*>(mInputMgr->createInputObject(OIS::OISMouse, true));

	mKeyboard->setEventCallback(this);
	mMouse->setEventCallback(this);

	windowResized(mWindow);

	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

	/*
	//mTrayMgr �ʱ�ȭ
	mInputContext.mKeyboard = mKeyboard;
	mInputContext.mMouse = mMouse;

	mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, mInputContext, this);
	*/
	//mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
	//mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
	//mTrayMgr->hideCursor();

	// Create a params panel for displaying sample details
	Ogre::StringVector items;
	items.push_back("cam.pX");
	items.push_back("cam.pY");
	items.push_back("cam.pZ");
	items.push_back("");
	items.push_back("cam.oW");
	items.push_back("cam.oX");
	items.push_back("cam.oY");
	items.push_back("cam.oZ");
	items.push_back("");
	items.push_back("Filtering");
	items.push_back("Poly Mode");

	mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 200, items);
	mDetailsPanel->setParamValue(9, "Bilinear");
	mDetailsPanel->setParamValue(10, "Solid");
	mDetailsPanel->hide();

	mRoot->addFrameListener(this);
}

void TutorialApplication::buttonHit(OgreBites::Button *button)
{
	if(button->getName() =="TitleStartButton")
	{
		sound.PlayStartButton_Sound();
		titleTrayMgr->destroyWidget("TitleStartButton");
		setupViewport(mSceneMgr);
		isTitleScene = !isTitleScene;
		DestroyBackground();
		sound.Title_BGM_Stop();
		LeftTime =LEFT_TIME;
		gameLimitTimer.reset(); 
		directionlight_Timer.reset();
		isTitleScene = false;
		isMainScene = true;
		mTrayMgr->showAll();
		titleTrayMgr->hideAll();
		background_overlay->hide();
		background_overlay->clear();
		sound.PlayStage1_BGM();
	}
	else if(button->getName() =="WaterButton")
	{
		waterCanUseNumber--;
	}
	else if(button->getName() =="GameClearReStartButton")
	{
		isGameOverScene = false;
		isGameClearScene = false;
		isMainScene = true;
		GameClearbackground_overlay->hide();
		gameclearTrayMgr->hideAll();
		setupViewport(mSceneMgr);
		mTrayMgr->showAll();
		leftTimePanel->show();
		CropNumberPanel->show();
		WaterNumberPanel->show();
		sound.Gameclear_Stop();
		sound.PlayStage1_BGM();
		LeftTime =LEFT_TIME;
		gameLimitTimer.reset();
		directionlight_Timer.reset();
		waterCanUseNumber = 5;
	    matureCropNumber = 0;
		player->setPosition(0,-30,0);
		inventory_overlay->hide();
	}
	else if(button->getName() =="GameOverReStartButton")
	{
		isGameOverScene = false;
		isGameClearScene = false;
		isMainScene = true;
		GameOverbackground_overlay->hide();
		gameOverTrayMgr->hideAll();
		setupViewport(mSceneMgr);
		mTrayMgr->showAll();
		leftTimePanel->show();
		CropNumberPanel->show();
		WaterNumberPanel->show();
		sound.Gameover_Stop();
		sound.PlayStage1_BGM();
		LeftTime =LEFT_TIME;
		gameLimitTimer.reset();
		directionlight_Timer.reset();
		waterCanUseNumber = 5;
	    matureCropNumber = 0;
		player->setPosition(0,-30,0);
		inventory_overlay->hide();
	}
}

//�����Ӹ��� �ǽð����� ����Ǵ� �ڵ� (�̵� �� ���� �̺�Ʈ)
bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if(isMainScene)
	{
		DayNightChange(evt);
		CheckWaterTimer();
		CheckGrowSeedTimer();
		ItemCheckWater();
		//�浹���� üũ
		InitializeAxisAlignedBox();
		CheckBoundingBox();
		//���ӿ�������
		if(LeftTime<0)
		{
			//x���̻� �۹����� ���� Ŭ����
			if(matureCropNumber>=3)
				GameClear();
			else
				GameOver();
			// mShutDown = true;
		}
		LeftTime = (LeftTime - (gameLimitTimer.getMilliseconds())) ;
		//�� �̵� ����
		if (mKeyboard->isKeyDown(OIS::KC_Z))
		{
			Ogre::Vector3 curPos = mSceneMgr->getSceneNode("node2")->getPosition();
			curPos.x +=1.0f;
			mSceneMgr->getSceneNode("node2")->setPosition(curPos);
		}
		if (mKeyboard->isKeyDown(OIS::KC_X))
		{
			Ogre::Vector3 curPos = mSceneMgr->getSceneNode("node2")->getPosition();
			curPos.x -=1.0f;
			mSceneMgr->getSceneNode("node2")->setPosition(curPos);
		}

		//����(���ΰ� ������)
		playerAnimationState1->addTime(evt.timeSinceLastFrame);
		playerAnimationState2->addTime(evt.timeSinceLastFrame);
		playerAnimationState3->addTime(evt.timeSinceLastFrame);
		MovePlayer();
		isPlayerMove = false;
		if(isPlayerMove)
		{
			playerAnimationState1->setEnabled(true);
			playerAnimationState2->setEnabled(false);
			playerAnimationState3->setEnabled(false);
		}
	}

	//Ÿ��Ʋ ������ Ÿ��Ʋȭ�� �гθ��� �ٸ� �г��� ���� �����.
	if(isTitleScene)
	{
		mDetailsPanel->hide(); //ī�޶� ����
		mTrayMgr->hideAll(); //��� Ʈ���� ����
		itemTrayMgr->hideAll();		
		gameclearTrayMgr->hideAll();
		gameOverTrayMgr->hideAll();
		background_overlay->show();
		inventory_overlay->hide();
		//if(!(OgreBites::Button*)titleTrayMgr->getWidget("TitleStartButton"))
		//	buttonHit((OgreBites::Button*)titleTrayMgr->getWidget("TitleStartButton"));
		//titleText_overlay->show();
	}
	/*
	else if(!isTitleScene)
	{
		isMainScene = true;
		mTrayMgr->showAll();
		titleTrayMgr->hideAll();
		background_overlay->hide();
		background_overlay->clear();
		//titleText_overlay->hide();
		sound.PlayStage1_BGM();
	}
	*/
	if (mWindow->isClosed())
		return false;
	if(mShutDown)
		return false;

	mKeyboard->capture();
	mMouse->capture();

	mTrayMgr->frameRenderingQueued(evt);
	titleTrayMgr->frameRenderingQueued(evt);
	itemTrayMgr->frameRenderingQueued(evt);
	gameclearTrayMgr->frameRenderingQueued(evt);
	gameOverTrayMgr->frameRenderingQueued(evt);

	//ī�޶� ȿ��(�÷��̾ ��ӹٶ�)
	//if(isCameraLookAtPlayer)
	//mCamera->lookAt(player->getPosition());

	if(!mTrayMgr->isDialogVisible())
	{
		mCameraMan->frameRenderingQueued(evt); // If dialog isn't up, then update the camera
		if (mDetailsPanel->isVisible())          // If details panel is visible, then update its contents
		{
			mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(mCamera->getDerivedPosition().x));
			mDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(mCamera->getDerivedPosition().y));
			mDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(mCamera->getDerivedPosition().z));
			mDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().w));
			mDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().x));
			mDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().y));
			mDetailsPanel->setParamValue(7, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().z));
		}
		if(leftTimePanel->isVisible())
		{
			leftTimePanel->setParamValue(0, Ogre::StringConverter::toString(LeftTime/10000000));
		}
		if(CropNumberPanel->isVisible())
		{
			CropNumberPanel->setParamValue(0, Ogre::StringConverter::toString(matureCropNumber));
		}
		if(WaterNumberPanel->isVisible())
		{
			WaterNumberPanel->setParamValue(0, Ogre::StringConverter::toString(waterCanUseNumber));
		}
	}
	return true;
}

void TutorialApplication::InitializeAxisAlignedBox()
{
	//���ΰ��� ���� �浹
	aab_PlayerToWater = player->_getWorldAABB().intersection(water1->_getWorldAABB()); 
	//���ΰ��� ������ �浹
	aab_PlayerToPlow1 = player->_getWorldAABB().intersection(plow1->_getWorldAABB());
	aab_PlayerToPlow2 = player->_getWorldAABB().intersection(plow2->_getWorldAABB());
	aab_PlayerToPlow3 = player->_getWorldAABB().intersection(plow3->_getWorldAABB());
	//���ΰ��� ���� �浹
	aab_PlayerToWall1 = player->_getWorldAABB().intersection(wall1->_getWorldAABB());
	aab_PlayerToWall2 = player->_getWorldAABB().intersection(wall2->_getWorldAABB());
	aab_PlayerToWall3 = player->_getWorldAABB().intersection(wall3->_getWorldAABB());
	aab_PlayerToWall4 = player->_getWorldAABB().intersection(wall4->_getWorldAABB());

	aab_PlayerToHouse = player->_getWorldAABB().intersection(house->_getWorldAABB());
}

void TutorialApplication::CheckBoundingBox()
{
	//���ΰ��� ���� �浹�ߴٸ�
	if(!aab_PlayerToWater.isNull())
	{
		sound.PlayWaterCharge_Sound();
		//�����Ƚ�� �ִ�����(5��)
		waterCanUseNumber = 5;
		return;
	}
	//���ΰ��� �������� �浹�ߴٸ�
	if(!aab_PlayerToPlow1.isNull() || !aab_PlayerToPlow2.isNull() || !aab_PlayerToPlow3.isNull())
	{
		//���ѻѸ��� �ൿ����.
		isCanPlantSeed = true;
		return;
	}
	else
	{
		isCanPlantSeed = false;
	}

}

void TutorialApplication::CheckWaterTimer()
{
	//�������� 8�ʰ� ������ �ٽ� ����������
	if(isPlantedPlow1&&seed1_waterTimer2.getMilliseconds()>8000 && Seed1State == Normal && isSeed1WaterLackOneMoreTime)
	{
		Seed1State = Thirsty;
		seed1_waterTimer.reset();
		isSeed1WaterLackOneMoreTime = false;
	}
	if(isPlantedPlow2&&seed2_waterTimer2.getMilliseconds()>8000 && Seed2State == Normal && isSeed2WaterLackOneMoreTime)
	{
		Seed2State = Thirsty;
		seed2_waterTimer.reset();
		isSeed2WaterLackOneMoreTime = false;
	}
	if(isPlantedPlow3&&seed3_waterTimer2.getMilliseconds()>8000 && Seed3State == Normal && isSeed3WaterLackOneMoreTime)
	{
		Seed3State = Thirsty;
		seed3_waterTimer.reset();
		isSeed3WaterLackOneMoreTime = false;
	}

	//�۹��� �ɾ����ְ� &������ Ÿ�̸� �߻��� 5�ʰ� ������ &������ �����̸� �۹� ����.
	if(isPlantedPlow1&&seed1_waterTimer.getMilliseconds()>5000 && Seed1State == Thirsty)
	{
		//�۹��� ���� �Ƚɾ�������
		isPlantedPlow1 = false;
		//�۹����� ����Ʈ(��ƼŬ) 2�ʵ��� �߻�
		seed1_destoyParticleTimer.reset();
		isSeed1Destroy = true;
		//���ѻ���
		mSceneMgr->destroyEntity(cropEntity);
		mSceneMgr->destroySceneNode(cropNode); //�۹��ı�
		//������ ���Ѱ��ú��� ���� �ʱ�ȭ
	}
	if(isPlantedPlow2&&seed2_waterTimer.getMilliseconds()>5000 && Seed2State == Thirsty)
	{
		isPlantedPlow2 = false;
		seed2_destoyParticleTimer.reset();
		isSeed2Destroy = true;
		mSceneMgr->destroyEntity(cropEntity2);
		mSceneMgr->destroySceneNode(cropNode2);
	}
	if(isPlantedPlow3&&seed3_waterTimer.getMilliseconds()>5000 && Seed3State == Thirsty)
	{
		isPlantedPlow3 = false;
		seed3_destoyParticleTimer.reset();
		isSeed3Destroy = true;
		mSceneMgr->destroyEntity(cropEntity3);
		mSceneMgr->destroySceneNode(cropNode3);
	}

	
	//�۹��� �ɾ����ְ� & �����������̸� ������ �������� ���
	/*
	if(isPlantedPlow1 &&Seed1State == Thirsty)
	{
		textOverlay->show();
		//seed1_Lackwater_overlay_Container->show();
		//seed1_Lackwater_overlay->show();
	}
	else if(isPlantedPlow1&&Seed1State == Normal)
	{
		textOverlay->hide();
		//seed1_Lackwater_overlay_Container->hide();
		//seed1_Lackwater_overlay->hide();
	}
	*/

	//�ı��� ����Ʈ 2�ʰ����
	if(seed1_destoyParticleTimer.getMilliseconds()<2000 && isSeed1Destroy)
	{
		destroyparticleNode1->setVisible(true);
	}
	else if(seed1_destoyParticleTimer.getMicroseconds()>2000 && isSeed1Destroy)
	{
		destroyparticleNode1->setVisible(false);
		isSeed1Destroy = false;
	}
	if(seed2_destoyParticleTimer.getMilliseconds()<2000 && isSeed2Destroy)
	{
		destroyparticleNode2->setVisible(true);
	}
	else if(seed2_destoyParticleTimer.getMicroseconds()>2000 && isSeed2Destroy)
	{
		destroyparticleNode2->setVisible(false);
		isSeed2Destroy = false;
	}
	if(seed3_destoyParticleTimer.getMilliseconds()<2000 && isSeed3Destroy)
	{
		destroyparticleNode3->setVisible(true);
	}
	else if(seed3_destoyParticleTimer.getMicroseconds()>2000 && isSeed3Destroy)
	{
		destroyparticleNode3->setVisible(false);
		isSeed3Destroy = false;
	}

}

void TutorialApplication::CheckGrowSeedTimer()
{
	//�۹� ������ 15�ʰ� ������ ���°� �����϶�
	if(isPlantedPlow1 && seed1_growTimer.getMilliseconds()>15000 && Seed1State == Normal)
	{
		isPlantedMaturePlow1 = true;
		isPlantedPlow1 = false;
		//���ѻ���
		mSceneMgr->destroyEntity(cropEntity);
		mSceneMgr->destroySceneNode(cropNode); //�۹��ı�
		//�۹�����Ϸ� ����Ʈ(��ƼŬ2�����)
		seed1_matureParticleTimer.reset();
		isSeed1Mature = true;
		//����ȿ����
		sound.PlayGrowUp_Sound();
		//������ ���Ѱ��ú��� �����ʱ�ȭ
		//���ڶ� �۹�������Ʈ ����
		cropMatureEntity = mSceneMgr->createEntity("cropMature1","penguin.mesh");
		cropMatureNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("cropMature1Node");
		cropMatureNode->attachObject(cropMatureEntity);
		cropMatureNode->setScale(1.0f,1.0f,1.0f);
		cropMatureNode->setPosition(plow1->getPosition().x,plow1->getPosition().y,plow1->getPosition().z);
	}
	if(isPlantedPlow2 && seed2_growTimer.getMilliseconds()>15000 && Seed2State == Normal)
	{
		isPlantedMaturePlow2 = true;
		isPlantedPlow2 = false;
		mSceneMgr->destroyEntity(cropEntity2);
		mSceneMgr->destroySceneNode(cropNode2);
		seed2_matureParticleTimer.reset();
		isSeed2Mature = true;
		sound.PlayGrowUp_Sound();
		cropMatureEntity2 = mSceneMgr->createEntity("cropMature2","penguin.mesh");
		cropMatureNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode("cropMature2Node");
		cropMatureNode2->attachObject(cropMatureEntity2);
		cropMatureNode2->setScale(1.0f,1.0f,1.0f);
		cropMatureNode2->setPosition(plow2->getPosition().x,plow2->getPosition().y,plow2->getPosition().z);
	}
	if(isPlantedPlow3 && seed3_growTimer.getMilliseconds()>15000 && Seed3State == Normal)
	{
		isPlantedMaturePlow3 = true;
		isPlantedPlow3 = false;
		mSceneMgr->destroyEntity(cropEntity3);
		mSceneMgr->destroySceneNode(cropNode3);
		seed3_matureParticleTimer.reset();
		isSeed3Mature = true;
		sound.PlayGrowUp_Sound();
		cropMatureEntity3 = mSceneMgr->createEntity("cropMature3","penguin.mesh");
		cropMatureNode3 = mSceneMgr->getRootSceneNode()->createChildSceneNode("cropMature3Node");
		cropMatureNode3->attachObject(cropMatureEntity3);
		cropMatureNode3->setScale(1.0f,1.0f,1.0f);
		cropMatureNode3->setPosition(plow3->getPosition().x,plow3->getPosition().y,plow3->getPosition().z);
	}

	//�۹� �ڶ��� ����Ʈ2�ʰ� ���
	if(seed1_matureParticleTimer.getMilliseconds()<2000 && isSeed1Mature)
	{
		matureparticleNode1->setVisible(true);
	}
	else if(seed1_matureParticleTimer.getMilliseconds()>2000 && isSeed1Mature)
	{
		matureparticleNode1->setVisible(false);
		isSeed1Mature =false;
	}
	if(seed2_matureParticleTimer.getMilliseconds()<2000 && isSeed2Mature)
	{
		matureparticleNode2->setVisible(true);
	}
	else if(seed2_matureParticleTimer.getMilliseconds()>2000 && isSeed2Mature)
	{
		matureparticleNode2->setVisible(false);
		isSeed2Mature =false;
	}
	if(seed3_matureParticleTimer.getMilliseconds()<2000 && isSeed3Mature)
	{
		matureparticleNode3->setVisible(true);
	}
	else if(seed3_matureParticleTimer.getMilliseconds()>2000 && isSeed3Mature)
	{
		matureparticleNode3->setVisible(false);
		isSeed3Mature =false;
	}

}


void TutorialApplication::CheckPlantSeed()
{
	//������ ������ �ִٸ�.
	if(isCanPlantSeed)
	{
		//������1�� �浹���϶�
		if(!aab_PlayerToPlow1.isNull())
		{
			if(!isPlantedPlow1 && !isPlantedMaturePlow1) //������ �Ƚɾ����ְ�, �ϼ����۹��� �������
			{
				cropEntity = mSceneMgr->createEntity("crop1", "ogrehead.mesh");
				cropNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("crop1Node");
				cropNode->attachObject(cropEntity);
				cropNode->setScale(1.0f,1.0f,1.0f);
				cropNode->setPosition(plow1->getPosition().x,plow1->getPosition().y,plow1->getPosition().z);
				isPlantedPlow1 = !isPlantedPlow1;
				sound.PlayPlant_Sound();
				Seed1State = Thirsty; //������ �ٷ� ����������
				seed1_waterTimer.reset(); //��Ÿ�̸� ����
				seed1_growTimer.reset(); //����ð� Ÿ�̸� ����
			}
			else if(!isPlantedPlow1 && isPlantedMaturePlow1) //�ϼ��� �۹��� �ִ°��
			{
				isPlantedMaturePlow1 = false;
				//��� ȿ����
				sound.PlayCropGet_Sound();
				//���ڶ��۹��ı�
				mSceneMgr->destroyEntity(cropMatureEntity);
				mSceneMgr->destroySceneNode(cropMatureNode); 
				matureCropNumber++; //�����Ƚ��1����.
			}
		}
		//������2�� �浹���϶�
		if(!aab_PlayerToPlow2.isNull())
		{
			if(!isPlantedPlow2 && !isPlantedMaturePlow2)
			{
				cropEntity2 = mSceneMgr->createEntity("crop2", "ogrehead.mesh");
				cropNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode("crop2Node");
				cropNode2->attachObject(cropEntity2);
				cropNode2->setScale(1.0f,1.0f,1.0f);
				cropNode2->setPosition(plow2->getPosition().x,plow2->getPosition().y,plow2->getPosition().z);
				isPlantedPlow2 = !isPlantedPlow2;
				sound.PlayPlant_Sound();
				Seed2State = Thirsty;
				seed2_waterTimer.reset();
				seed2_growTimer.reset();
			}
			else if(!isPlantedPlow2 && isPlantedMaturePlow2)
			{
				isPlantedMaturePlow2 = false;
				sound.PlayCropGet_Sound();
				mSceneMgr->destroyEntity(cropMatureEntity2);
				mSceneMgr->destroySceneNode(cropMatureNode2); 
				matureCropNumber++;
			}
		}
		//������3�� �浹���϶�
		if(!aab_PlayerToPlow3.isNull())
		{
			if(!isPlantedPlow3 && !isPlantedMaturePlow3)
			{
				cropEntity3 = mSceneMgr->createEntity("crop3", "ogrehead.mesh");
				cropNode3 = mSceneMgr->getRootSceneNode()->createChildSceneNode("crop3Node");
				cropNode3->attachObject(cropEntity3);
				cropNode3->setScale(1.0f,1.0f,1.0f);
				cropNode3->setPosition(plow3->getPosition().x,plow3->getPosition().y,plow3->getPosition().z);
				isPlantedPlow3 = !isPlantedPlow3;
				sound.PlayPlant_Sound();
				Seed3State = Thirsty;
				seed3_waterTimer.reset();
				seed3_growTimer.reset();
			}
			else if(!isPlantedPlow3 && isPlantedMaturePlow3)
			{
				isPlantedMaturePlow3 = false;
				sound.PlayCropGet_Sound();
				mSceneMgr->destroyEntity(cropMatureEntity3);
				mSceneMgr->destroySceneNode(cropMatureNode3); 
				matureCropNumber++;
			}
		}
	}
}

void TutorialApplication::CheckPlantWater()
{
	//������ ������ �ִٸ�.
	if(isCanPlantSeed)
	{
		//������1�� �浹���϶�
		if(!aab_PlayerToPlow1.isNull())
		{
			if(isPlantedPlow1) //������ �ɾ����ִٸ�.
			{
				if(Seed1State == Thirsty) //�����������϶�
				{
					Seed1State = Normal;
					seed1_waterTimer2.reset();
					isSeed1WaterLackOneMoreTime = true;
				}
			}
		}
		//������2�� �浹���϶�
		if(!aab_PlayerToPlow2.isNull())
		{
			if(isPlantedPlow2)
			{
				if(Seed2State == Thirsty)
				{
					Seed2State = Normal;
					seed2_waterTimer2.reset();
					isSeed2WaterLackOneMoreTime = true;
				}
			}
		}
		//������3�� �浹���϶�
		if(!aab_PlayerToPlow3.isNull())
		{
			if(isPlantedPlow3)
			{
				if(Seed3State == Thirsty)
				{
					Seed3State = Normal;
					seed3_waterTimer2.reset();
					isSeed3WaterLackOneMoreTime = true;
				}
			}
		}
	}
}

void TutorialApplication::MovePlayer()
{
	//��4 ��3 ��2 ����1
		if(isPlayerMoveUp)
		{
			//�̵�
			Ogre::Vector3 curPos = Ogre::Vector3::ZERO;
			if(aab_PlayerToWall4.isNull())
			{
				curPos.z -=1.0f;
			}
			if(!aab_PlayerToHouse.isNull()) //���� �浹���̸�
					curPos.z +=2.0f;
			mSceneMgr->getSceneNode("PlayerNode")->translate(curPos,Ogre::Node::TS_LOCAL);
			//ȸ��
			//Ogre::Vector3 direction = Ogre::Vector3(0,0,1);
			//Ogre::Vector3 src = mSceneMgr->getSceneNode("PlayerNode")->getOrientation()* -Ogre::Vector3::UNIT_Z;
			//Ogre::Quaternion quat = src.getRotationTo(src);
			//mSceneMgr->getSceneNode("PlayerNode")->rotate(quat);
			//tpsCameraNode->rotate(quat);
			//�ȱ� �������
			sound.PlayWalk_Sound();
		}
		else if(isPlayerMoveDown)
		{
			Ogre::Vector3 curPos = Ogre::Vector3::ZERO;
			if(aab_PlayerToWall3.isNull())
				curPos.z +=1.0f;
			if(!aab_PlayerToHouse.isNull()) //���� �浹���̸�
					curPos.z -=2.0f;
			mSceneMgr->getSceneNode("PlayerNode")->translate(curPos,Ogre::Node::TS_LOCAL);
			sound.PlayWalk_Sound();
		}
		else if(isPlayerMoveLeft)
		{
			Ogre::Vector3 curPos = Ogre::Vector3::ZERO;
			if(aab_PlayerToWall2.isNull())
				curPos.x -=1.0f;
			if(!aab_PlayerToHouse.isNull()) //���� �浹���̸�
				curPos.x +=2.0f;
			mSceneMgr->getSceneNode("PlayerNode")->translate(curPos,Ogre::Node::TS_LOCAL);
			sound.PlayWalk_Sound();
		}
		else if(isPlayerMoveRight)
		{
			Ogre::Vector3 curPos = Ogre::Vector3::ZERO;
			if(aab_PlayerToWall1.isNull())
				curPos.x+=1.0f;
			if(!aab_PlayerToHouse.isNull()) //���� �浹���̸�
				curPos.x -=2.0f;
			mSceneMgr->getSceneNode("PlayerNode")->translate(curPos,Ogre::Node::TS_LOCAL);
			sound.PlayWalk_Sound();
		}
}

void TutorialApplication::ItemCheckWater()
{
	if(isOpenInventory)
	{
		if(waterCanUseNumber<=0)
		{
			if((OgreBites::Button*)itemTrayMgr->getWidget("WaterButton")) //�� ��ư ����� ����
				itemTrayMgr->destroyWidget("WaterButton");
		}
		else
		{
			if(!(OgreBites::Button*)itemTrayMgr->getWidget("WaterButton")) //�� ��ư�� ���ٸ� ����
				waterItemButton = itemTrayMgr->createButton(OgreBites::TrayLocation::TL_BOTTOM,"WaterButton","water");
		}
	}
}
void TutorialApplication::GameClear()
{		
	if(!(OgreBites::Button*)gameclearTrayMgr->getWidget("GameClearReStartButton"))
		gameClearReStartButton = gameclearTrayMgr->createButton(OgreBites::TrayLocation::TL_BOTTOM,"GameClearReStartButton","ReStart!");
	setupViewport(GameClearSceneMgr);
	mTrayMgr->hideAll();
	leftTimePanel->hide();
	CropNumberPanel->hide();
	WaterNumberPanel->hide();
	itemTrayMgr->hideAll();
	gameOverTrayMgr->hideAll();
	gameclearTrayMgr->showAll();
	sound.Stage1_BGM_Stop();
	sound.PlayGameclear_Sound();
	inventory_overlay->hide();
	background_overlay->hide();
	GameClearbackground_overlay->show();
	GameOverbackground_overlay->hide();
	isGameClearScene = true;
	isMainScene = false;
}

void TutorialApplication::GameOver()
{
	if(!(OgreBites::Button*)gameOverTrayMgr->getWidget("GameOverReStartButton"))
		gameClearReStartButton = gameOverTrayMgr->createButton(OgreBites::TrayLocation::TL_BOTTOM,"GameOverReStartButton","ReStart!");

	setupViewport(GameOverSceneMgr);
	mTrayMgr->hideAll();
	leftTimePanel->hide();
	CropNumberPanel->hide();
	WaterNumberPanel->hide();
	itemTrayMgr->hideAll();

	gameOverTrayMgr->showAll();
	gameclearTrayMgr->hideAll();
	sound.Stage1_BGM_Stop();
	sound.PlayGameover_Sound();
	inventory_overlay->hide();
	background_overlay->hide();
	GameOverbackground_overlay->show();
	GameClearbackground_overlay->hide();
	isGameOverScene = true;
	isMainScene = false;
}

bool TutorialApplication::setOverlayCaption(const Ogre::String &elementName, const Ogre::String &caption)
{
	 
	 Ogre::OverlayElement* elm =Ogre::OverlayManager::getSingleton().getOverlayElement(elementName);
	 
	if(!elm) return false;
	elm->setPosition(63,73);
	elm->setCaption(caption);
	return true;
}

bool TutorialApplication::showOverlay(const Ogre::String &overlayName)
{ 
	
 // Load up our UI and display it
 textOverlay = (Ogre::Overlay*)Ogre::OverlayManager::getSingleton().getByName(overlayName);  
 
 if (!textOverlay) return false;

 return true;	
}

bool TutorialApplication::keyPressed( const OIS::KeyEvent &kev )
{
	if(mTrayMgr->isDialogVisible()) return true;

	//�κ��丮
	if(kev.key == OIS::KC_I)
	{
		isOpenInventory = !isOpenInventory;
		if(isOpenInventory)
		{
			inventory_overlay->show();
			//ItemCheckWater();
			itemTrayMgr->showAll();
			mTrayMgr->hideCursor();
			//mTrayMgr->hideAll

		}
		else
		{
			inventory_overlay->hide();
			itemTrayMgr->hideAll();
			mTrayMgr->showCursor();

		}
	}
	//�÷��̾� �̵�
	if(isCameraLookAtPlayer)
	{
		if (kev.key == OIS::KC_W)
		{
			isPlayerMove=true;
			isPlayerMoveUp = true;
			playerAnimationState1->setEnabled(false);
			playerAnimationState2->setEnabled(true);
			playerAnimationState3->setEnabled(false);
		}
		else if (kev.key ==OIS::KC_S)
		{
			isPlayerMove = true;
			isPlayerMoveDown = true;
			playerAnimationState1->setEnabled(false);
			playerAnimationState2->setEnabled(true);
			playerAnimationState3->setEnabled(false);
		}
		else if (kev.key == OIS::KC_A)
		{
			isPlayerMove = true;
			isPlayerMoveLeft = true;
			playerAnimationState1->setEnabled(false);
			playerAnimationState2->setEnabled(true);
			playerAnimationState3->setEnabled(false);
		}
		else if (kev.key == OIS::KC_D)
		{
			isPlayerMove = true;
			isPlayerMoveRight = true;
			playerAnimationState1->setEnabled(false);
			playerAnimationState2->setEnabled(true);
			playerAnimationState3->setEnabled(false);
		}
	}

	//���ѽɱ�
	if(kev.key == OIS::KC_J)
	{
		CheckPlantSeed();
	}
	//���ֱ�
	if(kev.key == OIS::KC_K)
	{
		waterCanUseNumber--;
		CheckPlantWater();
		sound.PlayWaterSpray_Sound();
		playerAnimationState3->setEnabled(true);
		plantWaterTimer.reset();
		if(plantWaterTimer.getMilliseconds()>1500)
			playerAnimationState3->setEnabled(false);
		//matureCropNumber++;
	}

	//ī�޶��� [�÷��̾����(true)/�����������(false)]
	if(kev.key == OIS::KC_1)
		isCameraLookAtPlayer =!isCameraLookAtPlayer;

	//ī�޶� ����(������)
	if (kev.key == OIS::KC_H)   // toggle visibility of even rarer debugging details
	{
		if (mDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
		{
			mTrayMgr->moveWidgetToTray(mDetailsPanel, OgreBites::TL_TOPRIGHT, 0);
			mDetailsPanel->show();
		}
		else
		{
			mTrayMgr->removeWidgetFromTray(mDetailsPanel);
			mDetailsPanel->hide();
		}
	}

	//esc - ����
	if (kev.key == OIS::KC_ESCAPE)
	{
		mShutDown = true;
	}

	//�⺻���� ī�޶� ����
	if(!isCameraLookAtPlayer)
		mCameraMan->injectKeyDown(kev);

	return true;
}

bool TutorialApplication::keyReleased(const OIS::KeyEvent &kev)
{
	if(!isCameraLookAtPlayer)
		mCameraMan->injectKeyUp(kev);


	//�÷��̾� �̵�
	if(isCameraLookAtPlayer)
	{
		if (kev.key == OIS::KC_W)
		{
			isPlayerMove = false;
			isPlayerMoveUp = false;
			playerAnimationState1->setEnabled(true);
			playerAnimationState2->setEnabled(false);
			sound.Walk_Sound_Stop();
		}
		else if (kev.key ==OIS::KC_S)
		{
			isPlayerMove = false;
			isPlayerMoveDown = false;
			playerAnimationState1->setEnabled(true);
			playerAnimationState2->setEnabled(false);
			sound.Walk_Sound_Stop();
		}
		else if (kev.key == OIS::KC_A)
		{
			isPlayerMove = false;
			isPlayerMoveLeft = false;
			playerAnimationState1->setEnabled(true);
			playerAnimationState2->setEnabled(false);
			sound.Walk_Sound_Stop();
		}
		else if (kev.key == OIS::KC_D)
		{
			isPlayerMove = false;
			isPlayerMoveRight = false;
			playerAnimationState1->setEnabled(true);
			playerAnimationState2->setEnabled(false);
			sound.Walk_Sound_Stop();
		}
	}

	return true;
}

bool TutorialApplication::mouseMoved(const OIS::MouseEvent &arg)
{
	if (mTrayMgr->injectMouseMove(arg)) 
		return true;
	if(itemTrayMgr->injectMouseMove(arg))
		return true;

	if(titleTrayMgr->injectMouseMove(arg))
		return true;
	if(gameOverTrayMgr->injectMouseMove(arg))
		return true;
	if(gameclearTrayMgr->injectMouseMove(arg))
		return true;

	if(!isCameraLookAtPlayer)
		mCameraMan->injectMouseMove(arg);
	else
		player->yaw(Ogre::Degree(-arg.state.X.rel * 0.05f));

	return true;
}
//---------------------------------------------------------------------------
bool TutorialApplication::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (mTrayMgr->injectMouseDown(arg, id)) return true;
	if (titleTrayMgr->injectMouseDown(arg, id))return true;
	if(itemTrayMgr->injectMouseDown(arg,id))return true;
	if(gameOverTrayMgr->injectMouseDown(arg,id))return true;
	if(gameclearTrayMgr->injectMouseDown(arg,id))return true;
	if(!isCameraLookAtPlayer)
		mCameraMan->injectMouseDown(arg, id);
	return true;
}
//---------------------------------------------------------------------------
bool TutorialApplication::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (mTrayMgr->injectMouseUp(arg, id)) return true;
	if (titleTrayMgr->injectMouseUp(arg, id))return true;
	if(itemTrayMgr->injectMouseUp(arg,id))return true;
	if(gameOverTrayMgr->injectMouseUp(arg,id))return true;
	if(gameclearTrayMgr->injectMouseUp(arg,id))return true;
	if(!isCameraLookAtPlayer)
		mCameraMan->injectMouseUp(arg, id);
	return true;
}

void TutorialApplication::windowResized(Ogre::RenderWindow* rw)
{
	int left, top;
	unsigned int width, height, depth;

	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState& ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

void TutorialApplication::windowClosed(Ogre::RenderWindow* rw)
{
	if(rw == mWindow)
	{
		if(mInputMgr)
		{
			mInputMgr->destroyInputObject(mMouse);
			mInputMgr->destroyInputObject(mKeyboard);

			OIS::InputManager::destroyInputSystem(mInputMgr);
			mInputMgr = 0;
		}
	}
}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
	int main(int argc, char *argv[])
#endif
	{
		TutorialApplication app;

		try
		{
			app.go();
		}
		catch(Ogre::Exception& e)
		{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			MessageBox(
				NULL,
				e.getFullDescription().c_str(),
				"An exception has occured!",
				MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
			std::cerr << "An exception has occured: " <<
				e.getFullDescription().c_str() << std::endl;
#endif
		}

		return 0;
	}
#ifdef __cplusplus
}
#endif