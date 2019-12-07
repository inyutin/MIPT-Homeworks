package geometry

/**
 * Представляем собой точку на плоскости.
 * @property x Координата по оси Ox.
 * @property x Координата по оси Oy.
 */
data class Point(val x: Int, val y: Int): Comparable<Point> {
    /**
     * Сравнение точки с [other]
     */
    override fun compareTo(other: Point): Int {
        if (x == other.x) {
            return y.compareTo(other.y)
        }

        return x.compareTo(other.x)
    }

    /**
     * Слева ли нынешняя точка от прямой заданной точками [from] и [to]
     *
     * Для этого достаточно посмотреть как направлен
     * вектор их векторного произведения
     */
    fun isLeftOfLine(from: Point, to: Point): Boolean {
        return crossProduct(from, to) > 0
    }

    /**
     * Расстояние до прямой заданной точками [from] и [to]
     * @return расстояние до прямой
     */
    fun distanceToLine(from: Point, to: Point): Double {
        return Math.abs((to.x - from.x) * (from.y - this.y) - (from.x - this.x) * (to.y - from.y)) /
               Math.sqrt(Math.pow((to.x - from.x).toDouble(), 2.0) + Math.pow((to.y - from.y).toDouble(), 2.0))
    }

    companion object {
        /**
         * Ориентация трех точек в пространстве
         *
         * @see [orientation] (https://www.geeksforgeeks.org/orientation-3-ordered-points/)
         *
         * @property first Первая точка.
         * @property second Вторая точка.
         * @property third Третья точка.
         *
         * @return < 0 : Против часовой стрелки,
         *         = 0 : Точки колинеарны
         *         > 0 : По часовой стрелки
         */
        fun orientation(first: Point, second: Point, third: Point): Int {
            return (second.y - first.y) * (third.x - second.x) -
                   (second.x - first.x) * (third.y - second.y)
        }
    }

    /**
     * Векторное произведение
     *
     * @property from Начало вектора.
     * @property to Конец вектора.
     * @return Векторное между векторами:
     *         - От точки [from] до точки [to]
     *         - От точки [from] до [this]
     *
     * Можно заметить, что это длина вектора по оси Oz, где
     * знак задает направление.
     *
     */
    private fun crossProduct(from: Point, to: Point): Int {
        return (to.x - from.x) * (this.y - from.y) -
               (to.y - from.y) * (this.x - from.x)
    }
}