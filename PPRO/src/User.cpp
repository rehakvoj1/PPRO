#include "User.h"

User::User() : m_name("")
{
}

User::User( std::string name ) : m_name( name )
{
}

std::string User::getName() {
    return m_name;
}
