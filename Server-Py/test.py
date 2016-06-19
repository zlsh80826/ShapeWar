import unittest
from shapewar.game.entity import Vector2d
import math


class Vector2dTest(unittest.TestCase):

    def test_math(self):
        self.assertEqual(Vector2d(8, 10), Vector2d(3, 4) + Vector2d(5, 6))
        self.assertEqual(Vector2d(3, 4).r, 5)
        v = Vector2d(3, 4)
        v.r = 10
        self.assertEqual(Vector2d(6, 8), v)
        self.assertEqual(Vector2d(0, 0), Vector2d())
        self.assertEqual(math.atan2(4, -3), Vector2d(-3, 4).phi)


if __name__ == '__main__':
    unittest.main()
