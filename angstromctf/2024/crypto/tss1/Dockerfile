FROM python:3.12-slim-bookworm AS app
RUN apt-get update && apt-get install -y gcc libgmp-dev
RUN pip install --no-cache-dir fastecdsa
FROM pwn.red/jail
COPY --from=app / /srv
COPY --chmod=755 tss1.py /srv/app/run
COPY key.txt flag.txt /srv/app/
