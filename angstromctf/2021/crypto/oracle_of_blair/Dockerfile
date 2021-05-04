FROM python:3.8

RUN apt-get update && apt-get install -y xinetd

COPY app /app
WORKDIR /app

RUN pip install -r requirements.txt

LABEL options='{"ports": {"21112": 21112}}'

COPY chall.xinetd /etc/xinetd.d/chall
CMD ["xinetd", "-dontfork"]
