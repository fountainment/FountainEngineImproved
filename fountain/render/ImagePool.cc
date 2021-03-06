#include "render/ImagePool.h"

#include "base/basedef.h"
#include "base/fileUtil.h"
#include "math/hash.h"
#include "math/Rect.h"

using fei::ImagePool;

struct ImageInfo {
	ImageInfo(const std::string& name, int hash, fei::Rect rect, fei::Vec2 anchor)
	: _name(name),
	  _hash(hash),
	  _rect(rect),
	  _anchor(anchor)
	{}

	std::string _name;
	int _hash;
	fei::Rect _rect;
	fei::Vec2 _anchor;
};

const std::vector<ImageInfo> loadSipFile(const std::string& filename)
{
	std::vector<ImageInfo> result;
	char name[100];
	int x, y, _imageNum, tmp;
	float rx, ry, rw, rh;
	fei::File sipF;
	sipF.open(filename.c_str(), "r");
	if (!sipF.exist()) {
		std::fprintf(stderr, "loadSipFile: \"%s\" file not exist!\n", filename.c_str());
		return result;
	}
	sipF.scanf("%d%d%d", &x, &y, &_imageNum);
	for (int i = 0; i < _imageNum; i++) {
		tmp = sipF.scanf("%s%f%f%f%f", name, &rw, &rh, &rx, &ry);
		if (EOF == tmp) break;
		result.push_back(ImageInfo(name, fei::bkdrHash(name), \
				fei::Rect(rx, y - ry - rh, rw, rh), fei::Vec2::ZERO));
	}
	sipF.close();
	return result;
}

const std::vector<ImageInfo> loadIpiFile(const std::string& filename)
{
	std::vector<ImageInfo> result;
	char name[100];
	int x, y, _imageNum, tmp;
	float rx, ry, rw, rh, ax, ay;
	fei::File ipiF;
	ipiF.open(filename.c_str(), "r");
	if (!ipiF.exist()) {
		std::fprintf(stderr, "loadIpiFile: \"%s\" file not exist!\n", filename.c_str());
		return result;
	}
	ipiF.scanf("%d%d%d", &x, &y, &_imageNum);
	for (int i = 0; i < _imageNum; i++) {
		tmp = ipiF.scanf("%s%f%f%f%f%f%f", name, &rw, &rh, &rx, &ry, &ax, &ay);
		if (EOF == tmp) break;
		result.push_back(ImageInfo(name, fei::bkdrHash(name), \
				fei::Rect(rx, y - ry - rh, rw, rh), fei::Vec2(ax, -ay)));
	}
	ipiF.close();
	return result;
}

void writeIpiFile(const std::string& filename, const fei::Vec2& size, const std::vector<ImageInfo>& imageInfo)
{
	std::FILE *ipiF = std::fopen(filename.c_str(), "w");
	if (!ipiF) {
		std::fprintf(stderr, "loadIpiFile: \"%s\" file not exist!\n", filename.c_str());
		return;
	}
	std::fprintf(ipiF, "%.0f %.0f\n%d\n", size.x, size.y, static_cast<int>(imageInfo.size()));
	for (auto im : imageInfo) {
		auto pos = im._rect.getPosition();
		auto imsize = im._rect.getSize();
		auto anchor = im._anchor;
		std::fprintf(ipiF, "%s %.0f %.0f %.0f %.0f %.1f %.1f\n", \
				im._name.c_str(), imsize.x, imsize.y, \
				pos.x, size.y - pos.y - imsize.y, \
				anchor.x, -anchor.y);
	}
	std::fclose(ipiF);
}

ImagePool::ImagePool()
: _imageNum(0)
{
}

ImagePool::ImagePool(const std::string& texName, const std::string& sipName)
{
	load(texName, sipName);
}

ImagePool::ImagePool(const fei::Texture& texure, const std::string& sipName)
{
	loadTextureAndSIP(texure, sipName);
}

void ImagePool::load(const std::string& texName, const std::string& sipName)
{
	fei::Texture tmpTex;
	tmpTex.load(texName);
	loadTextureAndSIP(tmpTex, sipName);
}

void ImagePool::loadTextureAndSIP(const fei::Texture& texure, const std::string& sipName)
{
	_imageList.clear();
	auto result = loadSipFile(sipName);
	_imageNum = result.size();
	for (int i = 0; i < _imageNum; i++) {
		_nameHash2ImageIndex[result[i]._hash] = i;
		auto image = texure.getImage(result[i]._rect);
		image.setName(result[i]._name);
		image.setAnchor(result[i]._anchor);
		_imageList.push_back(image);
	}
}

void ImagePool::loadTextureAndIPI(const fei::Texture& texure, const std::string& ipiName)
{
	_imageList.clear();
	auto result = loadIpiFile(ipiName);
	_imageNum = result.size();
	for (int i = 0; i < _imageNum; i++) {
		_nameHash2ImageIndex[result[i]._hash] = i;
		auto image = texure.getImage(result[i]._rect);
		image.setName(result[i]._name);
		image.setAnchor(result[i]._anchor);
		_imageList.push_back(image);
	}
}

fei::Image* ImagePool::getImage(const std::string& imageName)
{
	int index = _nameHash2ImageIndex[fei::bkdrHash(imageName)];
	return getImage(index);
}

fei::Image* ImagePool::getImage(int index)
{
	fei::Image* image = nullptr;
	if (index >= 0 && index < _imageNum) {
		image = &_imageList[index];
	}
	return image;
}

int ImagePool::getImageNum()
{
	return _imageNum;
}

void ImagePool::moveImageAnchor(const fei::Vec2& v)
{
	for (auto& image : _imageList) {
		image.moveAnchor(v);
	}
}

void ImagePool::roundAnchor()
{
	for (auto& image : _imageList) {
		image.roundAnchor();
	}
}

void ImagePool::dumpIPI(const std::string& name)
{
	std::vector<ImageInfo> infoVec;
	fei::Vec2 texSize;
	if (!_imageList.empty()) {
		texSize = _imageList[0].getTextureSize();
	}
	for (auto image : _imageList) {
		auto rect = image.getTexturePixelRect();
		auto anchor = image.getAnchor();
		auto imName = image.getName();
		if (imName == fei::EmptyStr) {
			imName = "image";
		}
		infoVec.push_back(ImageInfo(imName, fei::bkdrHash(imName), rect, anchor));
	}
	writeIpiFile(name, texSize, infoVec);
}

const fei::Rect ImagePool::getOuterRect()
{
	fei::Rect ret;
	bool first = true;
	for (auto image : _imageList) {
		auto rect = fei::Rect(image.getSize());
		rect.setCenter(-image.getAnchor());
		if (first) {
			ret = rect;
			first = false;
		} else {
			ret.mergeRect(rect);
		}
	}
	return ret;
}
