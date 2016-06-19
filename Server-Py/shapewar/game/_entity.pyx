from libc.math cimport hypot, atan2, INFINITY


cdef class Vector2d:

    cdef public:
        double x
        double y

    def __cinit__(self, double x = 0, double y = 0):
        self.x = x
        self.y = y

    property r:
        def __get__(self):
            return hypot(self.x, self.y)

        def __set__(self, double value):
            cdef double distance = hypot(self.x, self.y)
            if not distance:
                return
            cdef double mult = value / distance
            self.x *= mult
            self.y *= mult

    property phi:
        def __get__(self):
            return atan2(self.y, self.x)

    def __add__(self, Vector2d other not None):
        return Vector2d(self.x + other.x, self.y + other.y)

    def __sub__(self, Vector2d other not None):
        return Vector2d(self.x - other.x, self.y - other.y)

    def __repr__(self):
        return 'Vector2d(%r, %r)' % (self.x, self.y)

    def __richcmp__(self, other, int op):
        if not isinstance(other, Vector2d) or op != 2:
            return NotImplemented
        return self.x == other.x and self.y == other.y


cdef class Entity:

    cdef public:
        Vector2d pos
        double radius
        double angle
        Vector2d velocity
        double friction
        double max_speed
        bint visible
    cdef readonly:
        int id
        int x_min
        int x_max
        int y_min
        int y_max

    def __cinit__(
        self,
        int id,
        double x,
        double y,
        double radius,
        double angle = 0,
        int x_min = 0,
        int x_max = 4000,
        int y_min = 0,
        int y_max = 5000,
        Vector2d velocity = None,
        double friction = 0,
        double max_speed = INFINITY,
        bint visible = False,
        **kwargs
    ):
        self.id = id
        self.pos = Vector2d(x, y)
        self.radius = radius
        self.angle = angle
        self.friction = friction
        self.x_min = x_min
        self.x_max = x_max
        self.y_min = y_min
        self.y_max = y_max
        assert x_min < x_max, 'x_min (%i) >= x_max (%i)' % (x_min, x_max)
        assert y_min < y_max, 'y_min (%i) >= y_max (%i)' % (y_min, y_max)

        if velocity is None:
            self.velocity = Vector2d()
        else:
            self.velocity = velocity

        self.max_speed = max_speed
        self.visible = visible

    cdef void bound_and_bounce(self):
        if self.pos.x < self.x_min:
            self.pos.x = self.x_min
            self.velocity.x *= -1
        elif self.pos.x > self.x_max:
            self.pos.x = self.x_max
            self.velocity.x *= -1
        if self.pos.y < self.y_min:
            self.pos.y = self.y_min
            self.velocity.y *= -1
        elif self.pos.y > self.y_max:
            self.pos.y = self.y_max
            self.velocity.y *= -1

    cdef void apply_friction(self):
        cdef double reduced_speed = self.velocity.r - self.friction
        if reduced_speed < 0:
            self.velocity.x = self.velocity.y = 0
        else:
            self.velocity.r = reduced_speed

    cpdef void tick_pos(self):
        if self.velocity.r > self.max_speed:
            self.velocity.r = self.max_speed
        self.pos += self.velocity
        self.bound_and_bounce()
        self.apply_friction()

    cpdef dict to_dict(self):
        return {
            'id': self.id,
            'x': self.x,
            'y': self.y,
            'radius': self.radius,
            'maxHp': self.max_hp,
            'currentHp': self.current_hp,
            'angle': self.angle,
            'visible': self.visible
        }


cpdef tuple x_key(Entity obj):
    return (obj.x - obj.radius, obj.x + obj.radius)


def collision_pairs(objects):
    cdef list targets = sorted(objects, key=x_key, reverse=True)
    cdef Entity left, right
    while targets:
        left = targets.pop()
        for right in reversed(targets):
            cdef double distance = left.radius + right.radius
            if right.x - left.x < distance:
                if abs(left.y - right.y) < distance:
                    if (
                        (left.x - right.x) ** 2 + (left.y + right.y) ** 2
                        < distance ** 2
                    ):
                        yield (left, right)
