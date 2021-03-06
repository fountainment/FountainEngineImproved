#include "TestScene.h"

#include "base/stringUtil.h"

using namespace fei;

//TODO: add Debug module, write some macro utilities

void TestScene::init()
{
	setCamera(&mainCam);
	_needInitUILayout = false;

	initUILayout();

	testMath();
	testPhysics();
	testRender();
	testAudio();
	testFont();
	testUtil();
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
        if (_needInitUILayout) {
		initUILayout();
		_needInitUILayout = false;
	}
}

void TestScene::beforeDraw()
{
	_renderTarget.bind();
	Render::getInstance()->clearBuffer();
}

void TestScene::afterDraw()
{
	_renderTarget.unbind();
	auto image = _renderTarget.getTexture()->getImage();
	auto camera = Render::getInstance()->getCurrentCamera();
	auto rct = camera->getCameraRect();
	Color::White.use();
	image.drawRect(rct);
}

void TestScene::testMath()
{
	Vec2 a(0.0f);
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

	auto p = fei::collideLine(Vec2(0.0, 1.0), Vec2(0.0, 2.0), Vec2(1.0, 0.0), Vec2(2.0, 0.0));
	std::printf("(0.0, 0.0) == (%f, %f)\n", p.x, p.y);
	if (p == Vec2::ZERO) {
		std::printf("Yes\n");
	} else {
		std::printf("No\n");
	}
	p = fei::collideLine(Vec2(0.0, 1.0), Vec2(0.0, 2.0), Vec2(2.0, 0.0), Vec2(1.0, 0.0));
	std::printf("(0.0, 0.0) == (%f, %f)\n", p.x, p.y);
	if (p == Vec2::ZERO) {
		std::printf("Yes\n");
	} else {
		std::printf("No\n");
	}

	Segment segA(Vec2::ZERO, Vec2(10.0f));
	Segment segB(Vec2(0.0f, 30.0f), Vec2(20.0f, -1.0f));
	Segment segC(Vec2(1.0f, 31.0f), Vec2(21.0f, 0.0f));
	Segment segD(Vec2::ZERO, Vec2::ONE);
	Segment segE(Vec2(0.0f, 1.0f), Vec2::ONE);
	Segment segF(Vec2(0.0f, 10.0f), Vec2(1.0f, 10.0f));
	Segment segG(Vec2(2.0f, 10.0f), Vec2(3.0f, 10.0f));

	std::printf("SegCmp: %d\n", segA.cmpRenderY(segB));
	std::printf("SegCmp: %d\n", segB.cmpRenderY(segA));
	std::printf("SegCmp: %d\n", segC.cmpRenderY(segD));
	std::printf("SegCmp: %d\n", segD.cmpRenderY(segC));
	std::printf("SegCmp: %d\n", segE.cmpRenderY(segF));
	std::printf("SegCmp: %d\n", segF.cmpRenderY(segE));
	std::printf("SegCmp: %d\n", segF.cmpRenderY(segG));
}

void TestScene::keyCallback(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
		auto window = fei::Interface::getInstance()->getCurrentWindow();
		window->setFullscreen(!window->isFullscreen());
	}
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
		comLabel.executeCommand();
	}
	if (key == GLFW_KEY_BACKSPACE && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		comLabel.deleteChar();
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

void TestScene::characterCallback(unsigned int codepoint)
{
	comLabel.inputChar(codepoint);
}

void TestScene::framebufferSizeCallback(int width, int height)
{
	_needInitUILayout = true;
}

void TestScene::testPhysics()
{
}

void TestScene::testRender()
{
	int s = Render::getInstance()->getMaxTextureSize();
	std::printf("%d\n", s);
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
	testTex.setPosition((testTex.getSize() - winSize) * 0.5f);
	testTex.setColor(Color("#3f0"));
	testTex.setMagFilter(GL_LINEAR);
	add(&testTex);

	label.setString(fc, "Hello World! 我能吞下玻璃而不伤身体！");
	add(&label);

	button.setRectSize(Vec2(100.0f, 20.0f));
	button.setLabelString(fc, "Test!");
	add(&button);
}

void TestScene::testAudio()
{
	src.loadWAV("res/sound/test.wav");
	src.play();
}

void TestScene::testUtil()
{
	auto result = fei::strSplit("This is  a   split test! ");
	for (auto str : result) {
		std::printf(">>%s<<\n", str.c_str());
	}
	comLabel.setPosition(fei::Vec2(0.0f, 100.0f));
	comLabel.setFontCache(&fc);
	comLabel.getInterpreter()->registerCommand({":print"}, [](std::vector<std::string> params){if (params.size() >= 1) {std::printf("%s\n", params[0].c_str());} return fut::CommandResult::Ok;});
	add(&comLabel);
}

void TestScene::initUILayout()
{
	auto window = fei::Interface::getInstance()->getCurrentWindow();
	auto winSize = window->getFrameSize();
	std::printf("%f %f\n", winSize.x, winSize.y);
	fei::Render::getInstance()->setViewport(winSize);
	mainCam.setCameraSize(winSize);
	_renderTarget.setSize(winSize, Texture::Format::RGBAF);
	_renderTarget.getTexture()->setHasAlpha(true);
	_renderTarget.getTexture()->setMagFilter(GL_NEAREST);
	testTex.setPosition((testTex.getSize() - winSize) * 0.5f);
}
