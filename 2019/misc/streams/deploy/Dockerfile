FROM python:3-slim

COPY . /ctf
WORKDIR /ctf

RUN pip3 install -r requirements.txt

EXPOSE 3000

CMD ["gunicorn", "-c", "gunicorn.conf", "streams:app"]