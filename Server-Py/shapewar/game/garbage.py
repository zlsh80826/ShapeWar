import random
from . import motion


class Garbage(motion.MovableObject):

    def __init__(self, id):
        super().__init__()
        self.spawn()
        self.id = id
        self.visable = False

    def tick_angle(self):
        self.angle += self.angular_velocity
        self.angle %= 360

    def spawn(self):
        super().spawn()
        self.visable = True
        self.angle = random.randrange(360)
        self.hp = self.maxHp
        self.angular_velocity = random.random() - 0.5

    def to_dict(self):
        return {
            'id': self.id,
            'x': self.x,
            'y': self.y,
            'angle': self.angle,
            'hp': self.hp,
            'maxHp': self.maxHp,
            'radius': self.radius
        }


class Square(Garbage):

    radius = 5
    maxHp = 100


class Triangle(Garbage):

    radius = 5
    maxHp = 400


class Pentagon(Garbage):

    radius = 10
    maxHp = 5000
