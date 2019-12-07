package geometry.convexHull

import geometry.Point
import java.util.Stack

/**
 * Алгоритм заворачивания подарка
 *
 * @see [GiftWrapping](https://ru.wikipedia.org/wiki/%D0%90%D0%BB%D0%B3%D0%BE%D1%80%D0%B8%D1%82%D0%BC_%D0%94%D0%B6%D0%B0%D1%80%D0%B2%D0%B8%D1%81%D0%B0)
 */
class GiftWrapping: ConvexHullAlgorithm {

    /**
     * Реализация алгоритма для построение выпуклой оболочки над массивом входных точек.
     *
     * Время работы - O(hn), где n - общее число точек на плоскости,
     * h - число точек в выпуклой оболочке.
     *
     * @property points Массив из входных точек.
     * @return Построенная выпуклая оболочка.
     */
    override fun convexHull(points: Array<Point>): Collection<Point> {
        if (points.size < 3) {
            throw IllegalArgumentException("Должно быть не меньше трёх точек")
        }

        val hull = Stack<Point>()

        // Находим самую крайнюю левую точку.
        var leftIndex = 0
        points.indices.asSequence().filter{points[it].x < points[leftIndex].x}.forEach{leftIndex = it}

        // Начинаем двигаться против часовой стрелке, пока
        // не вернемся в исходную точку, т.е. пока оболочка не будет
        // построена.
        var currentIndex = leftIndex
        do {
            // Добавим нынешнюю точку в ответ
            hull.push(points[currentIndex])

            // Ищем новую точку, она должна находится
            // как можно правее относительно нынешней точки.
            var newIndex = (currentIndex+1) % points.size
            points.indices.asSequence()
                    .filter{Point.orientation(points[currentIndex], points[it], points[newIndex]) < 0}
                    .forEach{newIndex = it}

            // Новая точка становится текущей и будет добавлена в результат
            currentIndex = newIndex

        } while (currentIndex != leftIndex)

        return hull
    }
}