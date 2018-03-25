#pragma once

#define INSERT_USER_SQL "INSERT INTO users ( first_name, last_name, email, password ) VALUES ( :first_name, :last_name, :email, :password );"
#define GET_USER_ID_BY_EMAIL_PASS "SELECT id FROM users WHERE password = :password AND email = :email;"
#define GET_USERS_SQL " SELECT id, first_name, last_name FROM users;"

#define INSERT_SESSION_SQL "INSERT INTO sessions ( token, user_id ) VALUES ( :token, :user_id );"
#define GET_USER_BY_TOKEN_SQL "SELECT user_id FROM sessions WHERE token = :token;"

#define CREATE_CONVERSATION_SQL "INSERT INTO conversations ( user_one, user_two ) VALUES ( :user_one, :user_two );"
#define CREATE_MESSAGE_SQL "INSERT INTO messages ( conversation_id, author_id, msg_text ) VALUES ( :conversation_id, :author_id, :msg_text );"

#define GET_CONVERSATIONS_SQL "SELECT c.id, u_one.first_name as u_one_first_name, u_one.last_name as u_one_last_name, u_two.first_name as u_two_first_name, u_two.last_name as u_two_last_name FROM conversations AS c LEFT JOIN users AS u_one ON u_one.id = c.user_one LEFT JOIN users AS u_two ON u_two.id = c.user_two WHERE c.user_one = :user_id OR c.user_two = :user_id;"
#define GET_MESSAGES_SQL "SELECT id, author_id, msg_text FROM messages WHERE conversation_id = :conversation_id;"
