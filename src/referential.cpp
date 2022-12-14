#include "referential.h"

Referential2D::Referential2D(const Vector2D& origin, const Vector2D& i)
    : m_origin(origin), m_i(i.normalized()), m_j(i.normal())
{
    m_angle = -angle(i, { 1.f, 0.f });
}

// Rotate a referential by rotating its vectors and by updating its angle (used to rotate an entity)
void Referential2D::rotate(float angle)
{
    m_angle += angle;
    m_i.rotate(angle);
    m_j.rotate(angle);
}

// Convert a local point to a global point (used to get a global convex polygon and to get an AABB)
Vector2D Referential2D::pointLocalToGlobal(const Vector2D& pointLocal) const
{
    return m_origin + m_i * pointLocal.x + m_j * pointLocal.y;
}

// Not used
Vector2D Referential2D::pointGlobalToLocal(const Vector2D& pointGlobal) const
{
    Vector2D pos = pointGlobal - m_origin;

    return { dot(pos, m_i), dot(pos, m_j) };
}

// Not used
Vector2D Referential2D::vectorLocalToGlobal(const Vector2D& vectLocal) const
{
    return m_i * vectLocal.x + m_j * vectLocal.y;
}

// Not used
Vector2D Referential2D::vectorGlobalToLocal(const Vector2D& vectGlobal) const
{
    return { dot(vectGlobal, m_i), dot(vectGlobal, m_j) };
}

// Convert a local convex to a global convex (used to get a global concave polygon)
ConvexPolygon Referential2D::convexToGlobal(const ConvexPolygon& polygonLocal) const
{
    // Return a global convex by returing a new polygon with each point convert in global
    std::vector<Vector2D> points;
    for (const Vector2D& pointLocal : polygonLocal.pts)
        points.push_back(pointLocalToGlobal(pointLocal));

    return { points };
}

// Not used
ConvexPolygon Referential2D::convexToLocal(const ConvexPolygon& polygonGlobal) const
{
    // Return a local convex by returing a new polygon with each point convert in local

    std::vector<Vector2D> points;
    for (const Vector2D& pointGlobal : polygonGlobal.pts)
        points.push_back(pointGlobalToLocal(pointGlobal));

    return { points };
}

// Convert a local concave to a global concave (used to check collisions)
ConcavePolygon Referential2D::concaveToGlobal(const ConcavePolygon& polygonLocal) const
{
    // Return a global concave by returing a new concave with each convex convert in global

    std::vector<ConvexPolygon> polygon;
    for (const ConvexPolygon& convexLocal : polygonLocal.polygon)
        polygon.push_back(convexToGlobal(convexLocal));

    return { polygon };
}

// Not used
ConcavePolygon Referential2D::concaveToLocal(const ConcavePolygon& polygonGlobal) const
{
    // Return a local concave by returing a new concave with each convex convert in local

    std::vector<ConvexPolygon> polygon;
    for (const ConvexPolygon& convexGlobal : polygonGlobal.polygon)
        polygon.push_back(convexToLocal(convexGlobal));

    return { polygon };
}