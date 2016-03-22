#include "control/Joystick.h"

#include <GLFW/glfw3.h>
#include "base/basedef.h"

using fei::Joystick;

Joystick::Joystick()
: id(-1),
  type(Type::DEFAULT),
  axesCount(0),
  buttonCount(0),
  accuracy(0.3f)
{
	clearData();
}

void Joystick::init()
{
	clearData();
	if (usable()) {
		const char* name = glfwGetJoystickName(id);
		std::printf("Detect Joystick: \"%s\"\n", name);
		update();
		if (axesCount == 12) {
			type = Type::PS4;
		}
	}
}

void Joystick::update()
{
	if (usable()) {
		int count;
		const float *axes = glfwGetJoystickAxes(id, &axesCount);
		count = axesCount;
		if (count > 16) count = 16;
		for (int i = 0; i < count; i++) {
			if (std::abs(axes[i]) > accuracy) {
				axesData[i] = axes[i];
			} else {
				axesData[i] = 0.0f;
			}
		}
		const unsigned char *button = glfwGetJoystickButtons(id, &buttonCount);
		count = buttonCount;
		if (count > 16) count = 16;
		for (int i = 0; i < count; i++) {
			if (buttonData[i] == 0) {
				if (button[i] == 0) {
					buttonState[i] = Joystick::ButtonState::UP;
				} else {
					buttonState[i] = Joystick::ButtonState::PRESS;
				}
			} else {
				if (button[i] == 0) {
					buttonState[i] = Joystick::ButtonState::RELEASE;
				} else {
					buttonState[i] = Joystick::ButtonState::DOWN;
				}
			}
			buttonData[i] = button[i];
		};
	}
}

void Joystick::clearData()
{
	for (int i = 0; i < 16; i++) {
		axesData[i] = 0.0f;
		buttonData[i] = 0;
		buttonState[i] = Joystick::ButtonState::UP;
	}
}

bool Joystick::usable()
{
	return id >= GLFW_JOYSTICK_1 && id <= GLFW_JOYSTICK_LAST && glfwJoystickPresent(id);
}

void Joystick::setAccuracy(float acrc)
{
	accuracy = acrc;
}

int Joystick::getAxesCount()
{
	return axesCount;
}

int Joystick::getButtonCount()
{
	return buttonCount;
}

float Joystick::getAxesX()
{
	return axesData[0];
}

float Joystick::getAxesY()
{
	return -axesData[1];
}

fei::Vec2 Joystick::getAxes()
{
	return fei::Vec2(getAxesX(), getAxesY());
}

float Joystick::getDirectionX()
{
	return axesData[6];
}

float Joystick::getDirectionY()
{
	return -axesData[7];
}

fei::Vec2 Joystick::getDirection()
{
	return fei::Vec2(getDirectionX(), getDirectionY());
}

bool Joystick::getButton(Joystick::Button button)
{
	return buttonData[(int)button] != 0;
}

Joystick::ButtonState Joystick::getButtonState(Joystick::Button button)
{
	return buttonState[(int)button];
}

float Joystick::getTouchX()
{
	return -axesData[9];
}

float Joystick::getTouchY()
{
	return axesData[10];
}

fei::Vec2 Joystick::getTouch()
{
	return fei::Vec2(getTouchX(), getTouchY());
}

float Joystick::getTouchId()
{
	return axesData[11];
}
