FROM python:3.12-slim-bookworm AS app
RUN pip install --no-cache-dir pycryptodome libnum
FROM pwn.red/jail
COPY --from=app / /srv
COPY --chmod=755 random_rabin.py /srv/app/run
COPY flag.txt /srv/app/flag.txt
