FROM python:3.12-bookworm

RUN pip install aiohttp --no-cache-dir

WORKDIR /app
COPY main.py server.py ./

USER nobody

EXPOSE 3000

CMD ["python", "main.py"]
