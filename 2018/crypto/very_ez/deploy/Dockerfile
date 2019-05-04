FROM ubuntu:latest

RUN apt-get update && apt-get install -y xinetd python3-dev

COPY app /app

RUN echo "service app \n { \n server = /app/feistel.py \n disable = no \n socket_type = stream \n protocol = tcp \n wait = no \n bind = 0.0.0.0 \n user = root \n type = UNLISTED \n port = 9000 \n }" >> /etc/xinetd.d/app
RUN chmod +x /app/feistel.py

CMD ["sh", "-c", "service xinetd restart && while true; do sleep 30; done;"]

EXPOSE 9000

LABEL options='{"ports": {"9000/tcp": 18200}}'
