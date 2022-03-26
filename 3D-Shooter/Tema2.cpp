#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include <utility>

using namespace std;
using namespace m1;

Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    glUseProgram(shader->program);

    GLint locLightPos = glGetUniformLocation(shader->program, "light_position");
    glUniform3fv(locLightPos, 1, glm::value_ptr(lightPosition));

    glm::vec3 eyePosition = camera->position;

    GLint locEyePos = glGetUniformLocation(shader->program, "eye_position");
    glUniform3fv(locEyePos, 1, glm::value_ptr(eyePosition));

    GLint locMaterial = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(locMaterial, materialShininess);

    GLint locMaterialKd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(locMaterialKd, materialKd);

    GLint locMaterialKs = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(locMaterialKs, materialKs);

    GLint locObject = glGetUniformLocation(shader->program, "object_color");
    glUniform3fv(locObject, 1, glm::value_ptr(color));

    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    glm::mat4 viewMatrix = camera->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema2::Labirint()
{
    srand(time(NULL));
    for (int i = 0; i < DIM; i++)
    {
        for (int j = 0; j < DIM; j++)
        {
            a[i][j] = 1;
        }
    }

    int side1 = rand() % 4;//////0 = E, 1 = S, 2 = V, 3 = N;
    int side2 = rand() % 4;

    pair<int, int> road;
    pair<int, int> finish;
    road.first = 0;
    road.second = 0;
    finish.first = 0;
    finish.second = 0;

    if (side1 == side2)
    {
        if (side1 > 0)
            side1--;
        else
            side1++;
    }

    if (side1 == 0)
    {
        start_poz.second = DIM - 1;
        start_poz.first = rand() % (DIM - 2) + 1;
        road.second--;
    }
    else if (side1 == 1)
    {
        start_poz.first = DIM - 1;
        start_poz.second = rand() % (DIM - 2) + 1;
        road.first--;
    }
    else if (side1 == 2)
    {
        start_poz.second = 0;
        start_poz.first = rand() % (DIM - 2) + 1;
        road.second++;
    }
    else
    {
        start_poz.first = 0;
        start_poz.second = rand() % (DIM - 2) + 1;
        road.first++;
    }

    if (side2 == 0)
    {
        end_poz.second = DIM - 1;
        end_poz.first = rand() % (DIM - 2) + 1;
        finish.second--;    
    }
    else if (side2 == 1)
    {
        end_poz.first = DIM - 1;
        end_poz.second = rand() % (DIM - 2) + 1;
        finish.first--;
    }
    else if (side2 == 2)
    {
        end_poz.second = 0;
        end_poz.first = rand() % (DIM - 2) + 1;
        finish.second++;
    }
    else
    {
        end_poz.first = 0;
        end_poz.second = rand() % (DIM - 2) + 1;
        finish.first++;
    }

    road.first += start_poz.first;
    road.second += start_poz.second;

    finish.first += end_poz.first;
    finish.second += end_poz.second;

    a[start_poz.first][start_poz.second] = 1;
    a[end_poz.first][end_poz.second] = 0;

    initialCameraDirection.first = road.first;
    initialCameraDirection.second = road.second;

    while (road.first != finish.first && road.second != finish.second)
    {
        a[road.first][road.second] = 0;
        int select = rand() % 2;
        if (select == 0)
        {
            if (road.first < finish.first)
                road.first++;
            else
                road.first--;
        }
        else
        {
            if (road.second < finish.second)
                road.second++;
            else
                road.second--;
        }
    }

    while (road.first != finish.first)
    {
        a[road.first][road.second] = 0;
        if (road.first < finish.first)
            road.first++;
        else
            road.first--;

    }

    while (road.second != finish.second)
    {
        a[road.first][road.second] = 0;
        if (road.second < finish.second)
            road.second++;
        else
            road.second--;
    }

    a[finish.first][finish.second] = 0;

    for (int i = 1; i < DIM - 1; i++)
    {
        for (int j = 1; j < DIM - 1; j++)
        {
            if (i == initialCameraDirection.first && j == initialCameraDirection.second)
                continue;
            if (a[i][j] == 1)
            {
                int select = rand() % 100;
                if (select < 50)
                    a[i][j] = 0;
                else if (select < 80)
                {
                    a[i][j] = 2;
                    int aux = rand() % 4;
                    if (aux < 2)
                    {
                        ePos.push_back(make_pair(aux, i + 0.5f));
                    }
                    else
                    {
                        ePos.push_back(make_pair(aux, j + 0.5f));
                    }
                }
            }
            else if (a[i][j] == 0)
            {
                int select = rand() % 3;
                if (select == 0)
                {
                    a[i][j] = 2;
                    int aux = rand() % 4;
                    if (aux < 2)
                    {
                        ePos.push_back(make_pair(aux, i + 0.5f));
                    }
                    else
                    {
                        ePos.push_back(make_pair(aux, j + 0.5f));
                    }
                }
            }
        }
    }


    for (int i = 0; i < DIM; i++)
    {
        for (int j = 0; j < DIM; j++)
        {
            cout << a[i][j];
        }
        cout << "\n";
    }

}


