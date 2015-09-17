#if !defined(_FEI_SEGMENT_H_)
#define _FEI_SEGMENT_H_

#include "math/Vec2.h"
#include "math/mathdef.h"

namespace fei {

class Segment
{
public:
	Segment();
	Segment(Vec2 aa, Vec2 bb);

	const Vec2 getVector() const;
	float getLength() const;
	float getLengthSq() const;

	void swapAB();

	bool collidePoint(const Vec2& p) const;
	bool rectCollidePoint(const Vec2& p) const;
	bool collideSegment(Vec2& pt, const Segment& seg) const;
	bool collideRay(Vec2& pt, const Vec2& src, const Vec2& drct) const;
	int onLeftOrRight(const Vec2& pt) const;

	Vec2 a;
	Vec2 b;
};

}

inline const fei::Vec2 fei::Segment::getVector() const
{
	return b - a;
}

inline float fei::Segment::getLength() const
{
	return getVector().getLength();
}

inline float fei::Segment::getLengthSq() const
{
	return getVector().getLengthSq();
}

inline bool fei::Segment::collidePoint(const Vec2& p) const
{
	return (std::abs((p - a).cross(b - a)) < fei::epsf) && rectCollidePoint(p);
}

inline bool fei::Segment::rectCollidePoint(const fei::Vec2& p) const
{
	return ((p.x - a.x) * (p.x - b.x) <= 0.0f
		&& (p.y - a.y) * (p.y - b.y) <= 0.0f);
}

inline int fei::Segment::onLeftOrRight(const fei::Vec2& pt) const
{
	double cp = getVector().cross(pt - a);
	if (std::abs(cp) < fei::epsf) {
		return 0;
	}
	if (cp > 0.0f) {
		return -1;
	} else {
		return 1;
	}
}
		  
#endif
