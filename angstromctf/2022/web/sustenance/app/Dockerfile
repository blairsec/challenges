FROM node:17-bullseye-slim

WORKDIR /app
COPY . .
RUN npm ci

ENV PORT=8080

ARG ADMIN_SECRET=secretpw
ENV ADMIN_SECRET=${ADMIN_SECRET}
ARG FLAG="actf{someone_is_going_to_submit_this_out_of_desperation}"
ENV FLAG=${FLAG}

EXPOSE 8080

CMD ["node", "index.js"]