import pika
import random
import time
import logging
import sys
import asyncio

conn_params = pika.ConnectionParameters('rabbitmq', 5672)
connection = pika.BlockingConnection(conn_params)
channel = connection.channel()

channel.queue_declare(queue='numbers')

logging.basicConfig(stream=sys.stdout, level=logging.INFO,
                    format="%(asctime)s - %(message)s")

async def task(i):
    while True:
        await asyncio.sleep(i)

        number = random.randint(1, 100)
        logging.info("Sending number {}".format(number))

        await asyncio.sleep(random.randint(1, 10))

        channel.basic_publish(
            exchange='', routing_key='numbers', body=str(number))

ioloop = asyncio.get_event_loop()

tasks = []
for i in range(1000):
    newTask = ioloop.create_task(task(i))
    tasks.append(newTask)

ioloop.run_until_complete(asyncio.wait(tasks))

connection.close()
