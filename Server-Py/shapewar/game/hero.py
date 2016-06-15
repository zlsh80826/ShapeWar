from collections import defaultdict
import math
import cmath
import logging

from . import abilities
from .motion import MovableObject


logger = logging.getLogger(__name__)


class Hero(abilities.PropertyMixin, MovableObject):

    visible = True

    def __init__(self):
        self.skill_points = 0
        self.levels = defaultdict(int)
        self.abilities = abilities.Abilities(self)

        super().__init__()

        self.radius = 30
        self.angle = 0
        self.acc = 0.6  # acceleration
        self.current_hp = self.max_hp
        self.experience = 0
        self.level = 1

    def accept_keys(self, W, A, S, D):
        self.apply_friction()
        if W or A or S or D:
            self.accelerate(cmath.rect(self.acc, math.atan2(S - W, D - A)))
        self.pos += self.velocity
        self.limit_pos()

    def to_self_dict(self):
        return {
            'x': self.x,
            'y': self.y,
            'id': id(self),
            'maxHp': self.max_hp,
            'currentHp': self.current_hp,
            'level': self.level,
            'experience': self.experience,
            'passives': [ability.level for ability in self.abilities],
        }

    def to_player_dict(self):
        return {
            'x': self.x,
            'y': self.y,
            'id': id(self),
            'maxHp': self.max_hp,
            'currentHp': self.current_hp,
            'angle': self.angle
        }

    def shoot(self, bullet):
        bullet.hp = self.bullet_hp
        bullet.damage = self.bullet_damage
        bullet.pos = self.pos + cmath.rect(
            self.radius, math.radians(self.angle))
        bullet.velocity = cmath.rect(
            self.bullet_speed, math.radians(self.angle)) + self.velocity
        bullet.visible = True
        bullet.timeout = 50 * 2


class Bullet(MovableObject):

    def __init__(self, id, bqueue):
        self.bqueue = bqueue
        self.id = id
        self.radius = 20
        self.angle = 0
        self.visible = False
        self.maxHp = 1
        self.hp = 1
        self.timeout = 0
        self.owner = None
        self.friction = 0
        self.max_speed = 10000

    def tick(self):
        self.pos += self.velocity
        self.timeout -= 1
        if self.timeout == 0 or self.hp < 0:
            self.visible = False
            self.bqueue.append(self)

    def to_dict(self):
        return {
            'id': self.id,
            'x': self.x,
            'y': self.y,
            'hp': self.hp,
            'maxHp': self.maxHp,
            'radius': self.radius,
            'visible': self.visible
        }
