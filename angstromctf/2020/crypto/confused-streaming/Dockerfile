FROM aplet123/python-xinetd:python3.6

COPY . /ctf
WORKDIR /ctf

ENV flag actf{down_to_the_decimal}
ENV seed 1732050807569
COPY confused-streaming.service /etc/xinetd.d/ctf

LABEL options='{"ports": {"3000/tcp": 20601}}'

EXPOSE 3000
