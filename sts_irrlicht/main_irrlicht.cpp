#include <irrlicht.h>
#include <driverChoice.h>
#include <cmath>

#include <iostream>

int main(int argc, char* argv[])
{
	irr::video::E_DRIVER_TYPE driverType = irr::driverChoiceConsole();
	irr::IrrlichtDevice* device = irr::createDevice(driverType, irr::core::dimension2d<irr::u32>(800, 600), 16, false, false, false, nullptr);
	if (!device) {
		std::cerr << "Device is not created." << std::endl;
		return 1;
	}

	device->setWindowCaption(L"CTS Test");
	irr::video::IVideoDriver* driver = device->getVideoDriver();
	irr::scene::ISceneManager* smgr = device->getSceneManager();
	irr::gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

	irr::scene::IAnimatedMesh* mesh = smgr->getMesh("media/airship.x");
	if (!mesh) {
		std::cerr << "Mesh is not loaded." << std::endl;
		device->drop();
		return 1;
	}

	irr::core::vector3df shipSP(0, -200, -100);
	irr::scene::IMeshSceneNode* node = smgr->addMeshSceneNode(mesh);
	if (node) {
		node->setScale(irr::core::vector3df(6.3, 6.3, 6.3));
		node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		node->setMaterialTexture(0, driver->getTexture("media/all_diffuse.jpg"));
		node->setPosition(shipSP);
	}

	smgr->addCameraSceneNode(0, irr::core::vector3df(0, 100, -1), irr::core::vector3df(0, 0, 0));

	irr::video::ITexture* images = driver->getTexture("media/ship1.png");

	irr::scene::IBillboardSceneNode* billboard = smgr->addBillboardSceneNode(nullptr, irr::core::dimension2d<irr::f32>(48.0f, 58.0f));
	if (billboard) {
		billboard->setMaterialTexture(0, images);
		billboard->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		billboard->setMaterialType(irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL);
		billboard->setPosition(irr::core::vector3df(0, -200, 100));

		billboard->getMaterial(0).getTextureMatrix(0).setTextureScale(0.25, 1.0);
	}

	irr::ITimer* timer = device->getTimer();

	while (device->run()) {
		driver->beginScene(true, true, irr::video::SColor(255, 255, 0, 255));

		irr::u32 currentTime = timer->getTime();
		float angle = sin(float(currentTime) * irr::core::PI / 2000.0f) * 45.0f;
		float diplacement = cos(float(currentTime) * irr::core::PI / 2000.0f) * 100.0f;
		node->setRotation(irr::core::vector3df(0.0f, 0.0f, angle));
		node->setPosition(shipSP + irr::core::vector3df(diplacement, 0.0f, 0.0f));

		unsigned int currentFrame = (currentTime / 125) % 2;

		billboard->getMaterial(0).getTextureMatrix(0).setTextureTranslate(0.25 * currentFrame, 0.0);

		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();
	}

	device->drop();

	return 0;
}
