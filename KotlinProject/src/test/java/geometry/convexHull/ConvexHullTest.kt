package geometry.convexHull

import org.junit.jupiter.api.Assertions.assertEquals
import org.junit.jupiter.api.Test

import geometry.Point
import org.junit.jupiter.api.Assertions

abstract class ConvexHullTest(private val convexHullAlgorithm: ConvexHullAlgorithm) {

    @Test
    fun `First test`()  {
        val points = arrayOf(
            Point(3, 2),
            Point(2, 5),
            Point(4, 5)
        )
        val expected = points.toSortedSet()
        val hull = convexHullAlgorithm.convexHull(points)
        assertEquals(expected, hull.toSortedSet())
    }

    @Test
    fun `Second test`()  {
        val points = arrayOf(
            Point(0, 3),
            Point(2, 2),
            Point(1, 1),
            Point(2, 1),
            Point(3, 0),
            Point(0, 0),
            Point(3, 3)
        )
        val expected = sortedSetOf(
            Point(0, 3),
            Point(3, 0),
            Point(0, 0),
            Point(3, 3)
        )
        val hull = convexHullAlgorithm.convexHull(points)
        assertEquals(expected, hull.toSortedSet())
    }

    @Test
    fun `Third test`()  {
        val expected = arrayOf(
            Point(10, 3),
            Point(8, 0),
            Point(2, 0),
            Point(0, 3),
            Point(2, 6),
            Point(8, 6)
        )
        val points = expected + arrayOf(
            Point(3, 5),
            Point(3, 3),
            Point(6, 3),
            Point(6, 3),
            Point(7, 1),
            Point(8, 4)
        )
        val hull = convexHullAlgorithm.convexHull(points)
        assertEquals(expected.toSortedSet(), hull.toSortedSet())
    }


    @Test
    fun `IllegalArgumentException test 1`() {
        Assertions.assertThrows(IllegalArgumentException::class.java) {
            GiftWrapping().convexHull(emptyArray())
        }
    }

    @Test
    fun `IllegalArgumentException test 2`() {
        Assertions.assertThrows(IllegalArgumentException::class.java) {
            GiftWrapping().convexHull(arrayOf(Point(0, 0)))
        }
    }

    @Test
    fun `IllegalArgumentException test 3`() {
        Assertions.assertThrows(IllegalArgumentException::class.java) {
            GiftWrapping().convexHull(arrayOf(Point(0, 0), Point(1, 1)))
        }
    }
}



