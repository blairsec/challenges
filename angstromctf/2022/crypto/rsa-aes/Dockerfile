FROM python:3.10-slim-bullseye AS app
RUN pip install pyaes

FROM pwn.red/jail

COPY --from=app / /srv
COPY optimized.py /srv/app/run
RUN chmod 755 /srv/app/run
