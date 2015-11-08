#if !defined(_CHARACTOR_H_)
#define _CHARACTOR_H_

#include "fountain.h"
#include "util/PhysicsAnime.h"

class Charactor : public fei::RenderObj
{
public:
	Charactor();

	virtual void drawIt() override;
	virtual void init() override;
	virtual void destroy() override;
	virtual void update() override;

	void setSpeed(const fei::Vec2& sp);
	void setSpeed(const fei::Vec2& drct, float sp);

	void attack();

	fei::Clock charClock;

private:
	fei::Vec2 speed;
	fei::Body *body;

	fei::FrameAnime *curAnime;
	fei::FrameAnime walkAnime[4];
	fei::FrameAnime runAnime[4];
	fut::CollisionFrameAnime atk1Anime;
	fut::CollisionFrameAnime atk2Anime;
	fut::CollisionFrameAnime atk3Anime;

	//TODO: anime state control
};

#endif
