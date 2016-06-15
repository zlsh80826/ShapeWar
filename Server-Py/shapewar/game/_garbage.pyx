import random
from ._motion cimport MovableObject


cdef class Garbage(MovableObject):

    cdef public:
        int id
        bint visable
        double angle
        int hp
        int maxHp
        double angular_velocity
        double max_speed

    def __init__(self, id):
        super().__init__()
        self.spawn()
        self.id = id
        self.visable = False

    cpdef void tick_angle(self):
        self.angle += self.angular_velocity
        self.angle %= 360

    cpdef void spawn(self):
        super(Garbage, self).spawn()
        self.visable = True
        self.angle = random.randrange(360)
        self.hp = self.maxHp
        self.angular_velocity = random.random() - 0.5

    cpdef dict to_dict(self):
        return {
            'id': self.id,
            'x': self.x,
            'y': self.y,
            'angle': self.angle,
            'hp': self.hp,
            'maxHp': self.maxHp,
            'radius': self.radius
        }


cdef class Square(Garbage):

    def __init__(self, id):
        self.maxHp = 100
        super(Square, self).__init__(id)
        self.radius = 20
        self.max_speed = 5


cdef class Triangle(Garbage):

    def __init__(self, id):
        self.maxHp = 400
        super(Triangle, self).__init__(id)
        self.radius = 20
        self.max_speed = 5


cdef class Pentagon(Garbage):

    def __init__(self, id):
        self.maxHp = 5000
        super(Pentagon, self).__init__(id)
        self.radius = 20
        self.max_speed = 5
