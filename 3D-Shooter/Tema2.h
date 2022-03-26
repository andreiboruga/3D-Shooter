#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/camera.h"
#include <utility>
#include <iostream>

using namespace std;

#define DIM 40

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;

    private:

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color);
        void Labirint();

        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        implemented::CameraT* camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;
        float fov = RADIANS(60);
        float width = 50.0f;
        float height = 40.0f;

        int a[DIM][DIM];
        pair<int, int> start_poz;
        pair<int, int> end_poz;

        glm::vec3 lightPosition;
        unsigned int materialShininess;
        float materialKd;
        float materialKs;

        float playerX, playerY;
        pair<int, int> initialCameraDirection;
        float enemySpeed;
        vector<pair<int, float>> ePos; ///////////ePos[i].first = {0 -> se misca pe x pozitiv, 1 -> negativ, 2->z poz, 3 -> z neg}
                                     ///////////ePos[i].second = {x sau z actual}

        float playerWidth;
        int HP;
        int damage;
        float oldtime;
        float newtime;
        float maxTime;
        float cameraSpeed;

        bool projectileShot;
        float range;
        glm::vec3 bulletPos;
        glm::vec3 bulletDir;
        float bulletSpeed;
        float bulletTraveled;

    };
}