FROM python:3.10-slim-bullseye AS app
RUN pip install qrcode

FROM pwn.red/jail

COPY --from=app / /srv
COPY mask.py /srv/app/run
RUN chmod 755 /srv/app/run
