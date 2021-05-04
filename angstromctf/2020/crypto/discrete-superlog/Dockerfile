FROM aplet123/python-xinetd:python3.6

COPY . /ctf
WORKDIR /ctf

COPY discrete-superlog.service /etc/xinetd.d/ctf

RUN apt-get update
RUN apt-get install -y libgmp-dev libmpfr-dev libmpc-dev
RUN pip3 install -r requirements.txt

LABEL options='{"ports": {"3000/tcp": 20603}}'

EXPOSE 3000