#include "player.h"

#include "../utils/camera.h"

Player::Player()
{
	pos = glm::vec3(0, 0, 0);
	dir = glm::vec3(0, 0, 1);
}

Player::Player(PerspectiveCamera* camera)
{
	update(camera);
}

Player::~Player()
{
}

void Player::update(PerspectiveCamera* camera)
{
	pos = camera->getPosition();
	dir = camera->getDirection();
}
