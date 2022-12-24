#ifndef RENDER_GFXLIB_H
#define RENDER_GFXLIB_H

/**
 * @file    ViewLibGfx.h
 * @author  lyzirving
 * @date    2022-12-23
 * @brief   this head file is designed to help computation in graphics.
 *          namespace gfx2d is mainly used for 2d case, while namespace gfx3d is mainly used for 3d case.
 */

#include <cmath>

#include "glm/glm.hpp"

namespace gfx
{
    namespace gfx2d
    {
        /**
         * @brief       template method used to normalize 2d vector
         * @param item  2d vector that needs to be normalized
         */
        template <class T>
        void normalize(T &item)
        {
            float len = std::sqrt(std::pow(item.x, 2.f) + std::pow(item.y, 2.f));
            if(len < 0.00001f)
            {
                len = 1.f;
            }
            item.x /= len;
            item.y /= len;
        }

        /**
         * @brief       calculate the right handed side normal of a 2d vector
         *              note that this method takes effect in right-handed coordinate system
         * @param from  2d pt that is the start
         * @param end   2d pt that is the end
         * @param out   right handed side normal
         */
        template <class T>
        void rhsNormal(const T &from, const T &to, T &out)
        {
            glm::vec2 diff;
            diff.x = to.x - from.x;
            diff.y = to.y - from.y;
            out.x = diff.y;
            out.y = -diff.x;
            normalize(out);
        }

        /**
         * @brief       calculate the left handed side normal of a 2d vector
         *              note that this method takes effect in right-handed coordinate system
         * @param from  2d pt that is the start
         * @param end   2d pt that is the end
         * @param out   left handed side normal
         */
        template <class T>
        void lhsNormal(const T &from, const T &to, T &out)
        {
            rhsNormal(from, to, out);
            out.x = -out.x;
            out.y = -out.y;
        }

        /**
         * @brief       calculate dot product of 2d vector
         *              note that this method takes effect in right-handed coordinate system
         * @param vec0
         * @param vec1
         * @return      cos(theta), theta is the absolute angle between vec0 to vec1, and theta is between [0, pi].
         *              theta can be figured out by std::acos.
         */
        template <class T>
        float dotProduct(const T &vec0, const T &vec1)
        {
            glm::vec2 lhs, rhs;

            lhs.x = vec0.x;
            lhs.y = vec0.y;

            rhs.x = vec1.x;
            rhs.y = vec1.y;

            normalize(lhs);
            normalize(rhs);

            return float(lhs.x * rhs.x + lhs.y * rhs.y);
        }

        /**
         * @brief       calculate out product of 2d vector
         *              note that this method takes effect in right-handed coordinate system
         * @param vec0  from vector
         * @param vec1  to vector
         * @return      sin(theta), theta is the angle from vec0 to vec1, and theta is between [0, pi].
         *              sin(theta) can only be used to judge relative position between vec0 and vec1.
         *              theta can not be figured out by std::asin, because this method only return angle from [-pi/2, pi/2].
         */
        template <class T>
        float outProduct(const T &vec0, const T &vec1)
        {
            glm::vec2 from, to;
            from.x = vec0.x;
            from.y = vec0.y;

            to.x = vec1.x;
            to.y = vec1.y;

            normalize(from);
            normalize(to);

            return float(from.x * to.y - from.y * to.x);
        }

        /**
         * @brief       calculate counter-clockwise angle from vector A to vector B
         *              note that this method takes effect in right-handed coordinate system
         * @param vecA  from vector
         * @param vecB  to vector
         * @return      counter-clockwise angle from vector A to vector B
         */
        template <class T>
        float ccwAngle(const T &vecA, const T &vecB)
        {
            float dotVal = dotProduct(vecA, vecB);
            float outVal = outProduct(vecA, vecB);

            float absAngle = float(std::acos(dotVal) * 180.f / M_PI);

            if(outVal >= 0.f)
            {
                // vecA is on the right side of vecB
                return absAngle;
            }
            else
            {
                // vecA is on the left side of vecB
                return 360.f - absAngle;
            }
        }
    }

    namespace gfx3d
    {
        /**
         * @brief       template method used to normalize 3d vector
         * @param item  3d vector that needs to be normalized
         */
        template <class T>
        void normalize(T &item)
        {
            float len = std::sqrt(std::pow(item.x, 2.f) + std::pow(item.y, 2.f) + std::pow(item.z, 2.f));
            if(len < 0.00001f)
            {
                len = 1.f;
            }
            item.x /= len;
            item.y /= len;
            item.z /= len;
        }
    }
}

#endif //RENDER_GFXLIB_H
