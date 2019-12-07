package geometry.convexHull

import geometry.Point

/**
 * Interface for Convex hull algorithms.
 */

interface ConvexHullAlgorithm {
    fun convexHull(points: Array<Point>): Collection<Point>
}