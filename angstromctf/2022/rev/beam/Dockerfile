FROM pwn.red/jail

COPY --from=debian:bullseye-slim / /srv
COPY lmao /srv/app/run
COPY flag.txt /srv/app/

RUN chmod 755 /srv/app/run
