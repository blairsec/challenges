FROM pwn.red/jail

COPY --from=debian:bookworm-slim / /srv
COPY --chmod=755 putnam /srv/app/run
