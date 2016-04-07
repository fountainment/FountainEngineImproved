#ifndef _FEI_RECT_H_
#define _FEI_RECT_H_

#include "base/basedef.h"
#include "math/Segment.h"
#include "math/Shape.h"
#include "math/Vec2.h"

namespace fei {

class Rect : public Shape
{
public:
	Rect();
	Rect(const Vec2& sz);
	Rect(float x, float y, float w, float h);
	Rect(const Vec2& pos, const Vec2& sz);

	const Vec2 getCenter() const;
	void setCenter(const Vec2& p);
	const Vec2 getSize() const;
	void setSize(const Vec2& sz);

	float getLeft() const;
	float getRight() const;
	float getTop() const;
	float getBottom() const;

	Segment getLeftSegment() const;
	Segment getRightSegment() const;
	Segment getTopSegment() const;
	Segment getBottomSegment() const;
	std::vector<Segment> getAllSegments() const;

	void zoom(float scale);
	void zoom(const Vec2& v);
	const Rect zoomed(float scale) const;
	const Rect zoomed(const Vec2& v) const;
	void normalize();

	virtual bool collide(const Shape* other) const override;

	virtual bool collidePoint(const Vec2& pt) const override;

	bool collideSegment(Vec2& pt, const Segment& seg) const;

	bool collideRect(const Rect& rct) const;

	void getStripCoord(float* coord) const;

	const float* getDataPtr() const override;
	int getDataSize() const override;

private:
	Vec2 _size;
};

} // namespace fei

inline const fei::Vec2 fei::Rect::getSize() const
{
	return _size;
}

inline void fei::Rect::setSize(const fei::Vec2& sz)
{
	_size = sz;
}

#endif // _FEI_RECT_H_
