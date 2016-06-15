import collections
import cmath
from . import hero
from . import garbage


RangeKey = collections.namedtuple('RangeKey', 'left right')


def x_key(obj):
    return RangeKey(obj.x - obj.radius, obj.x + obj.radius)


def y_key(obj):
    return RangeKey(obj.y - obj.radius, obj.y + obj.radius)


def range_overlaps(objects, key):
    targets = sorted(objects, key=key, reverse=True)
    while targets:
        left = targets.pop()
        for right in reversed(targets):
            if key(left).right > key(right).left:
                # ensure object order so doing set intersection will work
                # consider also: using a forzenset
                if id(left) < id(right):
                    yield (left, right)
                else:
                    yield (right, left)
            else:
                break


def bounding_box_collision_pairs(objects):
    x_overlaps = set(range_overlaps(objects, x_key))
    return (op for op in range_overlaps(objects, y_key) if op in x_overlaps)


def check_circle_collision(obj, obk):
    return \
        (obj.x - obk.x) ** 2 + (obj.y - obk.y) ** 2 < obj.radius + obk.radius


def collide(obj, obk):
    dist, phi = cmath.polar(obj.pos - obk.pos)
    rsum = obj.radius + obk.radius
    if dist > rsum:
        return
    obj.accelerate(cmath.rect((rsum - dist) / rsum / 2, phi))
    obk.accelerate(cmath.rect((dist - rsum) / rsum / 2, phi))
    obk.pos += obk.velocity
    obj.pos += obj.velocity
    if get_team(obj) != get_team(obk):
        deduct_hp(obj, obk)
        deduct_hp(obk, obj)


def get_team(obj):
    if isinstance(obj, garbage.Garbage):
        return garbage.Garbage
    if isinstance(obj, hero.Bullet):
        return obj.owner
    if isinstance(obj, hero.Hero):
        return obj


def deduct_hp(obj, by):
    obj.hp -= by.body_damage
    if obj.hp <= 0:
        obj.hp = 0
        obj.visible = False
        if isinstance(by, hero.Bullet):
            by.owner.experience += 10
        if isinstance(by, hero.Hero):
            by.experience += 10
