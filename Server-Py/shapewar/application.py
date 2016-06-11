import os
import tornado.web
from torinja import Jinja2Env
from jinja2 import PackageLoader

from .database import DBAppMixin
from .handlers import all_handlers


class Application(DBAppMixin, tornado.web.Application):
    pass


def make_app(database_engine):
    return Application(
        handlers=all_handlers,
        database_engine=database_engine,
        template_loader=Jinja2Env(
            loader=PackageLoader('shapewar')
        ),
        cookie_secret=os.urandom(64)
    )
