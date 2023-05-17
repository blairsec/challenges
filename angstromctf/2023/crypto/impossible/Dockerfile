FROM python:3.10-slim-bullseye AS app

FROM pwn.red/jail

COPY --from=app / /srv
COPY impossible.py /srv/app/run
COPY flag.txt /srv/app/

RUN chmod 755 /srv/app/run