import json
import random
import logging
from tornado.websocket import WebSocketHandler, WebSocketClosedError
from tornado.ioloop import PeriodicCallback

from .hero import Hero


logger = logging.getLogger(__name__)


dummy_triangles = [
    {
        "id": i,
        "x": random.randrange(1000),
        "y": random.randrange(1000),
        "angle": random.randrange(360),
        "hp": 343,
        "maxHp": 1000
    }
    for i in range(300)
]


class Arena:

    tick_time = 20  # tick time in milliseconds

    def __init__(self):
        self.clients = set()
        self.tick_id = 0
        self.tick_timer = PeriodicCallback(self.tick, self.tick_time)

    def bind_application(self, application):
        if hasattr(self, 'application'):
            raise RuntimeError(
                'This arena is bounded to %r already' % self.application)
        self.application = application
        self.tick_timer.start()
        logger.info('started arena')

    def tick(self):
        """this function is called every `self.tick_time` milliseconds
        to process each tick
        """
        self.send_updates()

    def send_updates(self):
        self.tick_id += 1
        self.broadcast_updates({
            "tick": self.tick_id,
            "players": [
                client.hero.to_player_dict() for client in self.clients
            ],
            "bullets": [
                {
                    "id": 0,
                    "x": 250,
                    "y": 260 + (self.tick_id % 50)
                },
            ],
            "squares": [
                {
                    "id": 0,
                    "x": 400,
                    "y": 240,
                    "angle": 12,
                    "hp": 43,
                    "maxHp": 100
                }
            ],
            "triangles": dummy_triangles,
            "pentagons": [
                {
                    "id": 0,
                    "x": random.randrange(500),
                    "y": random.randrange(500),
                    "angle": 34,
                    "hp": 4300,
                    "maxHp": 10000
                }
            ]
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
        logger.debug('%s %r', self.request.remote_ip, data)
        self.hero.accept_keys(**data['keys'])
        self.hero.angle = data['angle']

    def send_updates(self, data):
        message = dict(data)
        message['self'] = self.hero.to_self_dict()
        self.write_message(message)

    def check_origin(self, origin):
        return True

    def on_close(self):
        logger.info('%s closed', self.request.remote_ip)
