FROM ubuntu:latest
RUN apt-get update -y
RUN apt-get install -y python-pip python-dev build-essential
ENV FLAG actf{2_und3rsc0res_h1des_n0th1ng}
EXPOSE 8899
COPY . /app
WORKDIR /app
RUN mv rename-to-.git .git
RUN pip install -r requirements.txt
ENTRYPOINT ["python"]
CMD ["index.py"]
