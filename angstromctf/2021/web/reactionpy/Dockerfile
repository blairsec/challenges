FROM ubuntu:20.04

RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y python3 python3-pip curl gcc g++ make git libasound2-dev libnss3-dev libx11-xcb-dev libxcomposite-dev libxcursor-dev libxdamage-dev libxext-dev libxfixes-dev libxi-dev libxrandr-dev libxrender-dev libxss-dev libxtst-dev ca-certificates libglib2.0-0 libcups2-dev libdbus-1-dev libatk-bridge2.0-dev libpango1.0-dev libpangocairo-1.0-0 libcairo2-dev libatspi2.0-dev libgtk-3-dev libgdk-pixbuf2.0-dev

RUN curl -sSL https://dl-ssl.google.com/linux/linux_signing_key.pub | apt-key add - \
    && sh -c 'echo "deb [arch=amd64] http://dl.google.com/linux/chrome/deb/ stable main" >> /etc/apt/sources.list.d/google.list' \
    && apt-get update \
    && apt-get install -y google-chrome-unstable fonts-ipafont-gothic fonts-wqy-zenhei fonts-thai-tlwg fonts-kacst fonts-freefont-ttf \
      --no-install-recommends \
    && rm -rf /var/lib/apt/lists/*

RUN curl -L https://raw.githubusercontent.com/tj/n/master/bin/n -o /usr/bin/n
RUN bash /usr/bin/n 15.13.0

COPY . /app
WORKDIR /app

RUN npm install
RUN python3 -m pip install -r requirements.txt

ENV CHROME_FLAGS --no-sandbox|--disable-setuid-sandbox

EXPOSE 8080

CMD ["python3", "server.py"]
