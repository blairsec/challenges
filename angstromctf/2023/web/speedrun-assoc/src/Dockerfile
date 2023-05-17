FROM python:3.10-slim-bullseye AS app

COPY . /ctf
WORKDIR /ctf
RUN chmod 755 /ctf

RUN pip3 install -r requirements.txt

RUN groupadd -r ctf && useradd -r -g ctf ctf

USER ctf

EXPOSE 8080

CMD ["gunicorn", "-c", "gunicorn.conf", "app:app"]