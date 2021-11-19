#include "Spaceship.h"

/**
* Constructors and Destructor
*/

Spaceship::Spaceship()
{
	isPlayer = false;
	shipID = -1;

	pos = glm::vec3(0.0f);
	rot = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);

	health = 100.0f;
	speed = 0.5f;
	ammo = 20;
}

Spaceship::Spaceship(bool status, int id, glm::vec3 nPos, glm::vec3 nRot, glm::vec3 nScale, float nHealth, float nSpeed, unsigned int nAmmo)
{
	isPlayer = status;
	shipID = id;

	pos = nPos;
	rot = nRot;
	scale = nScale;

	health = nHealth;
	speed = nSpeed;
	ammo = nAmmo;
}


Spaceship::~Spaceship()
{
	isPlayer = false;
	shipID = -1;

	pos = glm::vec3(0.0f);
	rot = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);

	health = 0.0f;
}

/**
* Power Related Functions
*/

// Kill ship if it loses health
void Spaceship::KillShip()
{
	// Destroy ship
	// TODO: Complete function
	if (health < 0.5f)
	{
		Spaceship::~Spaceship();
	}
}

/**
* Setters and Getters are Below
*/

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

void Spaceship::SetSpeed(float nSpeed)
{
	speed = nSpeed;
}

float Spaceship::GetSpeed()
{
	return speed;
}


void Spaceship::SetAmmo(unsigned int nAmmo)
{
	ammo = nAmmo;
}

unsigned int Spaceship::GetAmmo()
{
	return ammo;
}