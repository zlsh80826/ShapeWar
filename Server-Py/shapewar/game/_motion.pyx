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
        self.x_min = 0
        self.x_max = 5000
        self.y_min = 0
        self.y_max = 4000
        self.spawn()

    property x:
        def __get__(self):
            return self.pos.real

    property y:
        def __get__(self):
            return self.pos.imag

    cpdef void spawn(self):
        self.pos = complex(
            random.random() * (self.x_max - self.x_min) + self.x_min,
            random.random() * (self.y_max - self.y_min) + self.y_min
        )
        self.velocity = 0j

    cpdef void apply_friction(self):
        r, phi = cmath.polar(self.velocity)
        r = max(0, r - self.friction)
        self.velocity = cmath.rect(r, phi)

    cpdef void bound_and_bounce(self):
        if self.pos.real < self.x_min or self.x_max < self.pos.real:
            self.velocity = complex(-self.velocity.real, self.velocity.imag)
        if self.pos.imag < self.y_min or self.y_max < self.pos.imag:
            self.velocity = complex(self.velocity.real, -self.velocity.imag)
        self.pos = complex(
            value_limit(self.pos.real, self.x_min, self.x_max),
            value_limit(self.pos.imag, self.y_min, self.y_max)
        )

    cpdef void tick_pos(self):
        r, phi = cmath.polar(self.velocity)
        r = min(r, self.max_speed)
        self.velocity = cmath.rect(r, phi)
        self.pos += self.velocity
        self.bound_and_bounce()

    cpdef void hit_by(self, by):
        if self.visible:
            self.hp -= by.body_damage
            if self.hp < 0:
                self.visible = False
                by.killed(self)
