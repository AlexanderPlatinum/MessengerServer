#pragma once

#define INSERT_USER_SQL "INSERT INTO users ( first_name, last_name, email, password ) VALUES ( :first_name, :last_name, :email, :password );"
#define GET_USER_ID_BY_EMAIL_PASS "SELECT id FROM users WHERE password = :password AND email = :email;"

#define INSERT_SESSION_SQL "INSERT INTO sessions ( token, user_id ) VALUES ( :token, :user_id );"
