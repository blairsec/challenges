FROM pwn.red/jail

COPY --from=python:3.10-slim-bullseye / /srv
COPY main.py /srv/app/run
COPY flag.txt /srv/flag.txt
RUN chmod 755 /srv/app/run
