FROM python:3.10-slim-bullseye AS app
RUN pip install pycryptodome
RUN pip install binteger

FROM pwn.red/jail

COPY --from=app / /srv
COPY snap-circuits.py /srv/app/run
COPY flag.txt /srv/app/

RUN chmod 755 /srv/app/run
