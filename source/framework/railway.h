#ifndef RAILWAY_H
#define RAILWAY_H
#include "framework/spline.h"
#include "framework/mesh.h"

namespace Railway
{

void createSleepersAndRails(const Spline& spline, int sleepers_count,  float rails_step, float rails_width,
                            std::vector<glm::vec3>& inner_rails, std::vector<glm::vec3>& outer_rails, std::vector<glm::vec3>& sleepers);

Mesh createRailMesh(const std::vector<glm::vec3>& rails);
Mesh createSleepersMesh(const std::vector<glm::vec3>& sleepers);

};

#endif // RAILWAY_H
