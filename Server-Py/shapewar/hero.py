import logging


logger = logging.getLogger(__name__)


def apply_friction(v, a):
    if v > 0:
        return max(0, v - a)
    elif v < 0:
        return min(0, v + a)
    return 0


def limit_speed(v, limit):
    if v < -limit:
        return -limit
    if v > limit:
        return limit
    return v


class Hero:

    def __init__(self):
        self.x = 0
        self.y = 0
        self.angle = 245
        self.acc = 0.6  # acceleration
        self.friction = 0.3
        self.max_speed = 5
        self.speed_x = 0
        self.speed_y = 0
        self.max_hp = 10000
        self.current_hp = 3000
        self.experience = 0
        self.level = 1

    def accept_keys(self, W, A, S, D):
        self.speed_x = apply_friction(self.speed_x, self.friction)
        self.speed_y = apply_friction(self.speed_y, self.friction)
        self.speed_x = limit_speed(
            self.speed_x + (D - A) * self.acc,
            self.max_speed
        )
        self.speed_y = limit_speed(
            self.speed_y + (S - W) * self.acc,
            self.max_speed
        )
        self.x += self.speed_x
        self.y += self.speed_y

    def to_self_dict(self):
        return {
            'x': self.x,
            'y': self.y,
            'id': id(self),
            'maxHp': self.max_hp,
            'currentHp': self.current_hp,
            'level': self.level,
            'experience': self.experience,
            'passives': [1, 2, 1, 3, 2, 1, 4, 3],
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
