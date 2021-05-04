FROM aplet123/python-xinetd:python3.6

COPY . /ctf
WORKDIR /ctf

RUN python3 -m pip install pycrypto

ENV flag actf{it_hurt_itself_in_its_confusion}
ENV seed 235278708
COPY dream-stream.service /etc/xinetd.d/ctf

LABEL options='{"ports": {"3000/tcp": 20602}}'

EXPOSE 3000