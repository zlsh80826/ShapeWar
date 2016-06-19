import random
from ._motion cimport MovableObject


cdef class Garbage(MovableObject):

    cdef public:
        int id
        bint visible
        double angle
        int hp
        int maxHp
        double angular_velocity
        double max_speed
        int body_damage
        int rewarding_experience
        int respawn_cooldown
        double spawn_rate
        object team

    def __init__(self, id):
        super().__init__()
        self.spawn()
        self.id = id
        self.friction /= 5
        self.visible = False
        self.team = None

    cpdef void tick_angle(self):
        self.angle += self.angular_velocity
        self.angle %= 360

    cpdef void spawn(self):
        super(Garbage, self).spawn()
        self.visible = True
        self.angle = random.randrange(360)
        self.hp = self.maxHp
        self.angular_velocity = random.random() - 0.5
        self.respawn_cooldown = 100

    cpdef dict to_dict(self):
        return {
            'id': self.id,
            'x': self.x,
            'y': self.y,
            'angle': self.angle,
            'hp': self.hp,
            'maxHp': self.maxHp,
            'radius': self.radius,
            'visible': self.visible
        }

    cpdef void tick(self):
        if self.visible:
            self.tick_angle()
            self.tick_pos()
        elif self.respawn_cooldown:
            self.respawn_cooldown -= 1
        else:
            if random.random() < self.spawn_rate:
                self.spawn()

    cpdef void killed(self, other):
        pass


cdef class Square(Garbage):

    def __init__(self, id):
        self.maxHp = 100
        self.hp = 100
        super(Square, self).__init__(id)
        self.radius = 20
        self.max_speed = 5
        self.body_damage = 10
        self.spawn_rate = 0.008
        self.rewarding_experience = 10


cdef class Triangle(Garbage):

    def __init__(self, id):
        self.maxHp = 300
        self.hp = 300
        super(Triangle, self).__init__(id)
        self.radius = 20
        self.max_speed = 5
        self.body_damage = 15
        self.spawn_rate = 0.008
        self.rewarding_experience = 30


cdef class Pentagon(Garbage):

    def __init__(self, id):
        self.maxHp = 1000
        self.hp = 1000
        super(Pentagon, self).__init__(id)
        self.radius = 25
        self.max_speed = 5
        self.body_damage = 25
        self.spawn_rate = 0.008
        self.rewarding_experience = 250
