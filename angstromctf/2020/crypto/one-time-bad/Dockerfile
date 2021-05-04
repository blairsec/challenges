FROM ubuntu

RUN apt-get update
RUN apt-get install -y xinetd zsh software-properties-common
RUN add-apt-repository ppa:deadsnakes/ppa
RUN apt-get update

ENV FLAG actf{one_time_pad_more_like_i_dont_like_crypto-1982309}

LABEL options='{"ports": {"3000": 20301}}'

COPY xinetd.conf /etc/xinetd.d/ctf

WORKDIR /ctf

COPY server.sh .
COPY server.py .

RUN chmod +x server.sh

EXPOSE 3000

CMD ["xinetd", "-dontfork", "-limit", "256"]
