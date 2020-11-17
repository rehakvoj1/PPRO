#pragma once

#include <Wt/Dbo/Dbo.h>
#include <string>

class Riddle {
public:

    template<class Action>
    void persist( Action& a ) {
        dbo::field( a, m_riddle, "riddle" );
        dbo::field( a, m_lang, "language" );
        dbo::field( a, m_answer, "answer" );
    }

private:
    std::string m_riddle;
    std::string m_lang;
    std::string m_answer;
};