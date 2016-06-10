import tornado.web
from .database import DBHandlerMixin


class RequestHandler(DBHandlerMixin, tornado.web.RequestHandler):
    pass


all_handlers = [
]
