#include "Spaceship.h"

Spaceship::Spaceship()
{
	isPlayer = false;
	shipID = -1;

	pos = glm::vec3(0.0f);
	rot = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);

	health = 100.0f;
}

Spaceship::Spaceship(bool status, int id, glm::vec3 nPos, glm::vec3 nRot, glm::vec3 nScale, float health)
{
	isPlayer = status;
	shipID = id;

	pos = nPos;
	rot = nRot;
	scale = nScale;

	health = health;
}


Spaceship::~Spaceship()
{
	Spaceship();
}

void Spaceship::SetPlayerStatus(bool status)
{
	isPlayer = status;
}

bool Spaceship::GetPlayerStatus()
{
	return isPlayer;
}

void Spaceship::SetShipID(int id)
{
	shipID = id;
}

int Spaceship::GetShipID()
{
	return shipID;
}

void Spaceship::SetPosition(glm::vec3 nPos)
{
	// TODO: Validate values
	pos = nPos;
}

glm::vec3 Spaceship::GetPosition()
{
	return pos;
}

void Spaceship::SetRotation(glm::vec3 nRot)
{
	// TODO: Validate values
	rot = nRot;
}

glm::vec3 Spaceship::GetRotation()
{
	return rot;
}

void Spaceship::SetScale(glm::vec3 nScale)
{
	// TODO: Validate values
	scale = nScale;
}

glm::vec3 Spaceship::GetScale()
{
	return scale;
}

void Spaceship::SetHealth(float nHealth)
{
	if (nHealth > 0 && nHealth < 100.0f)
	{
		health = nHealth;
	}
	else
	{
		health = 20.0f;
	}
}

float Spaceship::GetHealth()
{
	return health;
}
