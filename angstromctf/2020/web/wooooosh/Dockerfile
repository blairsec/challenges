FROM node:13

COPY app /ctf
WORKDIR /ctf
EXPOSE 3000
ENV PORT 3000
ENV FLAG actf{w0000sh_1s_th3_s0und_0f_th3_r3qu3st_fly1ng_p4st_th3_fr0nt3nd}

RUN npm install
CMD ["node", "index.js"]
