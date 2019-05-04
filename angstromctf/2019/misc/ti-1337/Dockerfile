FROM ubuntu
RUN apt-get update
RUN apt-get install -y python3 xinetd
COPY app /app
COPY xinetd /etc/xinetd.d/app
RUN chmod +x /app/wrapper.py
RUN chmod +x /app/ti1337.py
EXPOSE 19110
LABEL options='{"ports": {"19110": 19304}}'
CMD ["/bin/sh", "-c", "service xinetd restart && sleep infinity"]