void Tema2::Init()
{
    cameraSpeed = 2.0f;
    oldtime = glfwGetTime();
    enemySpeed = 0.5f;
    playerWidth = 0.1f;
    HP = 100;
    damage = 20;
    maxTime = 999;
    projectileShot = false;
    range = 7;
    bulletSpeed = 5;

    {
        Shader* shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        lightPosition = glm::vec3(0, 1, 1);
        materialShininess = 30;
        materialKd = 0.5;
        materialKs = 0.5;
    }

    Labirint();
    

    renderCameraTarget = true;

    camera = new implemented::CameraT();

    if (start_poz.first == 0)
    {
        camera->Set(glm::vec3(start_poz.first + 1.5f - camera->distanceToTarget, 0.5, start_poz.second + 0.5f),
            glm::vec3(initialCameraDirection.first + 1.5f, 0.5, initialCameraDirection.second + 0.5f), glm::vec3(0, 1, 0));
        a[start_poz.first + 1][start_poz.second] = 0;
    }
    else if (start_poz.first == DIM - 1)
    {
        camera->Set(glm::vec3(start_poz.first - 0.5f + camera->distanceToTarget, 0.5, start_poz.second + 0.5f),
            glm::vec3(initialCameraDirection.first - 1.5f, 0.5, initialCameraDirection.second + 0.5f), glm::vec3(0, 1, 0));
        a[start_poz.first - 1][start_poz.second] = 0;
    }
    else if (start_poz.second == 0)
    {
        camera->Set(glm::vec3(start_poz.first + 0.5f, 0.5, start_poz.second + 1.5f - camera->distanceToTarget),
            glm::vec3(initialCameraDirection.first + 0.5f, 0.5, initialCameraDirection.second + 1.5f), glm::vec3(0, 1, 0));
        a[start_poz.first][start_poz.second + 1] = 0;
    }
    else
    {
        camera->Set(glm::vec3(start_poz.first + 0.5f, 0.5, start_poz.second - 0.5f + camera->distanceToTarget),
            glm::vec3(initialCameraDirection.first + 0.5f, 0.5, initialCameraDirection.second - 1.5f), glm::vec3(0, 1, 0));
        a[start_poz.first][start_poz.second - 1] = 0;
    }


    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

    int nr = 0;
    for (int i = 0; i < DIM; i++)
    {
        for (int j = 0; j < DIM; j++)
        {
            if (a[i][j] == 2)
                nr++;
        }
    }
    cout << "HP: " << HP << "\n";

}


