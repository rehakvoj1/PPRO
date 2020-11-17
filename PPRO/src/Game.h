#pragma once

#include <Wt/Dbo/Dbo.h>
#include <string>

class Game {
public:

    template<class Action>
    void persist( Action& a ) {
        dbo::field( a, m_name, "name" );
        dbo::field( a, m_desc, "description" );
    }

private:
    std::string m_name;
    std::string m_desc;
};

