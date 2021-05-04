FROM aplet123/python-xinetd:python3.6

COPY . /ctf
WORKDIR /ctf

COPY rsaotp.service /etc/xinetd.d/ctf

RUN pip3 install -r requirements.txt

LABEL options='{"ports": {"3000/tcp": 20600}}'

EXPOSE 3000
