FROM ubuntu
RUN apt-get update
RUN apt-get install -y python3 xinetd
COPY app /app
COPY xinetd /etc/xinetd.d/app
RUN chmod +x /app/wrapper.py
RUN chmod +x /app/alphabeta.py
EXPOSE 3000
LABEL options='{"ports": {"3000": 19600}}'
CMD ["/bin/sh", "-c", "service xinetd restart && sleep infinity"]
