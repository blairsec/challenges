FROM pwn.red/jail

COPY --from=ubuntu:22.04 / /srv
COPY --chmod=755 leftright /srv/app/run
COPY flag.txt /srv/app/flag.txt