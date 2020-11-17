#pragma once

#include <Wt/Dbo/Dbo.h>
#include <string>

class Word {
public:

    template<class Action>
    void persist( Action& a ) {
        dbo::field( a, m_word, "word" );
        dbo::field( a, m_lang, "language" );
        dbo::field( a, m_len, "length" );
    }

private:
	std::string m_word;
	std::string m_lang;
	int         m_len;
};

