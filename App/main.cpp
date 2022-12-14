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

void changeTrainPositions(const glm::vec3& start_position, const std::vector<glm::vec3> control_points, float offset, vector<Object *>& outTrain);

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
        sphere->setPosition(control_points[i]);
        sphere->setScale(0.25f);
        points.push_back(sphere);
    }
//    LineDrawer path_drawer(path, points.size(), true);

    std::vector<glm::vec3> spline;
    std::vector<glm::vec3> left_rails;
    std::vector<glm::vec3> right_rails;
    const float width_rails = 0.5f;
    float step = 0.005f;
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

    step = 0.3f;
    for (float t = 0; t < (float)SplineUtils::totalSplineLength(control_points); t += step) {
        std::vector<glm::vec3> sleeper_points;
        float offset = SplineUtils::normalizedOffset(control_points, t);
        glm::vec3 spline_position = SplineUtils::point_on_loop_spline(control_points, offset);
        glm::vec3 spline_grad = SplineUtils::gradient_on_loop_spline(control_points, offset);
        float r = atan2(spline_grad.z, -spline_grad.x);
        glm::vec3 target_perpendicular_position{-2.0f * std::sin(r), 0.0f, -2.0f * std::cos(r)};

        float sign = (glm::dot(glm::normalize(target_perpendicular_position), {0.0, 0.0f, -1.0f})) > 0 ? 1.0f : -1.0f;
        float cos = glm::dot(glm::normalize(target_perpendicular_position), {1.0, 0.0f, 0.0f});
        float rotationY =  glm::degrees(acos(cos)) ;

        Object *sleeper = engine->createObject(&plane_mesh);
        sleeper->setColor(1.0f, 0.25f, 0.10f);
        sleeper->setPosition(spline_position);
        sleeper->setRotation(-90.0f,  sign * rotationY, 0.0f);
        sleeper->setScale(1.0f, 0.05f, 1.0f);
        sleepers.push_back(sleeper);

//        float r1 = atan2(spline_position.z, -spline_position.x);
//        glm::vec3 target_path_position{-2.0f * std::sin(r1), 0.0f, -2.0f * std::cos(r1)};
////        sign = (glm::dot(glm::normalize(target_path_position), {0.0, 0.0f, -1.0f})) > 0 ? 1.0f : -1.0f;
////        cos = glm::dot(glm::normalize(target_path_position), {1.0, 0.0f, 0.0f});
////        rotationY =  glm::degrees(acos(cos)) ;
//        glm::vec3 right_point{width_rails * sin(r) + spline_position.x, spline_position.y, width_rails * std::cos(r) + spline_position.z};

//        Object *rail = engine->createObject(&plane_mesh);
//        rail->setColor(0.25f, 1.25f, 1.25f);
//        rail->setPosition(spline_position);
//        rail->setRotation(-90.0f, sign * rotationY, 0.0f);
//        rail->setRotation(-90.0f, -90.0f, 0.0f);
//        rail->setScale(1.0f, 0.05f, 1.0f);
    }


    const float totalSplineLength = SplineUtils::totalSplineLength(control_points);
    std::cout << " total length" << totalSplineLength << std::endl;
    float pointIdx = 0.0f ;
    float speed = 0.001f;
    // main loop
    while (!engine->isDone())
    {
        engine->update();
        engine->render();

//        path_drawer.draw();
        left_rails_drawer.draw();
        right_rails_drawer.draw();
        const float offset = SplineUtils::normalizedOffset(control_points, pointIdx);
        const glm::vec3 spline_position = SplineUtils::point_on_loop_spline(control_points, offset);
        changeTrainPositions(spline_position, control_points, offset ,train);
        pointIdx += speed;
        if (pointIdx >= totalSplineLength)
            pointIdx = 0.0f;
        engine->swap();
    }

    engine->shutdown();

	return 0;
}

void changeTrainPositions(const glm::vec3& start_position, const std::vector<glm::vec3> control_points, float offset, vector<Object *>& outTrain)
{
    float distance = 0.03f;

    for (size_t idx = outTrain.size() - 1; idx > 0; --idx) {
        const glm::vec3 spline_position = SplineUtils::point_on_loop_spline(control_points, offset - distance * idx);
        glm::vec3 spline_grad = SplineUtils::gradient_on_loop_spline(control_points, offset - distance * idx);
//        std::cout<<"idx: " << idx <<" offset: " <<  offset << std::endl;
//        std::cout<<"idx: " << idx <<" offset sub: " <<  offset - distance * idx << std::endl;
        float r = atan2(spline_grad.z, -spline_grad.x);
        glm::vec3 target_perpendicular_position{-2.0f * std::sin(r), 0.0f, -2.0f * std::cos(r)};
        float sign = (glm::dot(glm::normalize(target_perpendicular_position), {0.0, 0.0f, -1.0f})) > 0 ? 1.0f : -1.0f;
        float cos = glm::dot(glm::normalize(target_perpendicular_position), {1.0, 0.0f, 0.0f});
        float rotationY =  glm::degrees(acos(cos)) ;

        outTrain[idx]->setPosition(spline_position);
        outTrain[idx]->setRotation(0.0f, sign * rotationY, 0.0f);
    }

    glm::vec3 spline_grad = SplineUtils::gradient_on_loop_spline(control_points, offset);
    float r = atan2(spline_grad.z, -spline_grad.x);
    glm::vec3 target_perpendicular_position{-2.0f * std::sin(r), 0.0f, -2.0f * std::cos(r)};
    float sign = (glm::dot(glm::normalize(target_perpendicular_position), {0.0, 0.0f, -1.0f})) > 0 ? 1.0f : -1.0f;
    float cos = glm::dot(glm::normalize(target_perpendicular_position), {1.0, 0.0f, 0.0f});
    float rotationY =  glm::degrees(acos(cos)) ;

    outTrain[0]->setPosition(start_position);
    outTrain[0]->setRotation(0.0f, sign * rotationY, 0.0f);
}
