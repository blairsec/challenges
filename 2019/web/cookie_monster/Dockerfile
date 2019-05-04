FROM buildkite/puppeteer

COPY . /ctf
WORKDIR /ctf

RUN npm install

EXPOSE 3000
CMD ["node", "app.js"]