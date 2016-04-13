#include <exception>
#include <memory>

#include <vector>

#include <OGRE/OgreRoot.h>
#include <OGRE/OgreRenderSystem.h>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreWindowEventUtilities.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreBillboardSet.h>
#include <OGRE/OgreBillboard.h>

typedef std::vector<Ogre::String> Strings;

void loadPlugins(std::unique_ptr<Ogre::Root>& root)
{
	Strings lPluginNames;
	lPluginNames.push_back("RenderSystem_GL");
	lPluginNames.push_back("Plugin_ParticleFX");
	lPluginNames.push_back("Plugin_CgProgramManager");
	lPluginNames.push_back("Plugin_OctreeSceneManager");

	{
		Strings::iterator lIter = lPluginNames.begin();
		for (; lIter != lPluginNames.end(); ++lIter) {
			Ogre::String& lPluginName = (*lIter);
			bool lIsInDebugMode = OGRE_DEBUG_MODE;
			if (lIsInDebugMode) {
				lPluginName.append("_d");
			}
			root->loadPlugin(lPluginName);
		}
	}
}

void initRenderSystem(std::unique_ptr<Ogre::Root>& root)
{
	const Ogre::RenderSystemList& lRenderSystemList = root->getAvailableRenderers();
	if (lRenderSystemList.size() == 0) {
		Ogre::LogManager::getSingleton().logMessage("No render system was found.");
		throw Ogre::Exception(1, "Render system was not initialized.", "main.cpp");
	}
	Ogre::RenderSystem* lRenderSystem = lRenderSystemList[0];
	root->setRenderSystem(lRenderSystem);
}

void initResourceGroup(const Ogre::String& groupName, const Ogre::String& fsPath)
{
	Ogre::ResourceGroupManager& lRcGroupMgr = Ogre::ResourceGroupManager::getSingleton();
	lRcGroupMgr.createResourceGroup(groupName);
	lRcGroupMgr.addResourceLocation(fsPath, "FileSystem", groupName, false);
	lRcGroupMgr.initialiseResourceGroup(groupName);
	lRcGroupMgr.loadResourceGroup(groupName);
}

int main(int argc, char* argv[])
{
	try {
		Ogre::String lConfigFileName = "ogre.cfg";
		Ogre::String lPluginsFileName = "plugins.cfg";
		Ogre::String lLogFileName = "Ogre_STS.log";

		std::unique_ptr<Ogre::Root> lRoot(new Ogre::Root(lPluginsFileName, lConfigFileName, lLogFileName));

		loadPlugins(lRoot);
		initRenderSystem(lRoot);

		bool lCreateAWindowAutomatically = false;
		Ogre::String lWindowTitle = "STS Test";
		Ogre::String lCustomCapacities = "";
		lRoot->initialise(lCreateAWindowAutomatically, lWindowTitle, lCustomCapacities);

		Ogre::RenderWindow* lWindow = nullptr;
		{
			unsigned int lSizeX = 800;
			unsigned int lSizeY = 600;
			bool lFullscreen = false;

			Ogre::NameValuePairList lParams;
			lParams["FSAA"] = "0";
			lParams["vsync"] = "true";
			lWindow = lRoot->createRenderWindow(lWindowTitle, lSizeX, lSizeY, lFullscreen, &lParams);
		}

		/* Create a scene manager */
		Ogre::SceneManager* lScene = lRoot->createSceneManager(Ogre::ST_GENERIC, "SceneManager");

		Ogre::SceneNode* lRootSceneNode = lScene->getRootSceneNode();

		/* Create camera */
		Ogre::Camera* lCamera = lScene->createCamera("MyCamera");

		/* Create viewport (camera <-> window) */
		Ogre::Viewport* vp = lWindow->addViewport(lCamera);

		vp->setAutoUpdated(true);
		vp->setBackgroundColour(Ogre::ColourValue(1, 0, 1));

		lCamera->setAspectRatio(float(vp->getActualWidth()) / vp->getActualHeight());
		lCamera->setPosition(Ogre::Vector3(0, 100, -1));
		lCamera->lookAt(Ogre::Vector3(0, 0, 0));

		/* Set clipping*/
		lCamera->setNearClipDistance(1.5f);
		lCamera->setFarClipDistance(3000.0f);

		/* Lighting */
		Ogre::Light* lLight = lScene->createLight("MainLight");
		lLight->setPosition(Ogre::Vector3(0, 100, 0));

		/* Resource manager */
		Ogre::String lRcGroupName = "Test group";
		Ogre::String lRcGroupPath = "media";
		initResourceGroup(lRcGroupName, lRcGroupPath);

		/* Load model */
		Ogre::Entity* lShipEntity = lScene->createEntity("airship.mesh");
		lShipEntity->setCastShadows(false);

		Ogre::SceneNode* lShipNode = lRootSceneNode->createChildSceneNode();
		lShipNode->attachObject(lShipEntity);
		lShipNode->setScale(Ogre::Vector3(3.15f, 3.15f, 3.15f));

		/* Starship start point */
		Ogre::Vector3 razorSP(0, -200, -100);
		lShipNode->setPosition(razorSP);

		/* Sprite billboard */
		Ogre::SceneNode* lSpriteNode = lRootSceneNode->createChildSceneNode();
		Ogre::BillboardSet* lBillboardSet = lScene->createBillboardSet();
		lBillboardSet->setMaterialName("ship1", lRcGroupName);
		lBillboardSet->setTextureStacksAndSlices(1, 4);
		Ogre::Billboard* lSpriteBillboard = lBillboardSet->createBillboard(Ogre::Vector3(0, 0, 0));
		lSpriteBillboard->setDimensions(48.0f / 2.0f, 58.0f / 2.0f);
		lSpriteBillboard->setTexcoordIndex(1);
		lSpriteNode->attachObject(lBillboardSet);
		lSpriteNode->setPosition(Ogre::Vector3(0, -200, 100));

		/* Obtain the timer pointer */
		Ogre::Timer* lTimer = lRoot->getTimer();

		/* Skip all the messages */
		lWindow->setAutoUpdated(false);
		lRoot->clearEventTimes();

		while (!lWindow->isClosed()) {
			float angle = Ogre::Math::Sin(float(lTimer->getMilliseconds()) * Ogre::Math::PI / 2000.0f) * Ogre::Math::PI / 4.0f;
			float diplacement = Ogre::Math::Cos(float(lTimer->getMilliseconds()) * Ogre::Math::PI / 2000.0f) * 100.0f;
			lShipNode->setOrientation(Ogre::Quaternion(Ogre::Radian(angle), Ogre::Vector3(0, 0, 1)));
			lShipNode->setPosition(razorSP + Ogre::Vector3(diplacement, 0.0f, 0.0f));

			unsigned int spriteFrame = (lTimer->getMilliseconds() / 125) % 2;
			lSpriteBillboard->setTexcoordIndex(spriteFrame);

			lWindow->update(false);
			lWindow->swapBuffers();
			lRoot->renderOneFrame();

			Ogre::WindowEventUtilities::messagePump();
		}
		Ogre::LogManager::getSingleton().logMessage("Render window closed.");
	}
	catch (Ogre::Exception &e) {
		std::cerr << "Ogre::Exception: " << e.what() << std::endl;
	}
	catch (std::exception &e) {
		std::cerr << "std::exception: " << e.what() << std::endl;
	}

	return 0;
}
