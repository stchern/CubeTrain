#include "spline.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/spline.hpp>

Spline::Spline(const std::vector<glm::vec3>& control_points): m_control_points(control_points)
{
    m_segments_length = std::vector<float>(control_points.size(), 0.0f);
    totalSplineLengthCount();
}

std::vector<glm::vec3> Spline::splineByControlPoints(float step) const
{
    std::vector<glm::vec3> spline;
    for (float t = 0; t < (float)m_control_points.size(); t += step)
        spline.push_back(pointOnLoopSpline(t));

    return spline;
}

glm::vec3 Spline::pointOnLoopSpline(float t) const
{
    const size_t size = m_control_points.size();
    const int idx1 = (int)t;
    const int idx2 = (idx1 + 1) % size;
    const int idx3 = (idx2 + 1) % size;
    const int idx0 = idx1 >= 1 ? idx1 - 1 : size - 1;
    t = t - (int)t;

    return glm::catmullRom(m_control_points[idx0], m_control_points[idx1], m_control_points[idx2], m_control_points[idx3], t);
}

glm::vec3 Spline::gradientOnLoopSpline(float t) const
{
    const size_t size = m_control_points.size();
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

    return 0.5f * (m_control_points[idx0] * q1 + m_control_points[idx1] * q2 + m_control_points[idx2] * q3 + m_control_points[idx3] * q4);
}

float Spline::segmentSplineLength(int segIdx) const
{
    float result_length = 0.0f;
    const float step = 0.005f;

    glm::vec3 prev_point = pointOnLoopSpline((float)segIdx);
    for (float t = 0 ; t < 1.0f; t += step) {
        const glm::vec3 current_point = pointOnLoopSpline((float)segIdx + t);
        result_length += glm::distance(prev_point, current_point);
        prev_point = current_point;
    }

    return result_length;
}

float Spline::normalizedOffset(float pos) const
{
    int i = 0;
    while (pos > m_segments_length[i]) {
        pos -= m_segments_length[i];
        i++;
    }

    return (float)i + pos / m_segments_length[i];
}

float Spline::totalSplineLength() const
{
    return m_total_spline_length;
}

void Spline::totalSplineLengthCount()
{
    float result_length = 0.0f;

    for (size_t segIdx = 0; segIdx < m_control_points.size(); ++segIdx) {
        m_segments_length[segIdx] = segmentSplineLength(segIdx);
        result_length += m_segments_length[segIdx];
    }
    m_total_spline_length = result_length;
}


float SplineUtils::rotationY(const Spline& spline, float offset)
{
    const glm::vec3 spline_grad = spline.gradientOnLoopSpline(offset);
    const float r = atan2(spline_grad.z, -spline_grad.x);
    const glm::vec3 target_perpendicular_position{-2.0f * std::sin(r), 0.0f, -2.0f * std::cos(r)};

    const float sign = (glm::dot(glm::normalize(target_perpendicular_position), {0.0, 0.0f, -1.0f})) > 0 ? 1.0f : -1.0f;
    const float cos = glm::dot(glm::normalize(target_perpendicular_position), {1.0, 0.0f, 0.0f});
    const float rotationY =  glm::degrees(acos(cos)) ;

    return sign * rotationY;
}
