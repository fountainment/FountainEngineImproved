#include "TestScene.h"

using namespace fei;

void TestScene::init()
{
	pitch = 1.0f;

	setClock(&mainClock);

	mainCam.setCameraSize(Vec2(800, 600));
	mainCam.setCameraScale(2.0f);
	setCamera(&mainCam);

	map.load("map.png");

	UI.load("UI43.png");
	UI.setIsAlpha(true);

	UICam.setCameraSize(Vec2(1920, 1440));
	UILayer.setCamera(&UICam);
	UILayer.setIsAlpha(true);
	UILayer.add(&UI);

	UI.setColor(Color(Vec3(1.0f), 0.8f));

	audio.loadWAV("test.wav");
	audio.setLoop(true);
	audio.play();

	add(&map);
	add(&mainChar);
	add(&UILayer);

	body = Physics::getInstance()->createBody(fei::Vec2::ZERO);
}

void TestScene::destroy()
{
	Physics::getInstance()->destroyBody(body);
}

void TestScene::update()
{
	auto joystick = Control::getInstance()->getJoystick();
	auto window = Interface::getInstance()->getCurrentWindow();
	if (joystick) {
		auto speed = joystick->getAxes();
		float sp = speed.getLengthSq();
		if (sp > 0.49f) {
			mainChar.setSpeed(speed, 200.0f);
		} else if (sp > 0.0001f) {
			mainChar.setSpeed(speed, 100.0f);
		} else {
			mainChar.setSpeed(Vec2::ZERO);
		}
		if (joystick->getDirectionY() > 0.0f) {
			pitch *= 1.01f;
			audio.setPitch(pitch);
		}
		if (joystick->getDirectionY() < 0.0f) {
			pitch *= 0.99f;
			audio.setPitch(pitch);
		}
	} else if (window) {
		Vec2 speed(Vec2::ZERO);
		if (window->getKey(GLFW_KEY_UP)) {
			speed.add(Vec2::UP);
		}
		if (window->getKey(GLFW_KEY_LEFT)) {
			speed.add(Vec2::LEFT);
		}
		if (window->getKey(GLFW_KEY_DOWN)) {
			speed.add(Vec2::DOWN);
		}
		if (window->getKey(GLFW_KEY_RIGHT)) {
			speed.add(Vec2::RIGHT);
		}
		if (window->getKey(GLFW_KEY_W)) {
			pitch *= 1.01f;
			audio.setPitch(pitch);
		}
		if (window->getKey(GLFW_KEY_S)) {
			pitch *= 0.99f;
			audio.setPitch(pitch);
		}
		mainChar.setSpeed(speed, 100.0f);
	}
	mainCam.setPosition(mainChar.getPosition());
	auto bdv = body->getPosition();
	std::printf("%.3f %.3f\n", bdv.x, bdv.y);
}
