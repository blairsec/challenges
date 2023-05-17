FROM pwn.red/jail

COPY --from=debian:bullseye-slim / /srv
COPY meow /srv/app/run

RUN chmod 755 /srv/app/run
