FROM python:3.10-slim-bullseye AS app
RUN pip install pycryptodome

FROM pwn.red/jail

COPY --from=app / /srv
COPY fault.py /srv/app/run
RUN chmod 755 /srv/app/run
