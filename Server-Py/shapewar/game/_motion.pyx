import cmath
import random
import logging


logger = logging.getLogger(__name__)


cdef double value_limit(double value, double minimum, double maximum):
    if value < minimum:
        return minimum
    if value > maximum:
        return maximum
    return value


cdef class MovableObject:

    def __init__(self):
        self.friction = 0.1
        self.x_bound = 5000
        self.y_bound = 4000
        self.spawn()

    cpdef void spawn(self):
        self.pos = complex(
            random.randrange(self.x_bound),
            random.randrange(self.y_bound)
        )
        self.velocity = 0j

    property x:
        def __get__(self):
            return self.pos.real

    property y:
        def __get__(self):
            return self.pos.imag

    cpdef void apply_friction(self):
        r, phi = cmath.polar(self.velocity)
        r = max(0, r - self.friction)
        self.velocity = cmath.rect(r, phi)

    cpdef void tick_pos(self):
        r, phi = cmath.polar(self.velocity)
        r = min(r, self.max_speed)
        self.velocity = cmath.rect(r, phi)
        self.pos += self.velocity
        self.limit_pos()

    cpdef void limit_pos(self):
        self.pos = complex(
            value_limit(self.pos.real, 0, self.x_bound),
            value_limit(self.pos.imag, 0, self.y_bound)
        )
