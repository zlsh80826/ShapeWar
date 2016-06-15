import cmath
import random
import logging


logger = logging.getLogger(__name__)


def value_limit(value, minimum, maximum):
    if value < minimum:
        return minimum
    if value > maximum:
        return maximum
    return value


class MovableObject:

    friction = 0.15
    x_bound = 2000
    y_bound = 1500

    def __init__(self):
        self.pos = complex(
            random.randrange(self.x_bound),
            random.randrange(self.y_bound)
        )
        self.velocity = 0j

    @property
    def x(self):
        return self.pos.real

    @property
    def y(self):
        return self.pos.imag

    def apply_friction(self):
        r, phi = cmath.polar(self.velocity)
        r = max(0, r - self.friction)
        self.velocity = cmath.rect(r, phi)

    def accelerate(self, acc):
        """apply acceleration to velocity"""
        self.velocity += acc
        r, phi = cmath.polar(self.velocity)
        r = min(r, self.max_speed)
        self.velocity = cmath.rect(r, phi)

    def limit_pos(self):
        self.pos = complex(
            value_limit(self.pos.real, 0, self.x_bound),
            value_limit(self.pos.imag, 0, self.y_bound)
        )
