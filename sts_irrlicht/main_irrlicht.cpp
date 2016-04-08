#include <irrlicht.h>
#include <driverChoice.h>

#include <iostream>

int main(int argc, char* argv[])
{
	irr::video::E_DRIVER_TYPE driverType = irr::driverChoiceConsole();
	irr::IrrlichtDevice* device = irr::createDevice(driverType, irr::core::dimension2d<irr::u32>(800, 600), 16, false, false, false, nullptr);
	if (!device) {
		std::cerr << "Device is not created." << std::endl;
		return 1;
	}

	device->setWindowCaption(L"STS Test");
	irr::video::IVideoDriver* driver = device->getVideoDriver();
	irr::scene::ISceneManager* smgr = device->getSceneManager();
	irr::gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

	smgr->addCameraSceneNode(0, irr::core::vector3df(0, 30, -40), irr::core::vector3df(0, 5, 0));

	while (device->run()) {
		driver->beginScene(true, true, irr::video::SColor(255, 100, 101, 140));

		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();
	}

	device->drop();

	return 0;
}