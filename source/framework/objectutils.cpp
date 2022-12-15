#include "objectutils.h"
#include "splineutils.h"
#include <cmath>

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

std::vector<Object *> ObjectUtils::createTrain(const glm::vec3 start_position, int n_cubes, Engine* engine, Mesh& cube_mesh)
{
    std::vector<Object *> train;
    glm::vec3 next_position = start_position;
    const glm::vec3 train_color{0.5f, 0.5f, 0.5f};
    const glm::vec3 train_scale{0.25f};
//    float distance_between_cubes = 5.0f;
    for (int i = 0; i < n_cubes; i++)
    {
        Object *cube = engine->createObject(&cube_mesh);
        cube->setColor(train_color);
        cube->setPosition(next_position);
//        next_position.x += distance_between_cubes;
        cube->setScale(train_scale);
        train.push_back(cube);
    }

    return train;
}

void ObjectUtils::createSleepersAndRails(const std::vector<glm::vec3> control_points, int sleepers_step, float rails_step, Engine* engine, Mesh& plane_mesh)
{
    int sleeper_counter = 0;
    const glm::vec3 rails_color{0.0f, 0.0f, 0.0f};
    const glm::vec3 sleeper_color{1.0f, 0.25f, 0.10f};
    const glm::vec3 rails_scale{0.1f, 0.025f, 1.0f};
    const glm::vec3 sleeper_scale{1.0f, 0.05f, 1.0f};
    const float width_rails = 0.25f;

    for (float t = 0; t < (float)SplineUtils::totalSplineLength(control_points); t += rails_step) {
        const float offset = SplineUtils::normalizedOffset(control_points, t);
        const glm::vec3 spline_position = SplineUtils::point_on_loop_spline(control_points, offset);
        const glm::vec3 spline_grad = SplineUtils::gradient_on_loop_spline(control_points, offset);
        const float r = atan2(spline_grad.z, -spline_grad.x);
        glm::vec3 right_point{width_rails * sin(r) + spline_position.x, spline_position.y, width_rails * std::cos(r) + spline_position.z};
        glm::vec3 left_point{-width_rails * sin(r) + spline_position.x, spline_position.y, -width_rails * std::cos(r) + spline_position.z};

        const glm::vec3 target_perpendicular_position{-2.0f * std::sin(r), 0.0f, -2.0f * std::cos(r)};
        const float sign = (glm::dot(glm::normalize(target_perpendicular_position), {0.0, 0.0f, -1.0f})) > 0 ? 1.0f : -1.0f;
        const float cos = glm::dot(glm::normalize(target_perpendicular_position), {1.0, 0.0f, 0.0f});
        const float rotationY =  glm::degrees(acos(cos)) ;

        if (!sleeper_counter) {
            Object *sleeper = engine->createObject(&plane_mesh);
            sleeper->setColor(sleeper_color);
            sleeper->setPosition(spline_position);
            sleeper->setRotation(-90.0f, sign * rotationY, 0.0f);
            sleeper->setScale(sleeper_scale);
        }

        ++sleeper_counter;
        sleeper_counter %= (int)sleepers_step;

        Object *right_rail = engine->createObject(&plane_mesh);

        right_rail->setColor(rails_color);
        right_rail->setPosition(right_point);
        right_rail->setRotation(-90.0f, sign * rotationY - 90.0f, 0.0f);
        right_rail->setScale(rails_scale);

        Object *left_rail = engine->createObject(&plane_mesh);
        left_rail->setColor(rails_color);
        left_rail->setPosition(left_point);
        left_rail->setRotation(-90.0f, sign * rotationY - 90.0f, 0.0f);
        left_rail->setScale(rails_scale);
    }
}

void ObjectUtils::changeTrainPositions(const std::vector<glm::vec3> control_points, float point_idx, float total_spline_length, std::vector<Object *>& outTrain)
{
    float distance_between_cubes = 0.25f;
    for (size_t cube_idx = outTrain.size() - 1; cube_idx > 0; --cube_idx) {
        float curr_point_idx = (point_idx - distance_between_cubes * cube_idx) <= 0.0f ?
                    std::fmod(total_spline_length + point_idx - distance_between_cubes * cube_idx, total_spline_length) :
                    point_idx -  distance_between_cubes * cube_idx;
        const float offset = SplineUtils::normalizedOffset(control_points, curr_point_idx);
        const glm::vec3 spline_position = SplineUtils::point_on_loop_spline(control_points, offset);
        const float rotationY = SplineUtils::rotationY(control_points, offset);

        outTrain[cube_idx]->setPosition(spline_position);
        outTrain[cube_idx]->setRotation(0.0f, rotationY, 0.0f);
    }

    const float offset = SplineUtils::normalizedOffset(control_points, point_idx);
    const glm::vec3 start_position = SplineUtils::point_on_loop_spline(control_points, offset);
    const float rotationY = SplineUtils::rotationY(control_points, offset);
    outTrain[0]->setPosition(start_position);
    outTrain[0]->setRotation(0.0f, rotationY, 0.0f);
}
