FROM python:slim

COPY . /botcoin
WORKDIR /botcoin

RUN pip3 install -r requirements.txt

EXPOSE 3000

CMD ["gunicorn", "-c", "gunicorn.conf", "botcoin:app"]