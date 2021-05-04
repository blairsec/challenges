FROM python:slim
RUN pip3 --no-cache-dir install flask gunicorn
WORKDIR /srv
COPY *.py ./
ENV FLAG="actf{what?_ekans_is_evolving..._into_3K4N5!}"
EXPOSE 5000
CMD gunicorn -b 0.0.0.0:5000 ekans:app
