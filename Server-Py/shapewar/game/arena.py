import json
import logging
import itertools
from tornado.websocket import WebSocketHandler, WebSocketClosedError
from tornado.ioloop import PeriodicCallback

from .entity import collision_pairs, Square, Triangle, Pentagon, Hero, Vector2d


logger = logging.getLogger(__name__)


class Arena:

    tick_time = 33  # tick time in milliseconds

    def __init__(self):
        self.clients = set()

        self.squares = [Square(i) for i in range(250)]
        self.triangles = [Triangle(i) for i in range(50)]
        self.pentagons = [Pentagon(i) for i in range(10)]

        self.tick_id = 0
        self.tick_timer = PeriodicCallback(self.tick, self.tick_time)
        self.pptid = 0
        self.perf_timer = PeriodicCallback(self.perf_callback, 1000)

    def perf_callback(self):
        self.pptid, diff = self.tick_id, self.tick_id - self.pptid
        logger.log(
            logging.INFO
            if diff > 1000 / self.tick_time - 2
            else logging.WARNING,
            '%d clients, %d ticks/s', len(self.clients), diff
        )

    def bind_application(self, application):
        if hasattr(self, 'application'):
            raise RuntimeError(
                'This arena is bounded to %r already' % self.application)
        self.application = application
        self.tick_timer.start()
        self.perf_timer.start()
        logger.info('started arena')

    def iter_all_bullets(self):
        for client in self.clients:
            yield from client.hero.bullets

    def tick(self):
        """this function is called every `self.tick_time` milliseconds
        to process each tick
        """

        for entity in itertools.chain(
            self.squares,
            self.triangles,
            self.pentagons,
            (client.hero for client in self.clients),
            self.iter_all_bullets()
        ):
            entity.tick()

        for client in self.clients:
            client.hero.action()

        target_objects = [
            obj for obj in
            itertools.chain(
                self.triangles,
                self.squares,
                self.pentagons,
                [client.hero for client in self.clients],
                self.iter_all_bullets()
            )
            if obj.alive
        ]

        for obj, obk in collision_pairs(target_objects):
            d = (obj.pos - obk.pos)
            sumr = obj.radius + obk.radius
            obj.velocity += Vector2d.from_polar((sumr - d.r) / sumr, d.phi)
            obk.velocity += Vector2d.from_polar((d.r - sumr) / sumr, d.phi)
            if obj.team != obk.team:
                obj.hit_by(obk)
                obk.hit_by(obj)

        self.send_updates()

    def send_updates(self):
        self.tick_id += 1
        self.broadcast_updates({
            "tick": self.tick_id,
            "players": [
                client.hero.to_dict() for client in self.clients
            ],
            "squares": [square.to_dict() for square in self.squares],
            "triangles": [triangle.to_dict() for triangle in self.triangles],
            "pentagons": [pentagon.to_dict() for pentagon in self.pentagons]
        })

    def broadcast_updates(self, updates):
        removal = set()
        for client in self.clients:
            try:
                client.send_updates(updates)
            except WebSocketClosedError:
                removal.add(client)
        self.clients -= removal

    def broadcast_message(self, message):
        removal = set()
        for client in self.clients:
            try:
                client.write_message(message)
            except WebSocketClosedError:
                removal.add(client)
        self.clients -= removal


arena = Arena()


class ArenaHandler(WebSocketHandler):

    def open(self):
        self.hero = Hero()
        arena.clients.add(self)
        logger.info('%s connected', self.request.remote_ip)

    def on_message(self, message):
        data = json.loads(message)
        self.hero.last_keys = data['keys']
        self.hero.mouse_down = data['mouse']
        self.hero.angle = data['angle']
        logger.debug('%s %r', self.request.remote_ip, data)

    def send_updates(self, data):
        message = dict(data)
        message['self'] = self.hero.to_self_dict()
        self.write_message(message)

    def check_origin(self, origin):
        return True

    def on_close(self):
        logger.info('%s closed', self.request.remote_ip)
