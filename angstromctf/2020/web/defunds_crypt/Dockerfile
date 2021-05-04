FROM aplet123/php-nginx:php7-nginx1.10

COPY app /var/www
RUN mkdir -p /var/www/memories
RUN chmod 770 /var/www/memories
RUN chgrp www-data /var/www/memories
COPY flag.txt /
RUN chgrp www-data /flag.txt

EXPOSE 80

COPY cleardir.cron /etc/cron.d/cleardir
RUN chmod 0644 /etc/cron.d/cleardir
RUN crontab /etc/cron.d/cleardir
