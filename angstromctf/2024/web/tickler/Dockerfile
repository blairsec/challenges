FROM node:22-bookworm-slim

WORKDIR /usr/src/app

COPY package*.json ./

RUN npm install

COPY . .

RUN npm run build-client
RUN npm run build-server

CMD ["node", "server.js"]
