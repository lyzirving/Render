#ifndef RENDER_VIEWLIB_H
#define RENDER_VIEWLIB_H

#include <cmath>
#include "glm/glm.hpp"

namespace view
{
    /**
     * @brief transfer spherical coordinate system to cartesian
     */
    void sphericalToCartesian(float r, float theta, float phi, glm::vec3 &out)
    {
        float projection = std::abs(r * std::sin(glm::radians(theta)));
        out.x = projection * std::sin(glm::radians(phi));
        out.y = r * std::cos(glm::radians(theta));
        out.z = projection * std::cos(glm::radians(phi));
    }

    /**
     * @brief transfer cartesian coordinate to spherical
     * @param out (r, theta, phi)
     */
    void cartesianToSpherical(const glm::vec3 &cartesian, glm::vec3 &out)
    {
        float len = std::sqrt(std::pow(cartesian.x, 2) +
                    std::pow(cartesian.y, 2) +
                    std::pow(cartesian.z, 2));
        out.x = len;
        out.y = glm::degrees(std::acos(cartesian.y / len));

        // tan(a+b) = (tan(a) + tan(b)) / (1 - tan(a)tan(b))
        // tan(a-b) = (tan(a) - tan(b)) / (1 + tan(a)tan(b))
        if(cartesian.z == 0.f)
        {
            if (cartesian.x == 0.f)
                out.z = 0.f;
            if(cartesian.x > 0.f)
                out.z = 90.f;
            else
                out.z = 270.f;
        }
        else
        {
            float tanTheta{0.f};
            if (cartesian.x >= 0.f && cartesian.z > 0.f)
            {
                tanTheta = cartesian.x / cartesian.z;
                out.z = glm::degrees(std::atan(tanTheta));
            }
            else if(cartesian.x >= 0.f && cartesian.z < 0.f)
            {
                tanTheta = cartesian.x / std::abs(cartesian.z);
                out.z = 180.f - glm::degrees(std::atan(tanTheta));
            }
            else if(cartesian.x <= 0.f && cartesian.z < 0.f)
            {
                tanTheta = cartesian.x / cartesian.z;
                out.z = 180.f + glm::degrees(std::atan(tanTheta));
            }
            else if(cartesian.x <= 0.f && cartesian.z > 0.f)
            {
                tanTheta = std::abs(cartesian.x) / cartesian.z;
                out.z = 360.f - glm::degrees(std::atan(tanTheta));
            }
        }
    }
}

#endif //RENDER_VIEWLIB_H
