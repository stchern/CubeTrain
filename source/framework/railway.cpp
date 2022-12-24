#include "railway.h"

void Railway::createSleepersAndRails(const Spline& spline, int sleepers_count,  float rails_step, float rails_width,
                            std::vector<glm::vec3>& inner_rails, std::vector<glm::vec3>& outer_rails, std::vector<glm::vec3>& sleepers)
{
    const glm::vec3 up{0.0f, 1.0f, 0.0f};
    const glm::vec3 sleeper_position_transform{0.0f, 0.015f, 0.0f};
    const float rails_half_width = rails_width / 2;
    const float rail_width = rails_width / 8;
    const float inner_border_offset = rails_half_width - rail_width;
    const float sleepers_half_width = rail_width / 2;
    const float sleepers_step = spline.totalSplineLength() / sleepers_count;

    float current_distance = 0.0f;
    float next_sleeper_pos = 0.0f;
    glm::vec3 prev_spline_position = spline.pointOnLoopSpline(spline.normalizedOffset(spline.totalSplineLength() - rails_step));

    for (float t = 0.0f; t < spline.totalSplineLength(); t += rails_step) {
        const float offset = spline.normalizedOffset(t);
        const glm::vec3 spline_position = spline.pointOnLoopSpline(offset);

        const glm::vec3 direction = glm::normalize(spline_position - prev_spline_position);
        const float current_length = glm::distance(spline_position, prev_spline_position);
        prev_spline_position = spline_position;

        const glm::vec3 normal = glm::cross(direction, up);
        float next_distance = current_distance + current_length;

        while (next_sleeper_pos < next_distance) {
               const float distance_to_next_sleeper = next_sleeper_pos - current_distance;
               const glm::vec3 sleeper_position = spline_position + sleeper_position_transform + direction * distance_to_next_sleeper;

               sleepers.push_back(sleeper_position - normal * rails_half_width - direction * sleepers_half_width);
               sleepers.push_back(sleeper_position + normal * rails_half_width - direction * sleepers_half_width);
               sleepers.push_back(sleeper_position + normal * rails_half_width + direction * sleepers_half_width);
               sleepers.push_back(sleeper_position - normal * rails_half_width + direction * sleepers_half_width);

               next_sleeper_pos += sleepers_step;
        }
        current_distance = next_distance;

        inner_rails.push_back(spline_position + normal * inner_border_offset);
        inner_rails.push_back(spline_position + normal * rails_half_width);
        outer_rails.push_back(spline_position - normal * rails_half_width);
        outer_rails.push_back(spline_position - normal * inner_border_offset);
    }

}

Mesh Railway::createRailMesh(const std::vector<glm::vec3>& rails)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    const size_t size = rails.size();
    const glm::vec3 normal{0.0f, 1.0f, 0.0f};
    const glm::vec2 texture_coords{0.0f, 0.0f};

    for (size_t i = 0; i < size; i += 2)
    {
        vertices.push_back({ rails[i], normal, texture_coords});
        vertices.push_back({ rails[i + 1], normal, texture_coords });

        indices.push_back(i);
        indices.push_back((i + 1) % size);
        indices.push_back((i + 2) % size);
        indices.push_back((i + 2) % size);
        indices.push_back((i + 1) % size);
        indices.push_back((i + 3) % size);
    }

    return Mesh(vertices, indices);
}

Mesh Railway::createSleepersMesh(const std::vector<glm::vec3>& sleepers)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    const size_t size = sleepers.size();
    const glm::vec3 normal{0.0f, 1.0f, 0.0f};
    const glm::vec2 texture_coords{0.0f, 0.0f};

    for (size_t i = 0; i < size; i += 4)
    {
        vertices.push_back({sleepers[i], normal, texture_coords });
        vertices.push_back({sleepers[i + 1], normal, texture_coords});
        vertices.push_back({sleepers[i + 2], normal, texture_coords});
        vertices.push_back({sleepers[i + 3], normal, texture_coords});

        indices.push_back(i);
        indices.push_back(i + 1);
        indices.push_back(i + 2);
        indices.push_back(i + 2);
        indices.push_back(i + 3);
        indices.push_back(i);
    }

    return Mesh(vertices, indices);
}
















