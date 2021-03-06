#include "base/stringUtil.h"

#include "math/Vec2.h"

static char buffer[256];

const std::vector<std::string> fei::strSplit(const std::string& str, char delim)
{
	std::vector<std::string> elems;
	std::stringstream ss(str);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

const std::vector<std::string> fei::strSplit(const std::string& str)
{
	auto elems = fei::strSplit(str, ' ');
	std::vector<std::string> ret;
	for (auto str : elems) {
		if (str != fei::EmptyStr) {
			ret.push_back(str);
		}
	}
	return ret;
}

const std::vector<fei::Vec2> fei::strVecToVec2Vec(const std::vector<std::string>& strVec)
{
	std::vector<fei::Vec2> ret;
	int len = strVec.size();
	for (int i = 0; i < len; i += 2) {
		float x, y;
		x = fei::strToFloat(strVec[i]);
		y = i != len - 1 ? fei::strToFloat(strVec[i + 1]) : 0;
		ret.push_back(fei::Vec2(x, y));
	}
	return ret;
}

int fei::strFormat(std::string& str, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	auto ret = std::vsprintf(buffer, format, args);
	va_end(args);
	str = std::string(buffer);
	return ret;
}

const std::string fei::strFormat(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	std::vsprintf(buffer, format, args);
	va_end(args);
	return std::string(buffer);
}
