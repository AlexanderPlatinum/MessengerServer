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

CREATE TABLE conversations (
    id           INTEGER       PRIMARY KEY AUTOINCREMENT,
    user_one     INTEGER       REFERENCES users (id),
    user_two     INTEGER       REFERENCES users (id),
    last_message VARCHAR (255)
);

CREATE TABLE messages (
    id              INTEGER     PRIMARY KEY AUTOINCREMENT,
    conversation_id INTEGER     REFERENCES conversations (id),
    author_id       INTEGER     REFERENCES users (id),
    msg_text        TEXT (4096)
);
