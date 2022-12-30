#include "framework/engine.h"
#include "framework/utils.h"
#include "framework/spline.h"
#include "framework/train.h"
#include "framework/railway.h"
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
    plane->setColor(0.2f, 0.2f, 0.37f); // purple
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
        glm::vec3{  8.0f, -0.375f,  7.0f}
    };

//    vector<Object *> points = ObjectUtils::createPoints(control_points, engine, sphere_mesh);
//    LineDrawer path_drawer(control_points, true);

//   // spline creation
    const Spline spline(control_points);

//  // cube train creation
    const int n_cars = 8;
    const float distance_between_cars = 0.4f;
    TrainOnSpline train(n_cars, distance_between_cars, 1.0f, spline);

//  // sleepers and rails creation
    const float rails_step = 0.05f;
    const int sleepers_count = 100;
    const float rails_width = 0.8f;
    std::vector<glm::vec3> sleepers_vertices;
    std::vector<glm::vec3> inner_rails_vertices;
    std::vector<glm::vec3> outer_rails_vertices;
    Railway::createSleepersAndRails(spline, sleepers_count, rails_step, rails_width, inner_rails_vertices, outer_rails_vertices, sleepers_vertices);

//    // create rails object
    Mesh inner_rail_mesh = Railway::createRailMesh(inner_rails_vertices);
    Object* inner_rail = engine->createObject(&inner_rail_mesh);
    inner_rail->setColor(0.f, 0.f, 0.f);

    Mesh outer_rail_mesh = Railway::createRailMesh(outer_rails_vertices);
    Object* outer_rail = engine->createObject(&outer_rail_mesh);
    outer_rail->setColor(0.f, 0.f, 0.f);

//    // create sleepers object
    Mesh sleepers_mesh = Railway::createSleepersMesh(sleepers_vertices);
    Object* sleepers = engine->createObject(&sleepers_mesh);
    sleepers->setColor(0.4f, 0.1f, 0.1f);

    float speed = 3.0f;

    // main loop
    while (!engine->isDone())
    {
        engine->update();
        engine->render();

        float distance = speed * engine->getDeltaTime();
        train.move(distance);

        engine->swap();
    }

    engine->shutdown();

	return 0;
}

