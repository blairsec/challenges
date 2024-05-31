FROM python:3.12-bookworm
WORKDIR /app
COPY requirements.txt .
RUN pip install -r requirements.txt
COPY app.py flag.txt ./
USER nobody
EXPOSE 8000
CMD ["gunicorn", "-w8", "-t5", "--graceful-timeout", "0", "-b0.0.0.0", "app:app"]
