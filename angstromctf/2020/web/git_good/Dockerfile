FROM node:13

COPY . /ctf
WORKDIR /ctf
RUN mv git .git
RUN npm install

ENV PORT 3000
EXPOSE 3000

CMD ["node", "index.js"]