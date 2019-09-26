FROM buildkite/puppeteer

COPY . /puppet
WORKDIR /puppet

RUN npm install

EXPOSE 3000

CMD ["npm", "start"]