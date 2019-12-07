package geometry.convexHull

import geometry.Point

/**
 * Алгоритм быстрой оболочки
 *
 * @see [QuickHull](https://ru.wikipedia.org/wiki/%D0%90%D0%BB%D0%B3%D0%BE%D1%80%D0%B8%D1%82%D0%BC_%D0%B1%D1%8B%D1%81%D1%82%D1%80%D0%BE%D0%B9_%D0%BE%D0%B1%D0%BE%D0%BB%D0%BE%D1%87%D0%BA%D0%B8)
 */
class QuickHull: ConvexHullAlgorithm {

    /**
     * Реализация алгоритма для построение выпуклой оболочки над массивом входных точек.
     *
     * Время работы - O(nlgn), где n - общее число точек на плоскости
     *
     * @property points Массив из входных точек.
     * @return Построенная выпуклая оболочка.
     */
    override fun convexHull(points: Array<Point>): Collection<Point> {
        if (points.size < 3) {
            throw IllegalArgumentException("Должно быть не меньше трёх точек")
        }

        val left = points.min()!!
        val right = points.max()!!
        return quickHull(points.asList(), left, right) + quickHull(points.asList(), right, left)
    }

    private fun quickHull(points: Collection<Point>, first: Point, second: Point): Collection<Point> {
        val pointsLeftOfLine = points
                .filter { it.isLeftOfLine(first, second) }
                .map { Pair(it, it.distanceToLine(first, second)) }


        if (pointsLeftOfLine.isNotEmpty()) {
            val maxValue = pointsLeftOfLine.maxBy { it.second }!!.first
            val newPoints = pointsLeftOfLine.map { it.first }
            return quickHull(newPoints, first, maxValue) + quickHull(newPoints, maxValue, second)
        }

        return listOf(second)
    }
}