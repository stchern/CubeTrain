#ifndef TRAIN_H
#define TRAIN_H
#include "framework/object.h"
#include "framework/spline.h"
#include "framework/engine.h"

struct Car {
    Car(){};
    Car(float position_on_spline, Object * object): m_position_on_spline(position_on_spline), m_object(object) {
        m_object->setColor(m_color);
        m_object->setScale(m_scale);
    }
    ~Car(){};

    float position() const {return m_position_on_spline;};
    void setPosition(float position) { m_position_on_spline = position;};
    void setObject(Object * object) {m_object = object;};
    void setObjectPosition(const glm::vec3 position) {
        m_object->setPosition(position + m_position_transform);
    }
    void setObjectRotation(const glm::quat& rotation) {
        m_object->setRotation(rotation);
    }

private:
    float m_position_on_spline = 0.0f;
    Object * m_object = nullptr;

    glm::vec3 m_color{0.5f, 0.5f, 0.5f};
    glm::vec3 m_scale{0.5f};
    glm::vec3 m_position_transform{0.0f, 0.25f, 0.0f};
};

class TrainOnSpline {

public:
    TrainOnSpline(size_t n_cars, float distance_between_cars, float start_point_on_spline, const Spline& spline);
    ~TrainOnSpline(){};

    void move(float distance);

private:
    void moveCar(int carIdx, float distance);

    std::vector<Car> m_cars;
    size_t m_number_of_cars;
    float m_distance_between_cars;
    Spline m_spline;
    Mesh m_car_mesh;
};


#endif // TRAIN_H
