FROM pwn.red/jail
COPY --from=python:3.12-slim-bookworm / /srv
RUN mkdir /srv/app
COPY challenge.py /srv/app/c.py
RUN printf "#!/bin/sh\n/usr/local/bin/python ./c.py\n" > /srv/app/run
RUN chmod +x /srv/app/run
ENV JAIL_MEM=20M
