#ifndef OBJECTUTILS_H
#define OBJECTUTILS_H
#include "framework/object.h"
#include "framework/engine.h"
#include "framework/spline.h"

namespace ObjectUtils
{
    std::vector<Object*> createPoints(const std::vector<glm::vec3> control_points, Engine* engine, Mesh& sphere_mesh);
    std::vector<Object *> createTrain(const glm::vec3 start_position, int n_cubes, Engine* engine, Mesh& cube_mesh);
    void createSleepersAndRails(const Spline& spline, int sleepers_step, float rails_step, Engine* engine, Mesh& plane_mesh);
    void changeTrainPositions(const Spline& spline, float point_idx, std::vector<Object *>& outTrain);
};

#endif // OBJECTUTILS_H
