#pragma once

#include <Wt/Dbo/Dbo.h>
#include <string>

#include "Game.h"
#include "User.h"
#include "Word.h"

class Score {
public:
  
    template<class Action>
    void persist( Action& a ) {
        Wt::Dbo::field( a, m_guessCnt, "guessCount" );
        Wt::Dbo::belongsTo( a, m_user, "user" );
        Wt::Dbo::belongsTo( a, m_game, "game" );
        Wt::Dbo::belongsTo( a, m_word, "word" );
    }

private:
    int m_guessCnt;
    Wt::Dbo::ptr<Game> m_game;
    Wt::Dbo::ptr<User> m_user;
    Wt::Dbo::ptr<Word> m_word;
};

