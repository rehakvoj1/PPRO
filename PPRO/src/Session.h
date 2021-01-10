#pragma once

#include <Wt/Auth/Login.h>
#include <Wt/Dbo/Session.h>
#include <Wt/Dbo/ptr.h>
#include <Wt/Dbo/backend/Sqlite3.h>
#include <Wt/Auth/Dbo/UserDatabase.h>

#include "DAO/User.h"

using UserDatabase = Wt::Auth::Dbo::UserDatabase<AuthInfo>;

class Session : public Wt::Dbo::Session {
public:
	static void ConfigureAuth();

	Session();

	Wt::Dbo::ptr<User> GetUser() const;

	Wt::Auth::AbstractUserDatabase& GetUsers();
	Wt::Auth::Login& login() {
		return m_login;
	}

	static const Wt::Auth::PasswordService& PasswordAuth();
	static const Wt::Auth::AuthService& Auth();
private:
	std::unique_ptr<UserDatabase> m_users;
	Wt::Auth::Login m_login;
};

