#include "anime/FrameAnime.h"

using fei::FrameAnime;

FrameAnime::FrameAnime()
: curFrameIndex(0),
  fps(60.0f)
{
}

void FrameAnime::load(const fei::ImagePool& imagePool)
{
	framePool = imagePool;
	curFrameIndex = 0;
}

void FrameAnime::loadTextureAndSIP(const fei::Texture& texture, const std::string& sipName)
{
	framePool.loadTextureAndSIP(texture, sipName);
	curFrameIndex = 0;
}

void FrameAnime::load(const std::string& name)
{
	loadImageFileAndSIP(name);
}

void FrameAnime::load(const std::string& name1, const std::string& name2)
{
	loadImageFileAndSIP(name1, name2);
}

void FrameAnime::loadImageFileAndSIP(const std::string& name)
{
	loadImageFileAndSIP(name + ".png", name + ".sip");
}

void FrameAnime::loadImageFileAndSIP(const std::string& textureName, const std::string& sipName)
{
	fei::Texture tmpTex;
	tmpTex.load(textureName);
	loadTextureAndSIP(tmpTex, sipName);
}

void FrameAnime::loadImageFileAndIPI(const std::string& name)
{
	loadImageFileAndIPI(name + ".png", name + ".ipi");
}

void FrameAnime::loadImageFileAndIPI(const std::string& textureName, const std::string& ipiName)
{
	fei::Texture tmpTex;
	tmpTex.load(textureName);
	loadTextureAndIPI(tmpTex, ipiName);
}

void FrameAnime::loadTextureAndIPI(const fei::Texture& texture, const std::string& ipiName)
{
	framePool.loadTextureAndIPI(texture, ipiName);
	curFrameIndex = 0;
}

void FrameAnime::feiObjectUpdate(fei::RenderObj* rObj)
{
	updateFrameIndex();
	updateFrameContent(rObj);
	update(rObj);
}

void FrameAnime::updateFrameIndex()
{
	if (isPlay()) {
		curFrameIndex += getFps() * getDeltaTime();
		if (curFrameIndex >= getFrameNum()) {
			curFrameIndex = 0;
			if (!isLoop()) {
				stop();
			}
		}
	}
}

void FrameAnime::updateFrameContent(fei::RenderObj* rObj)
{
	//not set null when stop
	//rObj->setSubstitute(nullptr);
	if (!isStop()) {
		fei::Image *image = framePool.getImage((int)curFrameIndex);
		rObj->setSubstitute(image);
	}
}

float FrameAnime::getFps()
{
	return fps;
}

void FrameAnime::setFps(float f)
{
	fps = f;
}

int FrameAnime::getFrameNum()
{
	return framePool.getImageNum();
}

int FrameAnime::getCurFrameIndex()
{
	return (int)curFrameIndex;
}

void FrameAnime::setCurFrameIndex(int cfi)
{
	curFrameIndex = cfi;
}

fei::ImagePool* FrameAnime::getFramePool()
{
	return &framePool;
}
