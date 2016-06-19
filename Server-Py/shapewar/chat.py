import json
import logging
import tornado.websocket


logger = logging.getLogger(__name__)


class Room:

    def __init__(self):
        self.clients = set()

    def broadcast(self, message):
        for client in self.clients:
            try:
                client.write_message(message)
            except tornado.websocket.WebSocketClosedError:
                pass


class ChatHandler(tornado.websocket.WebSocketHandler):

    room = Room()

    def open(self):
        self.room.clients.add(self)
        logger.info('%s joined chat', self.request.remote_ip)

    def on_message(self, raw):
        d = json.loads(raw)
        logger.info(
            '%r from %s said: %r',
            d['name'], self.request.remote_ip, d['message'])
        self.room.broadcast(
            json.dumps({'name': d['name'], 'message': d['message']})
        )

    def on_close(self):
        self.room.clients.remove(self)
