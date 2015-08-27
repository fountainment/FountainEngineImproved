#include "TestApplication.h"
#include "TestScene.h"

void TestApplication::engineSetting(Engine *eg)
{
	if (!eg) return;

	eg->window->setSize(800, 600);
	eg->window->setTitle("fountain-tests");
	eg->window->setResizable(false);

	eg->window->sceneManager->gotoScene(new TestScene());
}
