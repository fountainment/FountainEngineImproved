#include "Image.h"
#include "render/Render.h"

using fei::Image;

Image::Image()
: id(0),
  size(fei::Vec2::ZERO)
{
	for (int i = 0; i < 8; i++) {
		texCoord[i] = 0.0f;
	}
}

Image::Image(const Image& img)
: id(0),
  size(fei::Vec2::ZERO)
{
	(*this) = img;
}

void Image::operator=(const Image& img)
{
	if (id != img.id) {
		fei::Render::getInstance()->releaseTexture(id);
		id = img.id;
		fei::Render::getInstance()->addRefTexture(id);
	}
	setIsAlpha(img.isAlpha());
	size = img.size;
	for (int i = 0; i < 8; i++) {
		texCoord[i] = img.texCoord[i];
	}
}

Image::~Image()
{
	fei::Render::getInstance()->releaseTexture(id);
}

Image::Image(GLuint texId, const fei::Vec2& texSize, const fei::Rect& imageRect)
{
	fei::Render::getInstance()->addRefTexture(texId);
	auto rect = imageRect;
	id = texId;
	size = imageRect.getSize();
	rect.zoom(texSize.reciprocal());
	rect.getStripCoord(texCoord);
}

void Image::drawIt()
{
	fei::Render::getInstance()->bindTexture(id);
	fei::Render::getInstance()->drawTexQuad(size, texCoord);
	fei::Render::getInstance()->disableTexture();
}

const fei::Vec2 Image::getSize() const
{
	return size;
}

const fei::Vec2 Image::getLB()
{
	auto v = getTextureSize();
	v.zoom(fei::Vec2(texCoord[2], texCoord[3]));
	return v;
}

const fei::Vec2 Image::getTextureSize()
{
	return fei::Vec2(size.x / (texCoord[4] - texCoord[2]), size.y / (texCoord[5] - texCoord[3]));
}

const Image Image::getImage(const fei::Rect& imageRect)
{
	auto rect = imageRect;
	rect.move(getLB());
	Image result = Image(id, getTextureSize(), rect);
	result.setIsAlpha(isAlpha());
	return result;
}

const Image Image::getImage(const fei::Vec2& p, const fei::Vec2& s)
{
	return getImage(fei::Rect(p, s));
}

const Image Image::getImage()
{
	return *this;
}
