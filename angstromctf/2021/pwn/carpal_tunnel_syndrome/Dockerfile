FROM ubuntu:20.04

RUN apt-get update
RUN apt-get install -y xinetd zsh

COPY xinetd.conf /etc/xinetd.d/ctf

WORKDIR /ctf
RUN mkdir /ctf/users

COPY carpal_tunnel_syndrome .
COPY server.sh .
COPY flag.txt .
RUN chmod +x server.sh
RUN chmod +x carpal_tunnel_syndrome
RUN chown -R 1000 /ctf

LABEL options='{"ports": {"1337/tcp": 21840}}'
EXPOSE 1337

ENTRYPOINT ["xinetd", "-dontfork", "-limit", "256"]
