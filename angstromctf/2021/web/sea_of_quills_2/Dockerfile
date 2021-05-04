FROM ruby:2.7.2

WORKDIR /ctf

RUN mkdir /ctf/views


COPY app.rb .
COPY quills.db .
COPY views/ /ctf/views/


RUN gem install sinatra -v 2.1.0
RUN gem install sqlite3 puma

EXPOSE 4567

ENV RACK_ENV production

RUN chmod 444 quills.db

CMD ["ruby", "app.rb"]
# CMD /bin/bash
