FROM ubuntu:xenial

RUN apt-get update
RUN apt-get install -y python python-pip libgmp3-dev
RUN apt-get install -y python-gmpy2
RUN pip install pycrypto

ADD . /app/
WORKDIR /app/

EXPOSE 3000

LABEL options='{"ports": {"3000/tcp": 18002}}'

CMD ["python", "server.py"]
