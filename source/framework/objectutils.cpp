#include "objectutils.h"
#include "splineutils.h"

std::vector<Object*> ObjectUtils::createPoints(const std::vector<glm::vec3> control_points, Engine* engine, Mesh& sphere_mesh)
{
    std::vector<Object*> points;
    for (int i = 0; i < control_points.size(); i++)
    {
        Object *sphere = engine->createObject(&sphere_mesh);
        sphere->setColor(1, 0, 0);
        sphere->setPosition(control_points[i]);
        sphere->setScale(0.25f);
        points.push_back(sphere);
    }
    return points;
}

std::vector<Object*> ObjectUtils::createTrain(const glm::vec3 start_position, int n_cubes, Engine* engine, Mesh& cube_mesh)
{
    std::vector<Object*> train;
    glm::vec3 next_position = start_position;
    float distance_between_cubes = 5.0f;
    for (int i = 0; i < n_cubes; i++)
    {
        Object *cube = engine->createObject(&cube_mesh);
        cube->setColor(0.5, 0.5, 0.5);
        cube->setPosition(next_position);
        next_position.x += distance_between_cubes;
        cube->setScale(0.25f);
        train.push_back(cube);
    }
    return train;
}

std::vector<Object*> createSleepers(const std::vector<glm::vec3> control_points, float sleepers_step, Engine* engine, Mesh& plane_mesh)
{
    std::vector<Object*> sleepers;
    for (float t = 0; t < (float)SplineUtils::totalSplineLength(control_points); t += sleepers_step) {
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

    return sleepers;
}

void ObjectUtils::changeTrainPositions(const glm::vec3& start_position, const std::vector<glm::vec3> control_points, float offset, vector<Object *>& outTrain)
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
