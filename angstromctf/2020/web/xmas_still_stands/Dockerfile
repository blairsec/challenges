FROM aplet123/puppeteer:node13

USER root
COPY . /ctf
WORKDIR /ctf

RUN npm install .

USER pptruser

EXPOSE 3000
ENV PORT 3000
ENV CAPTCHA_SITE 6LfsiNsUAAAAANqwkmlw0xCemxE8eeb9j0vXju3p
ENV CAPTCHA_SECRET 6LfsiNsUAAAAAHexIOQu_OJugfE86YxpeD7Yxbol
ENV FLAG actf{s4n1tize_y0ur_html_4nd_y0ur_h4nds}
ENV CHROME_FLAGS --no-sandbox|--disable-setuid-sandbox

LABEL options='{"init": true}'

CMD ["npm", "start"]
