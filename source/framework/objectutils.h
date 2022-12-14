#ifndef OBJECTUTILS_H
#define OBJECTUTILS_H
#include "framework/object.h"
#include "framework/engine.h"

namespace ObjectUtils
{
    std::vector<Object*> createPoints(const std::vector<glm::vec3> control_points, Engine* engine, Mesh& sphere_mesh);
    std::vector<Object*> createTrain(const glm::vec3 start_position, int n_cubes, Engine* engine, Mesh& cube_mesh);
    std::vector<Object*> createSleepers(const std::vector<glm::vec3> control_points, float sleepers_step, Engine* engine, Mesh& plane_mesh);
    void changeTrainPositions(const glm::vec3& start_position, const std::vector<glm::vec3> control_points, float offset, std::vector<Object *>& outTrain);
};

#endif // OBJECTUTILS_H
