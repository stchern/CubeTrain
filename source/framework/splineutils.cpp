#include "splineutils.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/spline.hpp>


glm::vec3 SplineUtils::point_on_loop_spline(const std::vector<glm::vec3>& control_points, float t)
{
    const size_t size = control_points.size();
    const int idx1 = (int)t;
    const int idx2 = (idx1 + 1) % size;
    const int idx3 = (idx2 + 1) % size;
    const int idx0 = idx1 >= 1 ? idx1 - 1 : size - 1;
    t = t - (int)t;
    return glm::catmullRom(control_points[idx0], control_points[idx1], control_points[idx2], control_points[idx3], t);
}

glm::vec3 SplineUtils::gradient_on_loop_spline(const std::vector<glm::vec3>& control_points, float t)
{
    const size_t size = control_points.size();
    const int idx1 = (int)t;
    const int idx2 = (idx1 + 1) % size;
    const int idx3 = (idx2 + 1) % size;
    const int idx0 = idx1 >= 1 ? idx1 - 1 : size - 1;
    t = t - (int)t;

    const float t2 = t * t;
    const float q1 = -3.0f * t2 + 4.0f * t - 1.0f;
    const float q2 = 9.0f * t2 - 10.0f * t;
    const float q3 = -9.0f * t2 + 8.0f * t + 1.0f;
    const float q4 = 3.0f * t2 - 2.0f * t;

    return 0.5f * (control_points[idx0] * q1 + control_points[idx1] * q2 + control_points[idx2] * q3 + control_points[idx3] * q4);
}

float SplineUtils::totalSplineLength(const std::vector<glm::vec3>& control_points)
{
    float result_length = 0.0f;

    for (size_t segIdx = 0; segIdx < control_points.size(); ++segIdx)
        result_length += segmentSplineLength(control_points, segIdx);

    return result_length;
}

float SplineUtils::segmentSplineLength(const std::vector<glm::vec3>& control_points, int segIdx)
{
    float result_length = 0.0f;
    const float step = 0.005f;

    glm::vec3 prev_point = point_on_loop_spline(control_points, (float)segIdx);
    for (float t = 0 ; t < (float)1.0f; t += step) {
        glm::vec3 current_point = point_on_loop_spline(control_points, (float)segIdx + t);
        result_length += glm::distance(prev_point, current_point);
        prev_point = current_point;
    }

    return result_length;
}

float SplineUtils::normalizedOffset(const std::vector<glm::vec3>& control_points, float pos)
{
    int i = 0;
    while (pos > segmentSplineLength(control_points, i)) {
        pos -= segmentSplineLength(control_points, i);
        ++i;
    }

    return (float)i + pos / segmentSplineLength(control_points, i);
}
