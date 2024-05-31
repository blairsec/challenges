FROM node:22-bookworm-slim
WORKDIR /app
COPY package.json package-lock.json ./
RUN npm ci
COPY index.js .
EXPOSE 3000
ENV TOKEN=d15453b0234690ccbb91861e FLAG=actf{b534186fa8b28780b1fcd1e95e2a2e2c}
CMD ["node", "index.js"]
