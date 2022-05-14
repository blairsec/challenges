FROM pwn.red/jail

COPY --from=ubuntu:20.04 / /srv
COPY caniride /srv/app/run
COPY flag.txt /srv/app/flag.txt
RUN chmod 755 /srv/app/run
