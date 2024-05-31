FROM pwn.red/jail

COPY --from=debian:bookworm-slim / /srv
COPY --chmod=755 exam /srv/app/run
COPY flag.txt /srv/app/flag.txt
