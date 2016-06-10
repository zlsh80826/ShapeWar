import logging

from tornado.ioloop import IOLoop
from tornado.options import parse_command_line, define, options
from sqlalchemy import create_engine

from shapewar.application import make_app
from shapewar.database import Base


logger = logging.getLogger(__name__)


define('web_port', default=8888)
define('database', default='sqlite:///:memory:')


def main():
    loop = IOLoop.instance()

    # connect to database
    database_engine = create_engine(options.database)
    logger.info('using database %r', options.database)

    # create all tables
    Base.metadata.create_all(database_engine)

    # listen
    app = make_app(database_engine)
    app.listen(options.web_port)
    logger.info('running web server on port %d', options.web_port)

    loop.start()


if __name__ == '__main__':
    parse_command_line()
    main()
