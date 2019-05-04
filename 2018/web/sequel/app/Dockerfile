FROM php:7.2-apache
RUN docker-php-ext-install mysqli
LABEL options='{"network":"sequel-2018"}'
COPY source/ /var/www/
COPY source/public/ /var/www/html/
