DROP DATABASE IF EXISTS wtdbotest;
DROP USER IF EXISTS wtdbotest;

CREATE USER wtdbotest WITH PASSWORD 'wtdbotest';
CREATE DATABASE wtdbotest;
GRANT ALL PRIVILEGES ON DATABASE wtdbotest to wtdbotest;
