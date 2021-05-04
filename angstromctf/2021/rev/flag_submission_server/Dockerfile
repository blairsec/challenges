FROM ubuntu:20.10

RUN apt-get update

WORKDIR /ctf
COPY fss .
COPY flag.txt .
RUN chmod +x fss

LABEL options='{"ports": {"21450/tcp": 21450}}'
EXPOSE 21450
ENTRYPOINT ["./fss"]
