FROM aplet123/nodegres:node13-pg11

COPY . /ctf
WORKDIR /ctf

RUN npm install .

USER postgres
RUN /etc/init.d/postgresql start &&\
	psql --command "CREATE USER pgadmin WITH SUPERUSER PASSWORD 'noonecanhackthis'" &&\
	createdb -O pgadmin CriminalDB &&\
	psql CriminalDB < prepare_db.sql

EXPOSE 3000
ENV PORT 3000
ENV DBNAME CriminalDB
ENV DBUSER readonly
ENV DBPASS readonlylmao
ENV DBPORT 5432
ENV DBHOST 127.0.0.1

CMD ["sh", "-c", "pg_ctlcluster 11 main start ; npm start"]
