FROM python:3
COPY deploy /app
WORKDIR /app
RUN pip install --no-cache-dir flask gunicorn pycryptodome
RUN curl https://raw.githubusercontent.com/pshved/timeout/master/timeout > /usr/local/bin/timeout && chmod 755 /usr/local/bin/timeout
RUN chown -R 1000 /app
USER 1000
EXPOSE 5000
CMD ["gunicorn", "-b", "0.0.0.0:5000", "app:app"]