void Tema2::FrameStart()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{
    if (projectileShot)
    {
        bulletPos = bulletPos + bulletDir * bulletSpeed * deltaTimeSeconds;
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, bulletPos);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
        RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, glm::vec3(1, 0.1, 0.2));
        bulletTraveled += bulletSpeed * deltaTimeSeconds;
        if (bulletTraveled >= range)
            projectileShot = false;

    }


    newtime = glfwGetTime();
    if (floor(newtime) > floor(oldtime))
    {
        oldtime = newtime;
        cout << "Time: " << maxTime - floor(newtime) << "\n";
        if (floor(newtime) == maxTime)
        {
            cout << "Game Over!\n";
            exit(0);
        }
    }

    glm::vec3 pos;
    if (renderCameraTarget)
    {
        pos = camera->GetTargetPosition();
    }
    else
    {
        pos = camera->position;
    }
    bool coll = false;
    pair<float, float> corner1;
    pair<float, float> corner2;
    pair<float, float> corner3;
    pair<float, float> corner4;

    corner1.first = pos.x - playerWidth;
    corner1.second = pos.z - playerWidth;
    corner2.first = pos.x - playerWidth;
    corner2.second = pos.z + playerWidth;
    corner3.first = pos.x + playerWidth;
    corner3.second = pos.z - playerWidth;
    corner4.first = pos.x + playerWidth;
    corner4.second = pos.z + playerWidth;

    if (pos.x < 0 || pos.x >= DIM || pos.z < 0 || pos.z >= DIM)
    {
        cout << "Congratulations! You Won!\n";
        exit(0);
    }


    int curr = 0;
    for (int i = 0; i < DIM; i++)
    {
        for (int j = 0; j < DIM; j++)
        {
            if (a[i][j] == 1)
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(i + 0.5f, 0.5f, j + 0.5f));
                RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.67, 0.5, 0.13));
                continue;
            }
            if (a[i][j] == 2)
            {
                float eX, eZ;
                glm::mat4 modelMatrix = glm::mat4(1);
                if (ePos[curr].first == 0)
                {
                    if (ePos[curr].second + enemySpeed * deltaTimeSeconds > ceil(ePos[curr].second) - 0.125f)
                    {
                        ePos[curr].first = 1;
                    }
                    else
                    {
                        ePos[curr].second = ePos[curr].second + enemySpeed * deltaTimeSeconds;
                    }
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(ePos[curr].second, 0.5f, j + 0.5f));
                    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f));
                    RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, glm::vec3(0.33, 0.20, 0.55));
                    eX = ePos[curr].second;
                    eZ = j + 0.5f;

                    if (ePos[curr].second < i || ePos[curr].second > i + 1)
                    {
                        ePos[curr].first = -1;
                    }

                }
                else if (ePos[curr].first == 1)
                {
                    if (ePos[curr].second - enemySpeed * deltaTimeSeconds < floor(ePos[curr].second) + 0.125f)
                    {
                        ePos[curr].first = 0;
                    }
                    else
                    {
                        ePos[curr].second = ePos[curr].second - enemySpeed * deltaTimeSeconds;
                    }
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(ePos[curr].second, 0.5f, j + 0.5f));
                    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f));
                    RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, glm::vec3(0.33, 0.20, 0.55));
                    eX = ePos[curr].second;
                    eZ = j + 0.5f;
                    if (ePos[curr].second < i || ePos[curr].second > i + 1)
                    {
                        ePos[curr].first = -1;
                    }
                }
                else if (ePos[curr].first == 2)
                {
                    if (ePos[curr].second + enemySpeed * deltaTimeSeconds > ceil(ePos[curr].second) - 0.125f)
                    {
                        ePos[curr].first = 3;
                    }
                    else
                    {
                        ePos[curr].second = ePos[curr].second + enemySpeed * deltaTimeSeconds;
                    }
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(i + 0.5f, 0.5f, ePos[curr].second));
                    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f));
                    RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, glm::vec3(0.33, 0.20, 0.55));
                    eX = i + 0.5f;
                    eZ = ePos[curr].second;
                    if (ePos[curr].second < j || ePos[curr].second > j + 1)
                    {
                        ePos[curr].first = -1;
                    }
                }
                else if(ePos[curr].first == 3)
                {
                    if (ePos[curr].second - enemySpeed * deltaTimeSeconds < floor(ePos[curr].second) + 0.125f)
                    {
                        ePos[curr].first = 2;
                    }
                    else
                    {
                        ePos[curr].second = ePos[curr].second - enemySpeed * deltaTimeSeconds;
                    }
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(i + 0.5f, 0.5f, ePos[curr].second));
                    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f));
                    RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, glm::vec3(0.33, 0.20, 0.55));
                    eX = i + 0.5f;
                    eZ = ePos[curr].second;
                    if (ePos[curr].second < j || ePos[curr].second > j + 1)
                    {
                        ePos[curr].first = -1;
                    }
                }

                if (ePos[curr].first != -1)
                {
                    if ((corner1.first >= eX - 0.125f && corner1.first <= eX + 0.125f &&
                        corner1.second >= eZ - 0.125f && corner1.second <= eZ + 0.125f) ||
                        (corner2.first >= eX - 0.125f && corner2.first <= eX + 0.125f &&
                            corner2.second >= eZ - 0.125f && corner2.second <= eZ + 0.125f) ||
                        (corner3.first >= eX - 0.125f && corner3.first <= eX + 0.125f &&
                            corner3.second >= eZ - 0.125f && corner3.second <= eZ + 0.125f) ||
                        (corner4.first >= eX - 0.125f && corner4.first <= eX + 0.125f &&
                            corner4.second >= eZ - 0.125f && corner4.second <= eZ + 0.125f))
                    {
                        HP -= damage;
                        cout << "HP: " << HP << "\n";
                        ePos[curr].first = -1;
                        if (HP == 0)
                        {
                            cout << "Game Over!\n";
                            exit(0);
                        }
                    }

                    if (projectileShot && bulletPos.x >= eX - 0.125f && bulletPos.x <= eX + 0.125f
                        && bulletPos.z >= eZ - 0.125f && bulletPos.z <= eZ + 0.125f
                        && bulletPos.y >= 0.5 - 0.125f && bulletPos.y <= 0.5 + 0.125f)
                    {
                        ePos[curr].first = -1;
                    }

                }

                curr++;

            }
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(i + 0.5f, 0, j + 0.5f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 0.1f, 1));
            RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.67, 0.5, 0.13));
           
        }
    }

    
    if (renderCameraTarget)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        glm::vec3 pos = camera->GetTargetPosition();
        playerX = pos.x;
        playerY = pos.y;
        modelMatrix = glm::translate(modelMatrix, glm::vec3(playerX, playerY, pos.z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.3f, 0.2f));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.1f, 0.7f, 0.4)); /////////corp
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(playerX, playerY + 0.2f, pos.z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f, 0.15f, 0.15f));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(1, 1, 1)); ///////////cap
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(playerX + 0.05f, playerY - 0.2f, pos.z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.07f, 0.5f, 0.07f));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.78, 1, 0.2)); ///////picior
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(playerX - 0.05f, playerY - 0.2f, pos.z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.07f, 0.5f, 0.07f));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.78, 1, 0.2)); ///////picior
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(playerX + 0.13f, playerY, pos.z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.07f, 0.3f, 0.07f));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(1, 1, 1)); ///////mana
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(playerX - 0.13f, playerY, pos.z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.07f, 0.3f, 0.07f));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(1, 1, 1)); ///////mana
    }

}


