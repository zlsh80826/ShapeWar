import zlib
import json
import logging
import itertools
import struct
from concurrent.futures import ThreadPoolExecutor
from tornado.websocket import WebSocketHandler, WebSocketClosedError
from tornado.ioloop import PeriodicCallback
from tornado import gen

from .hero import Hero
from . import garbage
from .collision import collision_pairs, collide


logger = logging.getLogger(__name__)


zlib_compress_executor = ThreadPoolExecutor(4)


@gen.coroutine
def qUnompress_compatible_compression(data):
    string = json.dumps(data)
    binary = string.encode('utf-8')
    compressed = yield zlib_compress_executor.submit(zlib.compress, binary)
    bytecount = len(compressed)
    header = struct.pack('>I', bytecount)  # big-endian unsigned int
    return header + compressed


class Arena:

    tick_time = 33  # tick time in milliseconds

    def __init__(self):
        self.clients = set()

        self.squares = [garbage.Square(i) for i in range(250)]
        self.triangles = [garbage.Triangle(i) for i in range(50)]
        self.pentagons = [garbage.Pentagon(i) for i in range(10)]

        self.max_score = 0

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

    async def tick(self):
        """this function is called every `self.tick_time` milliseconds
        to process each tick
        """
        self.tick_id += 1

        for obj in itertools.chain(
            self.squares,
            self.triangles,
            self.pentagons,
            [client.hero for client in self.clients],
            self.iter_all_bullets()
        ):
            obj.tick()

        target_objects = [
            obj for obj in
            itertools.chain(
                self.triangles,
                self.squares,
                self.pentagons,
                [client.hero for client in self.clients],
                self.iter_all_bullets()
            )
            if obj.visible
        ]

        for obj, obk in collision_pairs(target_objects):
            collide(obj, obk)

        for client in self.clients:
            client.hero.action()

        self.max_score = max(
            (client.hero.score for client in self.clients),
            default=1
        )

        selfs = self.broadcast_updates()
        globalz = self.send_updates()
        await globalz
        await selfs

    async def send_updates(self):
        self.broadcast_message(await qUnompress_compatible_compression({
            "tick": self.tick_id,
            "players": [
                client.hero.to_player_dict() for client in self.clients
            ],
            "squares": [square.to_dict() for square in self.squares],
            "triangles": [triangle.to_dict() for triangle in self.triangles],
            "pentagons": [pentagon.to_dict() for pentagon in self.pentagons]
        }))

    async def broadcast_updates(self):
        removal = set()
        for client in list(self.clients):
            try:
                client.write_message(
                    await qUnompress_compatible_compression(
                        {'self': client.hero.to_self_dict(
                            max_score=self.max_score)}
                    ),
                    binary=True
                )
            except WebSocketClosedError:
                removal.add(client)
        self.clients -= removal

    def broadcast_message(self, message):
        removal = set()
        for client in list(self.clients):
            try:
                client.write_message(message, binary=True)
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
        self.hero.last_control = data
        self.hero.handle_upgrade()
        logger.debug('%s %r', self.request.remote_ip, data)

    def check_origin(self, origin):
        return True

    def on_close(self):
        logger.info('%s closed', self.request.remote_ip)
