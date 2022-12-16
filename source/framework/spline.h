#ifndef SPLINEUTILS_H
#define SPLINEUTILS_H
#include <glm/glm.hpp>
#include <vector>

namespace SplineUtils
{

std::vector<glm::vec3> splineByControlPoints(const std::vector<glm::vec3>& control_points, float step);
glm::vec3 point_on_loop_spline(const std::vector<glm::vec3>& control_points, float t);
glm::vec3 gradient_on_loop_spline(const std::vector<glm::vec3>& control_points, float t);
float totalSplineLength(const std::vector<glm::vec3>& spline);
float segmentSplineLength(const std::vector<glm::vec3>& spline, int segIdx);
float normalizedOffset(const std::vector<glm::vec3>& spline, float pos);
float rotationY(const std::vector<glm::vec3>& control_points, float offset);
};

#endif // SPLINEUTILS_H
