#pragma once
#include <Wt/Dbo/Types.h>
#include <Wt/WGlobal.h>
#include <Wt/Dbo/Dbo.h>

class User;
#include "Score.h"


class User {
public:
	
	template<class Action>
	void persist( Action& a ) {
		Wt::Dbo::hasMany( a, m_scores, Wt::Dbo::ManyToOne, "user" );
	}
private:
	Wt::Dbo::collection< Wt::Dbo::ptr<Score> > m_scores;
};
