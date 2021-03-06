#include "render/RenderObj.h"

#include <glad/glad.h>

#include "anime/Anime.h"
#include "render/RenderList.h"

using fei::RenderObj;

void RenderObj::setShader(fei::ShaderProgram* sp)
{
	_shaderProg = sp;
}

fei::ShaderProgram* RenderObj::getShader()
{
	return _shaderProg;
}

fei::RenderList* RenderObj::getParent()
{
	return _parent;
}

void RenderObj::setParent(fei::RenderList* parent)
{
	_parent = parent;
}

void RenderObj::delFromParent()
{
	if (_parent) {
		_parent->del(this);
	}
}

void RenderObj::setSubstitute(RenderObj* sub)
{
	_substitute = sub;
}

RenderObj* RenderObj::getSubstitute()
{
	return _substitute;
}

void RenderObj::setAnime(fei::Anime* anime)
{
	_anime = anime;
}

fei::Anime* RenderObj::getAnime()
{
	return _anime;
}

void RenderObj::setHasAlpha(bool hasAlpha)
{
	_hasAlpha = hasAlpha;
}

bool RenderObj::isVisible() const
{
	return _isVisible;
}

void RenderObj::setVisible(bool visible)
{
	_isVisible = visible;
}

void RenderObj::hide()
{
	setVisible(false);
}

void RenderObj::show()
{
	setVisible(true);
}

void RenderObj::setUseColor(bool useColor)
{
	_useColor = useColor;
}

void RenderObj::setScale(float scl)
{
	_scale = scl;
}

void RenderObj::setAngleX(float agl)
{
	_angleX = agl;
}

void RenderObj::setAngleY(float agl)
{
	_angleY = agl;
}

void RenderObj::setAngleZ(float agl)
{
	_angleZ = agl;
}

void RenderObj::setAngle(float agl)
{
	_angle = agl;
}

void RenderObj::setAnchor(const fei::Vec2& acr)
{
	_anchor = acr;
}

void RenderObj::setAnchorX(float x)
{
	_anchor.x = x;
}

void RenderObj::setAnchorY(float y)
{
	_anchor.y = y;
}

void RenderObj::setZPos(float z)
{
	_zPos = z;
}

float RenderObj::getScale() const
{
	return _scale;
}

float RenderObj::getAngleX() const
{
	return _angleX;
}

float RenderObj::getAngleY() const
{
	return _angleY;
}

float RenderObj::getAngleZ() const
{
	return _angleZ;
}

float RenderObj::getAngle() const
{
	return _angle;
}

const fei::Vec2 RenderObj::getAnchor() const
{
	return _anchor;
}

float RenderObj::getAnchorX() const
{
	return _anchor.x;
}

float RenderObj::getAnchorY() const
{
	return _anchor.y;
}

float RenderObj::getZPos() const
{
	return _zPos;
}

void RenderObj::roundAnchor()
{
	_anchor *= 2.0f;
	_anchor.round();
	_anchor *= 0.5f;
}

void RenderObj::moveAnchor(const fei::Vec2& v)
{
	_anchor.add(v);
}

void RenderObj::rotateX(float dltAgl)
{
	_angleX += dltAgl;
}

void RenderObj::rotateY(float dltAgl)
{
	_angleY += dltAgl;
}

void RenderObj::rotateZ(float dltAgl)
{
	_angleZ += dltAgl;
}

void RenderObj::rotate(float dltAgl)
{
	_angle += dltAgl;
}

void RenderObj::matrixTransformBegin()
{
	glTranslatef(getPositionX(), getPositionY(), _zPos);
	if (_angleZ != 0.0f) {
		glRotatef(fei::R2Df(_angleZ), 0.0f, 0.0f, 1.0f);
	}
	if (_angleY != 0.0f) {
		glRotatef(fei::R2Df(_angleY), 0.0f, 1.0f, 0.0f);
	}
	if (_angleX != 0.0f) {
		glRotatef(fei::R2Df(_angleX), 1.0f, 0.0f, 0.0f);
	}
	if (_angle != 0.0f) {
		glRotatef(fei::R2Df(_angle), 0.0f, 0.0f, 1.0f);
	}
	glPushMatrix();
	if (_scale != 1.0f) {
		glScalef(_scale, _scale, _scale);
	}
}

void RenderObj::anchorTransform()
{
	glTranslatef(-_anchor.x, -_anchor.y, 0.0f);
}

void RenderObj::matrixTransformEnd()
{
	glPopMatrix();
}

void RenderObj::animeUpdate()
{
	if (_anime) {
		_anime->feiObjectUpdate(this);
	}
}

void RenderObj::feiBasicUpdate()
{
	animeUpdate();
}

void RenderObj::draw()
{
	if (!isActive()) {
		return;
	}
	beforeDraw();
	if (_useColor) {
		_color.use();
	}
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	if (_hasAlpha) {
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	} else {
		glDepthFunc(GL_LESS);
	}

	bool visible = isVisible();
	if (_shaderProg && visible) {
		_shaderProg->push();
	}

	matrixTransformBegin();

	if (visible) {
		if (_substitute) {
			_substitute->anchorTransform();
			_substitute->drawIt();
		} else {
			anchorTransform();
			drawIt();
		}
	}

	matrixTransformEnd();

	if (_shaderProg && visible) {
		_shaderProg->pop();
	}

	glDisable(GL_DEPTH_TEST);
	if (_hasAlpha) {
		glDisable(GL_BLEND);
	}
	afterDraw();
}

void RenderObj::beforeDraw()
{
}

void RenderObj::afterDraw()
{
}

void RenderObj::drawIt()
{
}

void RenderObj::setColor(const fei::Color& c)
{
	_color = c;
}

void RenderObj::setColorAlpha(float alpha)
{
	_color.w = alpha;
	if (alpha < 1.0f) {
		setHasAlpha(true);
	}
}

const fei::Color RenderObj::getColor() const
{
	return _color;
}

void RenderObj::processWith(std::function<void(RenderObj*)> func)
{
	if (func) {
		func(this);
	}
}
