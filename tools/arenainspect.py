from tornado.ioloop import IOLoop
from tornado.websocket import websocket_connect
from tornado.options import options, define, parse_command_line


define('url', default='ws://localhost:8888/arena/dummy')


async def main():
    conn = await websocket_connect(options.url)
    print(await conn.read_message())
    print(await conn.read_message())


if __name__ == '__main__':
    parse_command_line()
    loop = IOLoop.instance()
    loop.run_sync(main)
