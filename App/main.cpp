#include "framework/engine.h"
#include "framework/utils.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/spline.hpp>
#include <iostream>
using namespace std;
using namespace glm;

/*
* Coordinate system:
* x - right
* y - up
* z - backward
*/

void goAlongPath(const float *path, float *train_positions);
void changeTrainPositions(const glm::vec3& start_position, float distance, vector<Object *>& outTrain);
glm::vec3 catmull_rom_spline_loop(const std::vector<glm::vec3>& control_points, float t);

int main()
{
	// initialization
    Engine *engine = Engine::get();
    engine->init(1600, 900, "UNIGINE Test Task");

//	// set up camera
    Camera &cam = engine->getCamera();
    cam.Position = vec3(0.0f, 12.0f, 17.0f);
    cam.Yaw = -90.0f;
    cam.Pitch = -45.0f;
    cam.UpdateCameraVectors();

//	// create shared meshes
    Mesh plane_mesh = createPlane();
    Mesh sphere_mesh = createSphere();
    Mesh cube_mesh = createCube();

//	// create background objects
    Object *plane = engine->createObject(&plane_mesh);
    plane->setColor(0.2f, 0.37f, 0.2f); // green
    plane->setPosition(0, -0.5f, 0);
    plane->setRotation(-90.0f, 0.0f, 0.0f);
    plane->setScale(20.0f);

//	// path
//    const float path[] = {
//         0.0f, -0.375f,  7.0f, // 1
//        -6.0f, -0.375f,  5.0f, // 2
//        -8.0f, -0.375f,  1.0f, // 3
//        -4.0f, -0.375f, -6.0f, // 4
//         0.0f, -0.375f, -7.0f, // 5
//         1.0f, -0.375f, -4.0f, // 6
//         4.0f, -0.375f, -3.0f, // 7
//         8.0f, -0.375f,  7.0f  // 8
//    };

    const std::vector<glm::vec3> control_points {
        glm::vec3{  0.0f, -0.375f,  7.0f},
        glm::vec3{ -6.0f, -0.375f,  5.0f},
        glm::vec3{ -8.0f, -0.375f,  1.0f},
        glm::vec3{ -4.0f, -0.375f, -6.0f},
        glm::vec3{  0.0f, -0.375f, -7.0f},
        glm::vec3{  1.0f, -0.375f, -4.0f},
        glm::vec3{  4.0f, -0.375f, -3.0f},
        glm::vec3{  8.0f, -0.375f,  7.0f},

    };
    vector<Object *> points;
    for (int i = 0; i < control_points.size(); i++)
    {
        Object *sphere = engine->createObject(&sphere_mesh);
        sphere->setColor(1, 0, 0);
//        sphere->setPosition(path[i*3], path[i*3+1], path[i*3+2]);
        sphere->setPosition(control_points[i]);
        sphere->setScale(0.25f);
        points.push_back(sphere);
    }
//    LineDrawer path_drawer(path, points.size(), true);

    std::vector<glm::vec3> spline;
    const float step = 0.00005f;
    for (float t = 0; t < (float)control_points.size(); t += step) {
        spline.push_back(catmull_rom_spline_loop(control_points, t));
    }

    LineDrawer path_drawer(spline, true);

    const float distance_between_cubes = 0.30f;
    glm::vec3 train_position{0.0f, -0.375f,  7.0f};
    vector<Object *> train;
    for (int i = 0; i < 8; i++)
    {
        Object *cube = engine->createObject(&cube_mesh);
        cube->setColor(0.5, 0.5, 0.5);
        cube->setPosition(train_position);
        train_position.x += distance_between_cubes;
        cube->setScale(0.25f);
        train.push_back(cube);
    }
    size_t pointIdx = 0;
    // main loop
    while (!engine->isDone())
    {
        engine->update();
        engine->render();

        path_drawer.draw();
//        changeTrainPositions(glm::vec3{path[pointIdx * 3], path[pointIdx * 3 + 1], path[pointIdx * 3 + 2]}, distance_between_cubes, train);
        changeTrainPositions(control_points[pointIdx], distance_between_cubes, train);
        ++pointIdx %= control_points.size();
        engine->swap();
    }

    engine->shutdown();

	return 0;
}

void goAlongPath(const float *path, float *train_positions)
{

}

void changeTrainPositions(const glm::vec3& start_position, float distance, vector<Object *>& outTrain)
{
    outTrain[0]->setPosition(start_position);
    glm::vec3 next_position = start_position;
    next_position.x += distance;
    for (size_t idx = 1; idx < outTrain.size(); ++idx) {
        outTrain[idx]->setPosition(next_position);
        next_position.x += distance;
    }
}

glm::vec3 catmull_rom_spline_loop(const std::vector<glm::vec3>& control_points, float t)
{
    // indices of the relevant control points
    const size_t size = control_points.size();
    const int idx1 = (int)t;
    const int idx2 = (idx1 + 1) % size;
    const int idx3 = (idx2 + 1) % size;
    const int idx0 = idx1 >= 1 ? idx1 - 1 : size - 1;
    const float local_t = t - (int)t;
    return glm::catmullRom(control_points[idx0], control_points[idx1], control_points[idx2], control_points[idx3], local_t);
}
