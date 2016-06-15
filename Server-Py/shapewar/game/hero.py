from collections import defaultdict
import math
import cmath
import logging

from . import abilities
from .motion import MovableObject


logger = logging.getLogger(__name__)


class Hero(abilities.PropertyMixin, MovableObject):

    def __init__(self):
        self.skill_points = 0
        self.levels = defaultdict(int)
        self.abilities = abilities.Abilities(self)

        super().__init__()

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
