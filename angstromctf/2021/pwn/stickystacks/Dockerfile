FROM ubuntu:20.10

RUN apt-get update
RUN apt-get install -y xinetd zsh

COPY xinetd.conf /etc/xinetd.d/ctf

WORKDIR /ctf
RUN mkdir /ctf/users

COPY stickystacks .
COPY server.sh .
COPY flag.txt .
RUN chmod +x server.sh
RUN chmod +x stickystacks
RUN chown -R 1000 /ctf

LABEL options='{"ports": {"1337/tcp": 21820}}'
EXPOSE 1337

ENTRYPOINT ["xinetd", "-dontfork", "-limit", "256"]
