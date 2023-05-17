FROM pwn.red/jail

COPY --from=python:3.10-slim-bullseye / /srv
COPY jail.py /srv/app/run
COPY flag.txt /srv/app/flag.txt

RUN mv /srv/app/flag.txt /srv/app/flag-$(head -c16 /dev/urandom | od -tx1 -An | tr -d ' ').txt
RUN chmod 755 /srv/app/run

ENV JAIL_MEM=20M
