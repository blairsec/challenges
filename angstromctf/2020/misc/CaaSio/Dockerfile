FROM aplet123/node-xinetd:node13

COPY app /ctf
WORKDIR /ctf

COPY caas.service /etc/xinetd.d/ctf

RUN npm install

RUN useradd limited

LABEL options='{"read_only": true, "ports": {"3000/tcp": 20201}}'

EXPOSE 3000
