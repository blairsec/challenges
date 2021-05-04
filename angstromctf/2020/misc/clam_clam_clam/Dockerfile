FROM aplet123/node-xinetd:node13

COPY app /ctf
WORKDIR /ctf

COPY clam_clam_clam.service /etc/xinetd.d/ctf

RUN npm install

LABEL options='{"ports": {"3000/tcp": 20204}}'

EXPOSE 3000
