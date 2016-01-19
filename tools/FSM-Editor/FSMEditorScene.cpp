#include "FSMEditorScene.h"

#include "FSMEditor.h"

using namespace fei;

static FSMEditorScene* scene;
static fut::FSM* fsm;

ButtonBase::ButtonBase()
{
	setFrontColor(FSMEditor::brightColor);
	setBackColor(FSMEditor::darkColor);
}

void ButtonBase::init()
{
	setRectSize(Vec2(getLabel()->getLength() + 30.0f, 50.0f));
	getLabel()->setCenterAligned(true);
	getLabel()->setPosition(getRectSize() / 2.0f);
	getLabel()->move(Vec2(0.0f, -14.0f));
}

void ButtonBase::onEnter()
{
	setBackColor(FSMEditor::midColor);
}

void ButtonBase::onLeave()
{
	setBackColor(FSMEditor::darkColor);
}

void ButtonBase::onMouseDown()
{
	setBackColor(FSMEditor::lightColor);
}

void ButtonBase::onMouseUp()
{
	setBackColor(FSMEditor::midColor);
}

SignalButton::SignalButton(int sig)
{
	_sig = sig;
}

void SignalButton::onClick()
{
	if (_sig == -1) {
		if (scene->_tmpName != "") {
			fsm->registerSignal(scene->_tmpName);
			scene->_tmpName = "";
			scene->_tmpLabel.clearString();
			scene->updateSignalList();
		}
	} else {
		scene->setSignal(_sig);
	}
}

void SignalButton::update()
{
}

StateButton::StateButton(int state)
{
	_state = state;
	setFrontColor(FSMEditor::lighterColor);
	setBackColor(FSMEditor::darkColor);
}

void StateButton::onClick()
{
	if (_state == -1) {
		if (scene->_tmpName != "") {
			fsm->registerState(scene->_tmpName);
			scene->_tmpName = "";
			scene->_tmpLabel.clearString();
			scene->updateFSM();
		}
	} else {
		scene->setState(_state);
	}
}

void StateButton::update()
{
}

void FSMEditorScene::init()
{
	Color("#111").setClearColor();

	auto win = Interface::getInstance()->getCurrentWindow();
	auto winS = win->getFrameSize();

	scene = this;
	fsm = &_fsm;

	add(&_fsmLayer);
	add(&_signalListLayer);
	add(&_helpLayer);

	Vec2 startPosition = Interface::getInstance()->getWindowSize();
	startPosition.zoom(Vec2(0.5f, -0.5f));
	auto button = new StateButton(-1);
	button->setLabelString(FSMEditor::font, "+");
	button->feiInit();
	button->setPosition(startPosition + Vec2(-button->getRectSize().x - 1.0f, 1.0f));
	_fsmLayer.add(button);
	updateSignalList();

	_mainCam.setCameraSize(winS);
	setCamera(&_mainCam);

	_rect.setSize(Vec2(100.0f));
	_rectObj.setShape(&_rect);
	_helpLayer.add(&_rectObj);

	_tmpLabel.setPosition(Interface::getInstance()->getWindowSize() * -0.5f + Vec2(20.0f));
	add(&_tmpLabel);
}

void FSMEditorScene::update()
{
	auto win = Interface::getInstance()->getCurrentWindow();
	if (win) {
		if (win->getKey(GLFW_KEY_H)) {
			_helpLayer.setVisible(true);
		} else {
			_helpLayer.setVisible(false);
		}
	}
}

void FSMEditorScene::updateSignalList()
{
	//clear signalListLayer
	auto lv = _signalListLayer.getListVector();
	for (auto i : lv) {
		throwAway(i);
	}
	_signalListLayer.clear();

	auto signalList =_fsm.getSignalVector();
	Vec2 startPosition = Interface::getInstance()->getWindowSize();
	startPosition.zoom(Vec2(-0.5f, 0.5f));
	startPosition.add(Vec2(1.0f, -50.0f - 1.0f));
	for (auto& signal : signalList) {
		auto button = new SignalButton(signal.first);
		button->setLabelString(FSMEditor::font, signal.second);
		button->feiInit();
		button->setPosition(startPosition);
		startPosition.add(Vec2(button->getRectSize().x + 1.0f, 0.0f));
		_signalListLayer.add(button);
	}
	auto button = new SignalButton(-1);
	button->setLabelString(FSMEditor::font, "+");
	button->feiInit();
	button->setPosition(startPosition);
	_signalListLayer.add(button);
}

void FSMEditorScene::updateFSM()
{
	//clear fsmLayer
	auto lv = _fsmLayer.getListVector();
	for (auto i : lv) {
		throwAway(i);
	}
	_fsmLayer.clear();

	//TODO:
	//  memorize StateButton's position
	//  add all relations to fsm
	//  add save button saving fsm to text file
}

void FSMEditorScene::setSignal(int sig)
{
	std::printf("Signal: %d\n", sig);
	//TODO:
	//  memorize the select signal
	//  highlight the select SignalButton
}

void FSMEditorScene::setState(int state)
{
	std::printf("State: %d\n", state);
	//TODO:
	//  implement UI component draging logic
}

void FSMEditorScene::charactorCallback(unsigned int codepoint)
{
	char inputChar = (char)codepoint;
	if (inputChar != ' ') {
		_tmpName += inputChar;
		_tmpLabel.setString(FSMEditor::font, _tmpName);
	}
}

void FSMEditorScene::keyCallback(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS) {
		if (!_tmpName.empty()) {
			_tmpName.pop_back();
			_tmpLabel.setString(FSMEditor::font, _tmpName);
		}
	}
}
