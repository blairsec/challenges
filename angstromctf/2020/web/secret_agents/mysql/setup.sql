DROP DATABASE IF EXISTS AgentsDB;


CREATE DATABASE AgentsDB;

CREATE USER 'readonly'@'%' IDENTIFIED BY 'readonlylmao';

USE AgentsDB;


CREATE TABLE Agents (Name varchar(255),
UA varchar(255));

GRANT SELECT ON Agents TO 'readonly'@'%';

FLUSH PRIVILEGES;

SET SESSION wait_timeout=10;
SET SESSION interactive_timeout=10;


INSERT INTO `Agents` VALUES ('GRU',' AAAA'),
('vector but elon musk\'s brother',' shrek V'),
('actf{nyoom_1_4m_sp33d}','123ujklaguhjnkadsuop98uij2k3l4mlgjaisdopgjk;mlkl;k234lkjhasd89f709iok23jkdf keyboard bash op'),('wile e. coyote','neeeeeeeeeeeeeeeeeeeeeeeeum'),
('PLANKTON','I WANT THAT KRABBY PATTY'),
('mort','bzzzzzzztaaaa'),
('shaggy, destroyer of worlds','bzzz\';lhjkdjsjkldfgzzzztaaaa'),
('matt, lord of fitness','hiiiii whatis our aaa'),
('admin', 'nice try but this is harder :p');

FLUSH TABLES WITH READ LOCK;
SET GLOBAL read_only = 1;