void Tema2::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
        
        if (window->KeyHold(GLFW_KEY_W)) {
            camera->MoveForward(cameraSpeed * deltaTime);

            glm::vec3 pos;
            if (renderCameraTarget)
            {
                pos = camera->GetTargetPosition();
            }
            else
            {
                pos = camera->position;
            }
            bool coll = false;
            pair<float, float> corner1;
            pair<float, float> corner2;
            pair<float, float> corner3;
            pair<float, float> corner4;

            corner1.first = pos.x - playerWidth;
            corner1.second = pos.z - playerWidth;
            corner2.first = pos.x - playerWidth;
            corner2.second = pos.z + playerWidth;
            corner3.first = pos.x + playerWidth;
            corner3.second = pos.z - playerWidth;
            corner4.first = pos.x + playerWidth;
            corner4.second = pos.z + playerWidth;

            

            if (corner1.first >= 0 && corner1.second >= 0 && corner1.first < DIM && corner1.second < DIM)
            {
                if (a[int(floor(corner1.first))][int(floor(corner1.second))] == 1)
                {
                    camera->MoveForward(-cameraSpeed * deltaTime);
                    coll = true;
                }
            }
            if (!coll && corner2.first >= 0 && corner2.second >= 0 && corner2.first < DIM && corner2.second < DIM)
            {
                if (a[int(floor(corner2.first))][int(floor(corner2.second))] == 1)
                {
                    camera->MoveForward(-cameraSpeed * deltaTime);
                    coll = true;
                }
            }
            if (!coll && corner3.first >= 0 && corner3.second >= 0 && corner3.first < DIM && corner3.second < DIM)
            {
                if (a[int(floor(corner3.first))][int(floor(corner3.second))] == 1)
                {
                    camera->MoveForward(-cameraSpeed * deltaTime);
                    coll = true;
                }
            }
            if (!coll && corner4.first >= 0 && corner4.second >= 0 && corner4.first < DIM && corner4.second < DIM)
            {
                if (a[int(floor(corner4.first))][int(floor(corner4.second))] == 1)
                    camera->MoveForward(-cameraSpeed * deltaTime);
            }

        }

        if (window->KeyHold(GLFW_KEY_A)) {
            camera->TranslateRight(-cameraSpeed * deltaTime);

            glm::vec3 pos;
            if (renderCameraTarget)
            {
                pos = camera->GetTargetPosition();
            }
            else
            {
                pos = camera->position;
            }
            bool coll = false;
            pair<float, float> corner1;
            pair<float, float> corner2;
            pair<float, float> corner3;
            pair<float, float> corner4;

            corner1.first = pos.x - playerWidth;
            corner1.second = pos.z - playerWidth;
            corner2.first = pos.x - playerWidth;
            corner2.second = pos.z + playerWidth;
            corner3.first = pos.x + playerWidth;
            corner3.second = pos.z - playerWidth;
            corner4.first = pos.x + playerWidth;
            corner4.second = pos.z + playerWidth;

            if (corner1.first >= 0 && corner1.second >= 0 && corner1.first < DIM && corner1.second < DIM)
            {
                if (a[int(floor(corner1.first))][int(floor(corner1.second))] == 1)
                {
                    camera->TranslateRight(cameraSpeed * deltaTime);
                    coll = true;
                }
            }
            if (!coll && corner2.first >= 0 && corner2.second >= 0 && corner2.first < DIM && corner2.second < DIM)
            {
                if (a[int(floor(corner2.first))][int(floor(corner2.second))] == 1)
                {
                    camera->TranslateRight(cameraSpeed * deltaTime);
                    coll = true;
                }
            }
            if (!coll && corner3.first >= 0 && corner3.second >= 0 && corner3.first < DIM && corner3.second < DIM)
            {
                if (a[int(floor(corner3.first))][int(floor(corner3.second))] == 1)
                {
                    camera->TranslateRight(cameraSpeed * deltaTime);
                    coll = true;
                }
            }
            if (!coll && corner4.first >= 0 && corner4.second >= 0 && corner4.first < DIM && corner4.second < DIM)
            {
                if (a[int(floor(corner4.first))][int(floor(corner4.second))] == 1)
                    camera->TranslateRight(cameraSpeed * deltaTime);
            }
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            camera->MoveForward(-cameraSpeed * deltaTime);

            glm::vec3 pos;
            if (renderCameraTarget)
            {
                pos = camera->GetTargetPosition();
            }
            else
            {
                pos = camera->position;
            }
            bool coll = false;
            pair<float, float> corner1;
            pair<float, float> corner2;
            pair<float, float> corner3;
            pair<float, float> corner4;

            corner1.first = pos.x - playerWidth;
            corner1.second = pos.z - playerWidth;
            corner2.first = pos.x - playerWidth;
            corner2.second = pos.z + playerWidth;
            corner3.first = pos.x + playerWidth;
            corner3.second = pos.z - playerWidth;
            corner4.first = pos.x + playerWidth;
            corner4.second = pos.z + playerWidth;

            if (corner1.first >= 0 && corner1.second >= 0 && corner1.first < DIM && corner1.second < DIM)
            {
                if (a[int(floor(corner1.first))][int(floor(corner1.second))] == 1)
                {
                    camera->MoveForward(cameraSpeed* deltaTime);
                    coll = true;
                }
            }
            if (!coll && corner2.first >= 0 && corner2.second >= 0 && corner2.first < DIM && corner2.second < DIM)
            {
                if (a[int(floor(corner2.first))][int(floor(corner2.second))] == 1)
                {
                    camera->MoveForward(cameraSpeed* deltaTime);
                    coll = true;
                }
            }
            if (!coll && corner3.first >= 0 && corner3.second >= 0 && corner3.first < DIM && corner3.second < DIM)
            {
                if (a[int(floor(corner3.first))][int(floor(corner3.second))] == 1)
                {
                    camera->MoveForward(cameraSpeed* deltaTime);
                    coll = true;
                }
            }
            if (!coll && corner4.first >= 0 && corner4.second >= 0 && corner4.first < DIM && corner4.second < DIM)
            {
                if (a[int(floor(corner4.first))][int(floor(corner4.second))] == 1)
                    camera->MoveForward(cameraSpeed * deltaTime);
            }

        }

        if (window->KeyHold(GLFW_KEY_D)) {
            camera->TranslateRight(cameraSpeed * deltaTime);


            glm::vec3 pos;
            if (renderCameraTarget)
            {
                pos = camera->GetTargetPosition();
            }
            else
            {
                pos = camera->position;
            }
            bool coll = false;
            pair<float, float> corner1;
            pair<float, float> corner2;
            pair<float, float> corner3;
            pair<float, float> corner4;

            corner1.first = pos.x - playerWidth;
            corner1.second = pos.z - playerWidth;
            corner2.first = pos.x - playerWidth;
            corner2.second = pos.z + playerWidth;
            corner3.first = pos.x + playerWidth;
            corner3.second = pos.z - playerWidth;
            corner4.first = pos.x + playerWidth;
            corner4.second = pos.z + playerWidth;

            if (corner1.first >= 0 && corner1.second >= 0 && corner1.first < DIM && corner1.second < DIM)
            {
                if (a[int(floor(corner1.first))][int(floor(corner1.second))] == 1)
                {
                    camera->TranslateRight(-cameraSpeed * deltaTime);
                    coll = true;
                }
            }
            if (!coll && corner2.first >= 0 && corner2.second >= 0 && corner2.first < DIM && corner2.second < DIM)
            {
                if (a[int(floor(corner2.first))][int(floor(corner2.second))] == 1)
                {
                    camera->TranslateRight(-cameraSpeed * deltaTime);
                    coll = true;
                }
            }
            if (!coll && corner3.first >= 0 && corner3.second >= 0 && corner3.first < DIM && corner3.second < DIM)
            {
                if (a[int(floor(corner3.first))][int(floor(corner3.second))] == 1)
                {
                    camera->TranslateRight(-cameraSpeed * deltaTime);
                    coll = true;
                }
            }
            if (!coll && corner4.first >= 0 && corner4.second >= 0 && corner4.first < DIM && corner4.second < DIM)
            {
                if (a[int(floor(corner4.first))][int(floor(corner4.second))] == 1)
                    camera->TranslateRight(-cameraSpeed * deltaTime);
            }
        }

}


void Tema2::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_LEFT_CONTROL)
    {
        renderCameraTarget = !renderCameraTarget;
        if (!renderCameraTarget)
        {
            camera->TranslateForward(camera->distanceToTarget);
        }
        else
        {
            camera->TranslateForward(-1 * camera->distanceToTarget);
        }
    }
    if (key == GLFW_KEY_SPACE)
    {
        if (!renderCameraTarget)
        {
            if (!projectileShot)
            {
                projectileShot = true;
                bulletDir = glm::normalize(camera->forward);
                bulletPos = camera->position;
                bulletTraveled = 0;
            }
        }
    }
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (renderCameraTarget)
        {
            camera->RotateThirdPerson_OX(-sensivityOX * deltaY);
            camera->RotateThirdPerson_OY(-sensivityOY * deltaX);
        }
        else
        {
            camera->RotateFirstPerson_OX(-sensivityOX * deltaY);
            camera->RotateFirstPerson_OY(-sensivityOY * deltaX);
        }

    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}