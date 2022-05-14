FROM pwn.red/jail

COPY --from=node:17-bullseye-slim / /srv
COPY index.js /srv/app/run
COPY flag.txt /srv/app/flag.txt
RUN chmod 755 /srv/app/run
