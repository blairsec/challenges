FROM node:22-bookworm-slim

WORKDIR /app

COPY package.json package-lock.json /app
RUN npm ci && npm cache clean --force

COPY index.js flag.txt /app/
COPY static /app/static
EXPOSE 21111

USER node
CMD ["node", "index.js"]

