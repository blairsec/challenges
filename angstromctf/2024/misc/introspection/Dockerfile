FROM pwn.red/jail

COPY --from=python:2-slim-buster / /srv
COPY --chmod=755 introspection.py /srv/app/run
COPY cunpickle.py unpickle.py flag.txt /srv/app/
