import json
from functools import partial
from tornado.ioloop import IOLoop
from tornado.websocket import websocket_connect
from tornado.options import options, define, parse_command_line


define('url', default='ws://localhost:8888/chat')


async def print_all_messages(connection):
    conn = await websocket_connect(options.url)
    msg = await conn.read_message()
    data = json.loads(msg)
    print(data['name'], 'said', data['message'])


if __name__ == '__main__':
    parse_command_line()
    loop = IOLoop.instance()
    connection = loop.run_sync(partial(websocket_connect, options.url))
    name = input('your name: ')
    loop.spawn_callback(partial(print_all_messages, connection))
    while True:
        connection.write_message(
            json.dumps({'name': name, 'message': input()})
        )
