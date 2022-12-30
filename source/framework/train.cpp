#include "train.h"
#include "framework/engine.h"


TrainOnSpline::TrainOnSpline(size_t n_cars, float distance_between_cars, float start_point_on_spline, const Spline& spline):
    m_number_of_cars(n_cars), m_distance_between_cars(distance_between_cars), m_spline(spline), m_car_mesh(createCube())
{
    if (start_point_on_spline >= spline.totalSplineLength())
        start_point_on_spline = 0.0f;

    Engine* engine = Engine::get();

    for (int i = 0; i < m_number_of_cars; i++)
    {
        Object* object = engine->createObject(&m_car_mesh);
        float position_on_spline = start_point_on_spline * distance_between_cars * (m_number_of_cars - i - 1);
        float offset = m_spline.normalizedOffset(position_on_spline);
        Car car(offset, object);
        car.setObjectPosition(spline.pointOnLoopSpline(offset));
        m_cars.push_back(car);
        moveCar(i, position_on_spline);
    }
}

void TrainOnSpline::move(float distance)
{
    for (size_t i = 0; i < m_number_of_cars; ++i)
        moveCar(i, distance);
}

void TrainOnSpline::moveCar(int carIdx, float distance)
{
    const float result_position = m_cars[carIdx].position() + distance;
    const float offset = m_spline.normalizedOffset(result_position);
    const glm::vec3 point_on_spline = m_spline.pointOnLoopSpline(offset);
    const glm::vec3 prev_point_on_spline = m_spline.pointOnLoopSpline(offset - 0.01);

    glm::mat3 rotation_matrix;
    rotation_matrix[1] = {0.0f, 1.0f, 0.0f};
    rotation_matrix[2] = glm::normalize(prev_point_on_spline - point_on_spline);
    rotation_matrix[0] = glm::cross(-rotation_matrix[2], rotation_matrix[1]);
    glm::quat rotation = quat_cast(rotation_matrix);

    m_cars[carIdx].setObjectRotation(rotation);
    m_cars[carIdx].setObjectPosition(point_on_spline);
    m_cars[carIdx].setPosition(result_position);
}
