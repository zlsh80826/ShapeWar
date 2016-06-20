import time
from tornado.ioloop import IOLoop
from tornado.websocket import websocket_connect
from tornado.options import options, define, parse_command_line


define('url', default='ws://localhost:8888/arena/dummy')


async def main():
    conn = await websocket_connect(options.url)
    size = 0
    start_time = time.perf_counter()
    recvs = 0
    while True:
        size += len(await conn.read_message())
        recvs += 1
        print(
            '%g Mbps, %g recvs/sec' % (
                size / 1000000 * 8 / (time.perf_counter() - start_time),
                recvs / (time.perf_counter() - start_time)
            )
        )


if __name__ == '__main__':
    parse_command_line()
    loop = IOLoop.instance()
    loop.run_sync(main)
