FROM buildkite/puppeteer
COPY deploy /ctf
WORKDIR /ctf
ENV FLAG actf{seize_the_means_of_c0mp1l4tion}
ENV URL https://flags.2020.chall.actf.co
ENV UBI https://ubi.2020.chall.actf.co
ENV ADMIN 2881c2705e0f05729949439b5619cc78
RUN npm install
USER 1000
EXPOSE 5001
CMD ["node", "index.js"]
