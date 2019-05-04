FROM mariadb
ENV MYSQL_ROOT_PASSWORD=3d5621a8dc3756ad7a97278d19c74d494db3b6963a6af0d5834c7fdf68dc8582
ENV MYSQL_USER=actf
ENV MYSQL_PASSWORD=prequelmemesarebetterthansequelmemes
ENV MYSQL_DATABASE=sequel_fan_club
LABEL options='{"network":"sequel-2018", "name":"sequel-mysql"}'
COPY setup.sql /docker-entrypoint-initdb.d/setup.sql
