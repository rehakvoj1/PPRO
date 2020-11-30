#pragma once

#include <Wt/Dbo/Dbo.h>
#include <string>

class Score;

class Game {
public:
    
    template<class Action>
    void persist( Action& a ) {
        Wt::Dbo::field( a, m_name, "name" );
        Wt::Dbo::field( a, m_desc, "description" );
        Wt::Dbo::hasMany( a, m_scores, Wt::Dbo::ManyToOne, "game" );
    }

private:
    std::string m_name;
    std::string m_desc;
    Wt::Dbo::collection< Wt::Dbo::ptr<Score> > m_scores;
};

