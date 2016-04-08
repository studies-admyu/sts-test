#include <exception>
#include <memory>

#include <vector>

#include <OGRE/OgreRoot.h>
#include <OGRE/OgreRenderSystem.h>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreWindowEventUtilities.h>

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

int main(int argc, char* argv[])
{
	try {
		Ogre::String lConfigFileName = "";
		Ogre::String lPluginsFileName = "";
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

		while (!lWindow->isClosed()) {
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
