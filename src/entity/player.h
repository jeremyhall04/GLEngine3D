#ifndef PLAYER_H
#define PLAYER_H

#include "../GLcommon.h"
#include "../GLcommon_math.h"
#include "ray.h"

class PerspectiveCamera;

class Player
{
private:
	glm::vec3 pos, dir;
	float rayLength = 4.0f;

public:
	Ray ray;

public:
	Player();
	Player(PerspectiveCamera* camera);
	~Player();

	void update(PerspectiveCamera* camera);
	const glm::vec3& getPos() const { return pos; };
	const glm::vec3& getDir() const { return dir; };
};

#endif // !PLAYER_H
