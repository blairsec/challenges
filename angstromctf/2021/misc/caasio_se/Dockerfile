FROM node:15-buster

RUN apt-get update && apt-get install -y xinetd

COPY app /app
WORKDIR /app

RUN npm install

COPY chall.xinetd /etc/xinetd.d/chall

LABEL options='{"ports": {"8080": "21705"}}'
CMD ["xinetd", "-dontfork"]
