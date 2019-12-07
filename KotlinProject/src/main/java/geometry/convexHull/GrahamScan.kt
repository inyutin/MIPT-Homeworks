package geometry.convexHull

import geometry.Point
import java.util.*

/**
 * Алгоритм Грэхема
 *
 * @see [GrahamScan](https://ru.wikipedia.org/wiki/%D0%90%D0%BB%D0%B3%D0%BE%D1%80%D0%B8%D1%82%D0%BC_%D0%93%D1%80%D1%8D%D1%85%D0%B5%D0%BC%D0%B0)
 */

class GrahamScan: ConvexHullAlgorithm {

    /**
     * Реализация алгоритма для построение выпуклой оболочки над массивом входных точек.
     *
     * Время работы - O(n lg n), где n - общее число точек на плоскости,
     *
     * @property points Массив из входных точек.
     * @return Построенная выпуклая оболочка.
     */
    override fun convexHull(points: Array<Point>): Collection<Point> {
        if (points.size < 3) {
            throw IllegalArgumentException("there must be at least 3 points")
        }

        val hull = Stack<Point>()

        // Найдем крайнюю левую точку
        points.sortBy {it.y}

        // Отсортируем точки по полярному углу
        points.sortWith(Comparator {first, second ->
            compare(first, second, points[0])
        })

        hull.push(points[0])
        hull.push(points[1])

        for (i in IntRange(2, points.size - 1)) {
            var top = hull.peek()
            hull.pop()
            while (Point.orientation(hull.peek(), top, points[i]) <= 0) {
                top = hull.peek()
                hull.pop()
            }
            hull.push(top)
            hull.push(points[i])
        }

        return hull
    }
}

private fun compare (first: Point, second: Point, zeroPoint: Point) : Int {
    val dx1 = first.x - zeroPoint.x
    val dy1 = first.y - zeroPoint.y
    val dx2 = second.x - zeroPoint.x
    val dy2 = second.y - zeroPoint.y

    if (dy1 >= 0 && dy2 < 0) {
        return -1    // Первая выше; вторая ниже
    } else if (dy2 >= 0 && dy1 < 0) {
        return +1    // Первая ниже; вторая выше
    } else if (dy1 == 0 && dy2 == 0) { // Коллинеарны
        if (dx1 >= 0 && dx2 < 0) {
            return -1
        } else if (dx2 >= 0 && dx1 < 0) {
            return +1
        }
        return 0
    }
    return -Point.orientation(zeroPoint, first, second) // Обе выше или ниже
}