FROM python:3

COPY naas.py /app/
WORKDIR /app
RUN pip install --no-cache-dir flask gunicorn bs4

ENV URL https://naas.2019.chall.actf.co
EXPOSE 8000
CMD ["gunicorn", "-b", "0.0.0.0:8000", "naas:app"]
