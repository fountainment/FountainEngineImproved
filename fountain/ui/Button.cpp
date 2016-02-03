#include "ui/Button.h"

#include "interface/Interface.h"
#include "render/Render.h"

using fei::Button;

Button::Button()
: _drawCamera(nullptr),
  _backRect(Rect(fei::Vec2::ZERO, fei::Vec2(10.0f))),
  _backColor(fei::Color::White),
  _frontColor(fei::Color::Black),
  _collide(false),
  _mouseDown(false),
  _state(0)
{
	setHasAlpha(true);
}

void Button::drawIt()
{
	_drawCamera = fei::Render::getInstance()->getCurrentCamera();

	_backColor.use();
	fei::Render::drawShape(&_backRect);
	if (!_backImage.empty()) {
		_backImage.drawRect(_backRect);
	}

	_frontColor.use();
	glTranslatef(_label.getPosition().x, _label.getPosition().y, 0.0f);
	_label.drawIt();
}

void Button::feiUpdate()
{
	if (_drawCamera) {
		fei::Vec2 cPos = fei::Interface::getInstance()->getRHCursorPos();
		cPos = _drawCamera->screenToWorld(cPos);
		bool collide = _backRect.collidePoint(cPos - getPosition());
		bool mouseDown = fei::Interface::getInstance()->getCurrentWindow()->getMouseButton(GLFW_MOUSE_BUTTON_LEFT) != 0;
		if (collide != _collide) {
			_collide = collide;
			if (_collide) {
				onEnter();
				if (_mouseDown) {
					onMouseDown();
				}
			} else {
				if (_state == 1) {
					onButtonUp();
				}
				onLeave();
			}
		}
		if (_collide) {
			onCollide();
		}
		if (mouseDown != _mouseDown) {
			_mouseDown = mouseDown;
			if (_collide) {
				if (_mouseDown) {
					onMouseDown();
					onButtonDown();
					_state = 1;
				} else {
					if (_state == 1) {
						click();
						onButtonUp();
						_state = 0;
					}
					onMouseUp();
				}
			} else {
				_state = 0;
			}
		}
	}
	update();
}

void Button::setLabel(const fei::Label& label)
{
	_label = label;
}

fei::Label* Button::getLabel()
{
	return &_label;
}

void Button::setRectSize(const fei::Vec2& rectSize)
{
	_backRect.setSize(rectSize);
}

const fei::Vec2 Button::getRectSize()
{
	return _backRect.getSize();
}

void Button::setBackColor(const fei::Color& color)
{
	_backColor = color;
}

void Button::setFrontColor(const fei::Color& color)
{
	_frontColor = color;
}

void Button::setBackImage(const fei::Image& image)
{
	_backImage = image;
}

void Button::setLabelString(fei::FontCache& fontCache, const std::string& str)
{
	_label.setString(fontCache, str);
}

void Button::setLabelCenterAligned(bool b)
{
	_label.setCenterAligned(b);
}

const fei::Vec2 Button::getCenter()
{
	return getPosition() + _backRect.getCenter();
}

void Button::setCenter(const fei::Vec2& v)
{
	setPosition(v - _backRect.getSize() / 2.f);
}

bool Button::isCollide()
{
	return _collide;
}

void Button::click()
{
	onClick();
}

void Button::onEnter()
{
}

void Button::onLeave()
{
}

void Button::onCollide()
{
}

void Button::onMouseDown()
{
}

void Button::onButtonDown()
{
}

void Button::onClick()
{
}

void Button::onButtonUp()
{
}

void Button::onMouseUp()
{
}
