CREATE TABLE Criminals (Name varchar(255), Crime varchar(255));
CREATE USER readonly WITH PASSWORD 'readonlylmao';
COPY Criminals (Name, Crime) FROM stdin;
clam	He was looking at me funny.
kmh	I think he was probably hacking something maybe.
boshua	He says 'pawn' instead of 'pwn'.
derekthesnake	He's a snake, nothing else to say.
Joe	Who is he?
John	He said that my challs were bad.
Jack	He fell down and broke his crown.
Jill	And she came tunbling after.
Jonah	I'm running out of excuses.
Jeff	Can we stop with the J names?
aplet123	He's been hoarding the flag actf{qu3r7_s7r1ng5_4r3_0u7_70_g37_y0u}
\.
GRANT CONNECT ON DATABASE "CriminalDB" TO readonly;
GRANT USAGE ON SCHEMA public TO readonly;
GRANT SELECT ON ALL TABLES IN SCHEMA public TO readonly;

