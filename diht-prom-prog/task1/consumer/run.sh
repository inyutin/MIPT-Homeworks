#! /bin/bash

until timeout 5 bash -c "cat < /dev/null > /dev/tcp/rabbitmq/5672"; do
  sleep 5
done

python consumer.py
