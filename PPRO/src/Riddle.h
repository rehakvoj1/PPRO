#pragma once

#include <Wt/Dbo/Dbo.h>
#include <string>

class Riddle {
public:

    Riddle( std::string riddle = "", std::string lang = "", std::string answer = "" ) :
        m_riddle( riddle ),
        m_lang( lang ),
        m_answer( answer ) 
    {}


    template<class Action>
    void persist( Action& a ) {
        Wt::Dbo::field( a, m_riddle, "riddle" );
        Wt::Dbo::field( a, m_lang, "language" );
        Wt::Dbo::field( a, m_answer, "answer" );
    }

private:
    std::string m_riddle;
    std::string m_lang;
    std::string m_answer;
};
