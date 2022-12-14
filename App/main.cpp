#include "framework/engine.h"
#include "framework/utils.h"
#include "framework/splineutils.h"
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
    Mesh sleeper_mesh = createSleeper();

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
        sphere->setPosition(control_points[i]);
        sphere->setScale(0.25f);
        points.push_back(sphere);
    }
//    LineDrawer path_drawer(path, points.size(), true);

    std::vector<glm::vec3> spline;
    std::vector<glm::vec3> left_rails;
    std::vector<glm::vec3> right_rails;
    const float width_rails = 0.5f;
    float step = 0.00005f;
    for (float t = 0; t < (float)control_points.size(); t += step) {
        glm::vec3 spline_position = SplineUtils::point_on_loop_spline(control_points, t);
        glm::vec3 spline_grad = SplineUtils::gradient_on_loop_spline(control_points, t);
        float r = atan2(spline_grad.z, -spline_grad.x);
        glm::vec3 right_point{width_rails * sin(r) + spline_position.x, spline_position.y, width_rails * cos(r) + spline_position.z};
        glm::vec3 left_point{-width_rails * sin(r) + spline_position.x, spline_position.y, -width_rails * cos(r) + spline_position.z};

        spline.push_back(spline_position);
        left_rails.push_back(left_point);
        right_rails.push_back(right_point);
    }

    LineDrawer left_rails_drawer(left_rails, true);
    LineDrawer right_rails_drawer(right_rails, true);
    left_rails_drawer.setColor(0.0f, 0.0f, 0.0f);
    right_rails_drawer.setColor(0.0f, 0.0f, 0.0f);

    const float distance_between_cubes = 0.30f;
    glm::vec3 train_position{0.0f, -0.375f,  7.0f};
    vector<Object *> train;
    vector<Object *> sleepers;
    for (int i = 0; i < 8; i++)
    {
        Object *cube = engine->createObject(&cube_mesh);
        cube->setColor(0.5, 0.5, 0.5);
        cube->setPosition(train_position);
        train_position.x += distance_between_cubes;
        cube->setScale(0.25f);
        train.push_back(cube);
    }

    step = 0.07f;
    for (float t = 0; t < (float)control_points.size(); t += step) {
        std::vector<glm::vec3> sleeper_points;
        glm::vec3 spline_position = SplineUtils::point_on_loop_spline(control_points, t);
        glm::vec3 spline_grad = SplineUtils::gradient_on_loop_spline(control_points, t);
        float r = atan2(spline_grad.z, -spline_grad.x);
        glm::vec3 target_position{-2.0f * sin(r), 0.0f, -2.0f * cos(r)};

        float sign = (glm::dot(glm::normalize(target_position), {0.0, 0.0f, -1.0f})) > 0 ? 1.0f : -1.0f;
        float cos = glm::dot(glm::normalize(target_position), {1.0, 0.0f, 0.0f});
        float rotationY =  glm::degrees(acos(cos)) ;

        Object *plane = engine->createObject(&plane_mesh);
        plane->setColor(1.0f, 0.25f, 0.10f);
        plane->setPosition(spline_position);
        plane->setRotation(-90.0f,  sign * rotationY, 0.0f);
        plane->setScale(1.0f, 0.05f, 1.0f);
    }


    size_t pointIdx = 0;
    // main loop
    while (!engine->isDone())
    {
        engine->update();
        engine->render();

//        path_drawer.draw();
        left_rails_drawer.draw();
        right_rails_drawer.draw();
//        changeTrainPositions(control_points[pointIdx], distance_between_cubes, train);
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
