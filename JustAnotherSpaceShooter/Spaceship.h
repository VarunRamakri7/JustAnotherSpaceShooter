#include <glm/glm.hpp>

//#include "LoadMesh.h"

class Spaceship
{
    public:
        Spaceship();
        Spaceship(bool status, int id, glm::vec3 nPos, glm::vec3 nRot, glm::vec3 nScale, float health);
        ~Spaceship();

        // TODO: Add collision check

        void KillShip();

        // Getters and setter
        void SetPlayerStatus(bool status);
        bool GetPlayerStatus();
        void SetShipID(int id);
        int GetShipID();

        void SetPosition(glm::vec3 nPos);
        glm::vec3 GetPosition();
        void SetRotation(glm::vec3 nRot);
        glm::vec3 GetRotation();
        void SetScale(glm::vec3 nScale);
        glm::vec3 GetScale();

        void SetHealth(float nHealth);
        float GetHealth();
        void SetSpeed(float nSpeed);
        float GetSpeed();
        void SetAmmo(int nAmmo);
        int GetAmmo();

    private:
        bool isPlayer; // "True" if player, "false" if enemy
        int shipID;

        // MeshData mesh;
        glm::vec3 pos; // Position of ship
        glm::vec3 rot; // Rotation of ship
        glm::vec3 scale; // Scale of ship

        // Power related
        float health;
        float speed;
        int ammo;
};
