#include "TestScene.h"

using namespace fei;

void TestScene::init()
{
	setCamera(&mainCam);
	mainCam.setCameraSize(Vec2(800, 600));

	testMath();
	testPhysics();
	testRender();
	testAudio();
	testFont();
}

void TestScene::destroy()
{
}

void TestScene::update()
{
	auto window = Interface::getInstance()->getCurrentWindow();
	static Vec2 oldPos = window->getRHCursorPos();
	Vec2 deltaV = (window->getRHCursorPos() - oldPos) / -mainCam.getCameraScale();
	oldPos = window->getRHCursorPos();
	if (window->getMouseButton(GLFW_MOUSE_BUTTON_MIDDLE)) {
		mainCam.move(deltaV);
	}
}

void TestScene::testMath()
{
	Vec2 a;
	if (a == Vec2::ZERO) {
		std::printf("Vec2: 1ok!\n");
	}
	a.set(1.5f, 1.5f);
	auto b = a;
	a.mul(10.0f);
	a/=10.0f;
	if (a == b) {
		std::printf("Vec2: 2ok!\n");
	}
	a.add(a);
	if (a == Vec2(3.0f)) {
		std::printf("Vec2: 3ok!\n");
	}
	a = Vec2(10.0f);
	a += a + a * 3.0f / 100.0f;
	if (a == Vec2(20.3f)) {
		std::printf("Vec2: 4ok!\n");
	}
	a.normalize();

	Segment x(Vec2::ZERO, Vec2(10.0f));
	Vec2 tmp;
	if (x.collideRay(tmp, Vec2(10.0f, 0.0f), Vec2(-10.0f, 10.0f))) {
		if (tmp == Vec2(5.0f)) {
			std::printf("Segment: 1ok!\n");
		}
	}

	Polygon y;
	y.pushVertex(Vec2(98.999977f, 76.000000f));
	y.pushVertex(Vec2(209.999985f, 6.999993f));
	y.pushVertex(Vec2(141.421356f, 141.421356f));
	y.pushVertex(Vec2(-141.421356f, 141.421356f));
	y.pushVertex(Vec2(141.421326f, -141.421371f));
	y.pushVertex(Vec2(-141.421326f, -141.421371f));

	auto tl = y.getVisibleVerticesIndex(1);
	std::printf("%lu\n", tl.size());
	for (int vertex : tl) {
		std::printf("%d\n", vertex);
	}
	std::printf("%d\n", y.getBestCutVertexIndex(1));
}

void TestScene::keyCallback(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
		auto window = fei::Interface::getInstance()->getCurrentWindow();
		window->setFullscreen(!window->isFullscreen());
		auto winSize = window->getWindowSize();
		std::printf("%f %f\n", winSize.x, winSize.y);
		mainCam.setCameraSize(winSize);
		testTex.setPosition(Vec2(2048) - winSize / 2.0f);
	}
}

void TestScene::scrollCallback(double xoffset, double yoffset)
{
	if (yoffset > 0.0) {
		mainCam.zoomCameraScale(1.1f);
	}
	if (yoffset < 0.0) {
		mainCam.zoomCameraScale(0.90909f);
	}
}

void TestScene::testPhysics()
{
}

void TestScene::testRender()
{
	int s = Render::getInstance()->getMaxTextureSize();
	std::printf("%d\n", s);
	normalShader.loadFile("nm.vert", "nm.frag");
	emptyTex.load(0, 500, 500);
	emptyTex.setShader(&normalShader);
	emptyTex.setZPos(100.0f);
	add(&emptyTex);
}

void TestScene::testFont()
{
	fc.loadFont("res/font/wqy.ttc", 24);
	std::vector<unsigned long> str;
	for (int i = 1; i < 255; i++) {
		str.push_back(i);
	}
	fc.updateCache(str);
	str.clear();
	for (int i = 0x4e00; i <= 0x9fa5; i++) {
		str.push_back(i);
	}
	fc.updateCache(str);

	auto window = fei::Interface::getInstance()->getCurrentWindow();
	auto winSize = window->getWindowSize();
	testTex = fc.getCacheTexture();
	testTex.setPosition(Vec2(2048) - winSize / 2.0f);
	testTex.setColor(Color("#3f0"));
	add(&testTex);

	label.setString(fc, "Hello World! 我能吞下玻璃而不伤身体！");
	add(&label);
}

void TestScene::testAudio()
{
	src.loadWAV("res/sound/test.wav");
	src.play();
}
