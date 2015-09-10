#if !defined(_FEI_SHAPE_H_)
#define _FEI_SHAPE_H_

#include "base/basedef.h"
#include "base/NodeBase.h"
#include "math/Vec2.h"

namespace fei {

class Shape : public NodeBase
{
public:
	enum class Type {
		POLYGON = 1,
		CIRCLE = 2
	};

	Shape();
	virtual ~Shape();

	virtual bool collide(const Shape* other) const;

	virtual const float* getDataPtr() const = 0;
	virtual int getDataSize() const = 0;

	void setSolid(bool solid);
	bool isSolid() const;

	Type getType() const;

	//TODO: design Shape data storage and render logic

private:
	bool _isSolid;
	Type shapeType;
};

}

#endif
