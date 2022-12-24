#ifndef SPLINE_H
#define SPLINE_H
#include <glm/glm.hpp>
#include <vector>

class Spline {

public:
    Spline(const std::vector<glm::vec3>& control_points);
    std::vector<glm::vec3> splineByControlPoints(float step) const;
    glm::vec3 pointOnLoopSpline(float t) const;
    glm::vec3 gradientOnLoopSpline(float t) const;
    float segmentSplineLength(int segIdx) const;
    float normalizedOffset(float pos) const;
    float totalSplineLength() const;
private:
    void totalSplineLengthCount();
    float segmentSplineLengthCount(int segIdx);
    float m_total_spline_length;
    std::vector<glm::vec3> m_control_points;
    std::vector<float> m_segments_length;
};

namespace SplineUtils
{
    float rotationY(const Spline& spline, float offset);
};

#endif // SPLINE_H
