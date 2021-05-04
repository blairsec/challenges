FROM ubuntu

RUN apt-get update
RUN apt-get install -y xinetd zsh software-properties-common
RUN add-apt-repository ppa:deadsnakes/ppa
RUN apt-get update
RUN apt-get install -y python3.7

ENV FLAG actf{h0p3_y0u_us3d_th3_f0rmu14-1985098}

LABEL options='{"ports": {"3000": 20300}}'

COPY xinetd.conf /etc/xinetd.d/ctf

WORKDIR /ctf

COPY server.sh .
COPY server.py .

RUN chmod +x server.sh

EXPOSE 3000

CMD ["xinetd", "-dontfork", "-limit", "256"]
