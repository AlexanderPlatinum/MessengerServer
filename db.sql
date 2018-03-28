/* Таблица пользователей */
CREATE TABLE users (
    id         INTEGER       PRIMARY KEY AUTOINCREMENT
                             UNIQUE,
    first_name VARCHAR (25),
    last_name  VARCHAR (25),
    email      VARCHAR (255) UNIQUE,
    password   VARCHAR (255)
);

/* Таблица сессий */

CREATE TABLE sessions (
    id      INTEGER      PRIMARY KEY AUTOINCREMENT
                         UNIQUE,
    token   VARCHAR (50) UNIQUE,
    user_id INTEGER      REFERENCES users (id)
);

/* Таблица диалогов */

CREATE TABLE conversations (
    id           INTEGER       PRIMARY KEY AUTOINCREMENT,
    user_one     INTEGER       REFERENCES users (id),
    user_two     INTEGER       REFERENCES users (id),
    last_message VARCHAR (255)
);

/* Таблица сообщений */

CREATE TABLE messages (
    id              INTEGER     PRIMARY KEY AUTOINCREMENT,
    conversation_id INTEGER     REFERENCES conversations (id),
    author_id       INTEGER     REFERENCES users (id),
    msg_text        TEXT (4096)
);

/* Вставка пользователя */

INSERT INTO users ( first_name, last_name, email, password ) VALUES ( :first_name, :last_name, :email, :password );

/* Выбор ид пользователя по почтовому ящику и паролю */

SELECT id FROM users WHERE password = :password AND email = :email;

/* Выборка всех пользователей */

SELECT id, first_name, last_name FROM users;


/* Вставка сессии */

INSERT INTO sessions ( token, user_id ) VALUES ( :token, :user_id );

/* Выбор пользователя по токену */

SELECT user_id FROM sessions WHERE token = :token;

/* Вставка диалога */

INSERT INTO conversations ( user_one, user_two ) VALUES ( :user_one, :user_two );

/* Вставка сообщения */

INSERT INTO messages ( conversation_id, author_id, msg_text ) VALUES ( :conversation_id, :author_id, :msg_text );

/* Выборка диалогов */

SELECT
    c.id,
    u_one.first_name AS u_one_first_name,
    u_one.last_name AS u_one_last_name,
    u_two.first_name AS u_two_first_name,
    u_two.last_name AS u_two_last_name
FROM
    conversations AS c
LEFT JOIN
    users AS u_one
ON
    u_one.id = c.user_one
LEFT JOIN
    users AS u_two
ON
    u_two.id = c.user_two
WHERE
    c.user_one = :user_id
OR
    c.user_two = :user_id;

/* Выборка сообщений из диалога */

SELECT id, author_id, msg_text FROM messages WHERE conversation_id = :conversation_id;
