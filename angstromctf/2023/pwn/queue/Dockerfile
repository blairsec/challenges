FROM pwn.red/jail

COPY --from=ubuntu:22.04 / /srv
COPY queue /srv/app/run
COPY flag.txt /srv/app/flag.txt
RUN chmod 755 /srv/app/run
