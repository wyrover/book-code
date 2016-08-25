#ifndef __PLANE_H
#define __PLANE_H

#include "vector.h"

class CPlane
{
public:
    CVector N;      // plane normal
    float  D;      // plane shift constant

    // constructors

    // Ax + By + Cz - D = 0
    CPlane(scalar_t a = 1, scalar_t b = 0, scalar_t c = 0, scalar_t d = 0)
    {
        N = CVector(a, b, c);
        D = d;
    }

    // instantiate a plane with normal Normal and D = d
    CPlane(const CVector& normal, scalar_t d = 0)
    {
        N = normal;
        D = d;
    }

    // instantiate a copy of Plane
    CPlane(const CPlane& plane)
    {
        N = plane.N;
        D = plane.D;
    }

    // instantiate a plane with three points
    CPlane(const CVector& vertexA, const CVector& vertexB, const CVector& vertexC)
    {
        CVector normalA((vertexC - vertexA) | 1.0);  // unit normal of C - A
        CVector normalB((vertexC - vertexB) | 1.0);  // unit normal of C - B
        N = (normalA ^ normalB) | 1.0;    // normalize cross product
        D = -vertexA % N;                 // calculate distance
    }

    // assignment operator
    const CPlane& operator=(const CPlane& plane)
    {
        N = plane.N;
        D = plane.D;
        return *this;
    }

    // equality operator
    const bool operator==(const CPlane& plane) const
    {
        return N == plane.N && D == plane.D;
    }

    // inequality operator
    const bool operator!=(const CPlane& plane) const
    {
        return !(*this == plane);
    }

    // is point on this plane?
    const bool inline PointOnPlane(const CVector& point) const
    {
        return DistanceToPlane(point) == 0;
    }

    // return the distance of point to the plane
    const float inline DistanceToPlane(const CVector& point) const
    {
        return N % point + D;
    }

    // return the intersection point of the ray to this plane
    const CVector inline RayIntersection(const CVector& rayPos, const CVector& rayDir) const
    {
        const scalar_t a = N % rayDir;

        if (a == 0)
            return rayPos;     // ray is parallel to plane

        return rayPos - rayDir * (DistanceToPlane(rayPos) / a);
    }
};

#endif    //__PLANE_H