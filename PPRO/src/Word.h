#pragma once

#include <Wt/Dbo/Dbo.h>
#include <string>

class Score;

class Word {
public:
   
    Word(std::string word, std::string lang) : 
        m_word( word ), 
        m_lang( lang ), 
        m_len( word.length() )
    {}

    template<class Action>
    void persist( Action& a ) {
        Wt::Dbo::field( a, m_word, "word" );
        Wt::Dbo::field( a, m_lang, "language" );
        Wt::Dbo::field( a, m_len, "length" );
        Wt::Dbo::hasMany( a, m_scores, Wt::Dbo::ManyToOne, "word" );
    }

private:
	std::string m_word;
	std::string m_lang;
	int         m_len;
    Wt::Dbo::collection< Wt::Dbo::ptr<Score> > m_scores;

};

