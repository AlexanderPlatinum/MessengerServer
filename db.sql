CREATE TABLE users (
    id         INTEGER       PRIMARY KEY AUTOINCREMENT
                             UNIQUE,
    first_name VARCHAR (25),
    last_name  VARCHAR (25),
    email      VARCHAR (255) UNIQUE,
    password   VARCHAR (255)
);

CREATE TABLE sessions (
    id      INTEGER      PRIMARY KEY AUTOINCREMENT
                         UNIQUE,
    token   VARCHAR (50) UNIQUE,
    user_id INTEGER      REFERENCES users (id)
);

