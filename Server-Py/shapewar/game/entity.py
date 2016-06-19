import random
import itertools
import collections
import math
from .abilities import PropertyMixin, Abilities

import pyximport
pyximport.install()  # noqa
from ._entity import Vector2d, Entity, collision_pairs

__all__ = ('Vector2d', 'Garbage', 'collision_pairs', 'Hero', 'Bullet')


class Garbage(Entity):

    max_speed = float('inf')
    friction = 0.2
    team = None

    def __init__(self, *args, **kwargs):
        self.alive = False
        self.opacity = 0
        self.current_hp = 0
        self.radius = self.R

    def spawn(self):
        self.pos = Vector2d(
            random.random() * (self.x_max - self.x_min) + self.x_min,
            random.random() * (self.y_max - self.y_min) + self.y_min
        )
        self.angular_velocity = random.random() - 1
        self.current_hp = self.max_hp
        self.alive = True
        self.opacity = 100

    def tick(self):
        if self.alive:
            self.tick_pos()
        elif self.opacity:
            self.tick_pos()
            self.opacity -= 1
        else:
            if random.random() < self.spawn_rate:
                self.spawn()

    def killed(self, other):
        pass

    def hit_by(self, by):
        if self.alive:
            self.current_hp -= by.body_damage
            if self.current_hp < 0:
                self.alive = False
                by.killed(self)


class Square(Garbage):

    body_damage = 10
    rewarding_experience = 10
    max_hp = 100
    R = 20
    spawn_rate = 0.008


class Triangle(Garbage):

    body_damage = 15
    rewarding_experience = 50
    max_hp = 300
    R = 20
    spawn_rate = 0.008


class Pentagon(Garbage):

    body_damage = 25
    rewarding_experience = 250
    max_hp = 1000
    R = 25
    spawn_rate = 0.008


class Hero(PropertyMixin, Entity):

    spawn_counter = itertools.count()

    def __new__(cls):
        return super().__new__(cls, next(cls.spawn_counter), radius=20.)

    def __init__(self):
        self.silll_points = 0
        self.levels = collections.defaultdict(int)
        self.abilities = Abilities(self)
        self.radius = 30
        self.spawn()

        self.bullets = [Bullet(id=i, owner=self) for i in range(200)]
        self.available_bullets = list(self.bullets)

        self.last_keys = {'W': False, 'A': False, 'S': False, 'D': False}
        self.mouse_down = False
        self.angle = 0
        self.friction = 0.2

        self.team = self

    def spawn(self):
        self.current_hp = self.max_hp
        self.pos = Vector2d(
            random.random() * (self.x_max - self.x_min) + self.x_min,
            random.random() * (self.y_max - self.y_min) + self.y_min
        )
        self.alive = True
        self.opacity = 100
        self.cooldown = 0
        self.level = 1
        self.experience = 0

    def tick(self):
        if self.alive:
            self.tick_pos()
        elif self.opacity:
            self.tick_pos()
            self.opacity -= 1

    def action(self):
        if not self.alive:
            return
        if self.cooldown > 0:
            self.cooldown -= 1
        elif self.mouse_down:
            self.shoot(self.available_bullets.pop())
            self.cooldown += self.reload

        self.accept_keys(**self.last_keys)
        self.current_hp = min(self.max_hp, self.current_hp + self.hp_regen)

    def shoot(self, bullet):
        bullet.spawn(
            max_hp=self.bullet_hp,
            body_damage=self.bullet_damage,
            pos=(
                self.pos +
                Vector2d.from_polar(self.radius, math.radians(self.angle))
            ),
            velocity=(
                Vector2d.from_polar(
                    self.bullet_speed,
                    math.radians(self.angle)
                )
            ),
        )

    @property
    def acc(self):
        return self.max_speed / 5

    def accept_keys(self, W, A, S, D):
        if W or A or S or D:
            acceleration = Vector2d(D - A, S - W)
            acceleration.r = self.acc
            self.velocity += acceleration

    def to_self_dict(self):
        return {
            'x': self.pos.x,
            'y': self.pos.y,
            'id': self.id,
            'maxHp': self.max_hp,
            'currentHp': self.current_hp,
            'level': self.level,
            'experience': self.experience,
            'passives': [ability.level for ability in self.abilities],
            'bullets': [bullet.to_dict() for bullet in self.bullets]
        }

    def to_dict(self):
        ret = super().to_dict()
        ret['bullets'] = [
            bullet.to_dict() for bullet in self.bullets if bullet.alive
        ]
        return ret

    def killed(self, other):
        self.experience += other.rewarding_experience

    def hit_by(self, by):
        if self.alive:
            self.current_hp -= by.body_damage
            if self.current_hp < 0:
                self.alive = False
                by.killed(self)


class Bullet(Entity):

    def __new__(cls, *args, owner, **kwargs):
        return super().__new__(cls, *args, **kwargs)

    def __init__(self, *args, owner, **kwargs):
        self.owner = owner
        self.radius = 20
        self.alive = False
        self.friction = 0
        self.max_speed = float('inf')
        self.opacity = 100
        self.max_hp = 0
        self.current_hp = 0

    @property
    def team(self):
        return self.owner

    def spawn(self, *, max_hp, body_damage, pos, velocity):
        self.max_hp = max_hp
        self.current_hp = max_hp
        self.body_damage = body_damage
        self.pos = pos
        self.velocity = velocity
        self.alive = True
        self.timeout = 100
        self.opacity = 100

    def tick(self):
        if self.alive:
            self.tick_pos()
        elif self.opacity:
            self.tick_pos()
            self.opacity -= 1
            if not self.opacity:
                self.owner.available_bullets.append(self)

    def killed(self, other):
        self.owner.killed(other)

    def hit_by(self, by):
        if self.alive:
            self.current_hp -= by.body_damage
            if self.current_hp < 0:
                self.alive = False
                # by.killed(self)
                # killing a bullet rewards nothing
