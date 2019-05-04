FROM elixir:1.5

ADD . /app/
WORKDIR /app/

EXPOSE 3001

LABEL options='{"ports": {"3001/tcp": 18003}}'

CMD ["mix", "run", "--no-halt"]
