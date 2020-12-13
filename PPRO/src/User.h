#pragma once
#include <Wt/Dbo/Types.h>
#include <Wt/WGlobal.h>
#include <Wt/Dbo/Dbo.h>
#include <Wt/Auth/Dbo/AuthInfo.h>

class User;
#include "Score.h"

using AuthInfo = Wt::Auth::Dbo::AuthInfo<User>;


class User {
public:
	
	template<class Action>
	void persist( Action& a ) {
		Wt::Dbo::hasMany( a, m_scores, Wt::Dbo::ManyToOne, "player" );
		Wt::Dbo::hasMany( a, m_authInfos, Wt::Dbo::ManyToOne, "user" );
	}
private:
	Wt::Dbo::collection< Wt::Dbo::ptr<Score> > m_scores;
	Wt::Dbo::collection< Wt::Dbo::ptr<AuthInfo> > m_authInfos;
};
