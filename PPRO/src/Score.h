#pragma once

#include <Wt/Dbo/Dbo.h>
#include <string>

class Score;
#include "Game.h"
#include "User.h"
#include "Word.h"


class Score {
public:

    Score( std::unique_ptr<Game> game = nullptr, std::unique_ptr<User> user = nullptr, std::unique_ptr<Word> word = nullptr ) : 
        m_guessCnt( 0 ),
        m_game( std::move(game) ),
        m_user( std::move(user) ),
        m_word( std::move(word) )
    { }

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


