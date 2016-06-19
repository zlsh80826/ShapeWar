import random
from .entity import Entity


class Garbage(Entity):

    def __new__(cls, id, **kw):
        return super().__new__(id, radius=20)

    def __init__(self, *args, max_hp, **kwargs):
        self.max_hp = max_hp
        self.current_hp = self.max_hp
        self.spawn()
        self.visible = False
        self.dying = 0

    def spawn(self):
        self.x = random.random() * (self.x_max - self.x_min) + self.x_min
        self.y = random.random() * (self.y_max - self.y_min) + self.y_min
        self.active_counter = 30
