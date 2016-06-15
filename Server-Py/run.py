#!venv/bin/python

import logging

from tornado.ioloop import IOLoop
from tornado.options import parse_command_line, define, options
from sqlalchemy import create_engine

from shapewar.application import make_app
from shapewar.database import Base
from shapewar.game.arena import arena


logger = logging.getLogger(__name__)


define('port', default=8888)
define('database', default='sqlite:///:memory:')


def main():
    loop = IOLoop.instance()

    # connect to database
    database_engine = create_engine(options.database)
    logger.info('using database %r', options.database)

    # create all tables
    Base.metadata.create_all(database_engine)

    app = make_app(database_engine)

    # listen on configured port
    app.listen(options.port)
    logger.info('running server on port %d', options.port)

    # bind arena
    arena.bind_application(app)

    loop.start()


if __name__ == '__main__':
    parse_command_line()
    main()
