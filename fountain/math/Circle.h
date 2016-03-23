#if !defined(_FEI_CIRCLE_H_)
#define _FEI_CIRCLE_H_

#include "base/basedef.h"
#include "math/Shape.h"
#include "math/Vec2.h"

namespace fei {

class Circle : public Shape
{
public:
	Circle();
	explicit Circle(float r);

	virtual int getDataSize() const override;
	virtual const float* getDataPtr() const override;

	float getRadius() const;
	void setRadius(float r);

	virtual bool collide(const Shape* shape) const override;
	virtual bool collidePoint(const Vec2& p) const override;

private:
	float _radius;

	static std::vector<Vec2> _data32;
};

} // namespace fei

#endif // _FEI_CIRCLE_H_
