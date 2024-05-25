FROM ubuntu:latest

RUN apt-get update && apt-get install -y xinetd python-dev python-pip

COPY app /app

RUN echo "service app \n { \n server = /app/oracle.py \n disable = no \n socket_type = stream \n protocol = tcp \n wait = no \n bind = 0.0.0.0 \n user = root \n type = UNLISTED \n port = 9000 \n }" >> /etc/xinetd.d/app

WORKDIR /app
RUN pip install -r requirements.txt && chmod +x oracle.py

CMD ["sh", "-c", "service xinetd restart && while true; do sleep 30; done;"]
