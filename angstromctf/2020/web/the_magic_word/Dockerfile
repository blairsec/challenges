FROM node:13

COPY . /ctf
WORKDIR /ctf
EXPOSE 3000
ENV PORT 3000
ENV FLAG actf{1nsp3c7_3l3m3nt_is_y0ur_b3st_fri3nd}

RUN npm install
CMD ["node", "index.js"]
