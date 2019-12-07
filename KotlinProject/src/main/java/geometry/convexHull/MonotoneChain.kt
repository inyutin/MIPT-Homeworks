package geometry.convexHull

import geometry.Point
import java.util.Arrays

/**
 * Алгоритм монотонной цепи
 *
 * @see [MonotoneChain](https://www.algorithmist.com/index.php/Monotone_Chain_Convex_Hull)
 */

class MonotoneChain: ConvexHullAlgorithm {

    /**
     * Реализация алгоритма для построение выпуклой оболочки над массивом входных точек.
     *
     * Время работы - O(n lg n), где n - общее число точек на плоскости,
     *
     * @property points Массив из входных точек.
     * @return Построенная выпуклая оболочка.
     */
    override fun convexHull(points: Array<Point>): Collection<Point>  {
        if (points.size < 3) {
            throw IllegalArgumentException("Должно быть не меньше трёх точек")
        }

        val size = points.size
        var index = 0
        var result = Array<Point>(2 * size, {i -> Point(i,i)})

        Arrays.sort(points)

        // Построим нижнию часть оболочки
        for (i in 0 until size) {
            while (index >= 2 && Point.orientation(result[index - 2], result[index - 1], points[i]) <= 0) {
                index--
            }
            result[index++] = points[i]
        }

        // Построим верхнюю часть оболочки
        var newIndex = size - 2
        val oldIndex = index + 1
        while (newIndex >= 0) {
            while (index >= oldIndex && Point.orientation(result[index - 2], result[index - 1], points[newIndex]) <= 0) {
                index--
            }
            result[index++] = points[newIndex]
            newIndex--
        }

        // Удаляем дубликаты
        if (index > 1) {
            result = Arrays.copyOfRange(result, 0, index - 1)
        }

        return result.toSortedSet()
    }
}
