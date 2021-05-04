FROM python:3.5-slim

COPY . /ctf
WORKDIR /ctf

RUN pip3 install -r requirements.txt

LABEL options='{"network":"agents-2020"}'

EXPOSE 2345

CMD ["gunicorn", "-c", "gunicorn.conf", "agents:app"]
