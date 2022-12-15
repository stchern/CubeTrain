#include "framework/engine.h"
#include "framework/utils.h"
#include "framework/splineutils.h"
#include "framework/objectutils.h"
#include <chrono>
#include <ctime>
#include <iostream>
using namespace std;
using namespace glm;

/*
* Coordinate system:
* x - right
* y - up
* z - backward
*/



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

    vector<Object *> points = ObjectUtils::createPoints(control_points, engine, sphere_mesh);
//    LineDrawer path_drawer(path, points.size(), true);

//      // spline creation
    const float spline_step = 0.005f;
    std::vector<glm::vec3> spline = SplineUtils::splineByControlPoints(control_points, spline_step);

//  // cube train creation
    const glm::vec3 start_position{0.0f, -0.375f,  7.0f};
    const int n_cubes = 8;
    std::vector<Object *> train = ObjectUtils::createTrain(start_position, n_cubes, engine, cube_mesh);

//      // sleepers and rails creation
    const int sleepers_step = 40;
    const float rails_step = 0.01f;
    ObjectUtils::createSleepersAndRails(control_points, sleepers_step, rails_step, engine, plane_mesh);


    const float totalSplineLength = SplineUtils::totalSplineLength(control_points);
    float pointIdx = 0.0f;
    float speed = 0.05f;

    // main loop
    while (!engine->isDone())
    {
        engine->update();
        engine->render();

        ObjectUtils::changeTrainPositions(control_points, pointIdx, totalSplineLength, train);
        pointIdx += speed;
        if (pointIdx >= totalSplineLength)
            pointIdx = 0.0f;
        engine->swap();
    }

    engine->shutdown();

	return 0;
}

