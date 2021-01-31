#pragma once
#include <Wt/Dbo/Dbo.h>
#include <string>

class Song {
public:
    Song( std::string artist = "", std::string title = "") :
        m_artist( artist ), 
        m_title( title ) {
    }


    template<class Action>
    void persist( Action& a ) {
        Wt::Dbo::field( a, m_title, "title" );
        Wt::Dbo::field( a, m_artist, "artist" );
    }

    std::string m_title;
    std::string m_artist;
};

