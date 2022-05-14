FROM pwn.red/jail

COPY --from=debian:bullseye-slim / /srv
COPY run.sh /srv/app/run
COPY number-game flag.txt /srv/app/

RUN chmod 755 /srv/app/run
