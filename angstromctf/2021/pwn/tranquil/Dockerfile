FROM ubuntu:20.10

RUN apt-get update
RUN apt-get install -y xinetd zsh libc6-i386

COPY xinetd.conf /etc/xinetd.d/ctf

WORKDIR /ctf
RUN mkdir /ctf/users

COPY tranquil .
COPY server.sh .
COPY flag.txt .
RUN chmod +x server.sh
RUN chmod +x tranquil
RUN chown -R 1000 /ctf

LABEL options='{"ports": {"1337/tcp": 21830}}'
EXPOSE 1337

ENTRYPOINT ["xinetd", "-dontfork", "-limit", "256"]
