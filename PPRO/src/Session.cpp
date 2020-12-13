#include "Session.h"
#include "Riddle.h"

#include "Wt/Auth/AuthService.h"
#include "Wt/Auth/HashFunction.h"
#include "Wt/Auth/PasswordService.h"
#include "Wt/Auth/PasswordStrengthValidator.h"
#include "Wt/Auth/PasswordVerifier.h"
#include "Wt/Auth/GoogleService.h"
#include "Wt/Auth/FacebookService.h"
#include "Wt/Auth/Dbo/AuthInfo.h"

#include "Wt/Dbo/backend/Sqlite3.h"


Wt::Auth::AuthService authService;
Wt::Auth::PasswordService passAuthService(authService);


void Session::ConfigureAuth() {
    std::unique_ptr<Wt::Auth::PasswordVerifier> verifier = std::make_unique<Wt::Auth::PasswordVerifier>();
    verifier->addHashFunction( std::make_unique<Wt::Auth::BCryptHashFunction>( 7 ) );
    passAuthService.setVerifier( std::move( verifier ) );
    passAuthService.setAttemptThrottlingEnabled( true );
    passAuthService.setStrengthValidator( std::make_unique<Wt::Auth::PasswordStrengthValidator>() );
}

Session::Session() {
    auto connection = std::make_unique<Wt::Dbo::backend::Sqlite3>( "ppro.db" );
    connection->setProperty( "show-queries", "true" );
    setConnection( std::move( connection ) );

    mapClass<Game>( "game" );
    mapClass<Riddle>( "riddle" );
    mapClass<Score>( "score" );
    mapClass<User>( "user" );
    mapClass<Word>( "word" );
    mapClass<AuthInfo>( "auth_info" );
    mapClass<AuthInfo::AuthIdentityType>( "auth_identity" );
    mapClass<AuthInfo::AuthTokenType>( "auth_token" );

    try {
        createTables();
        std::cerr << "Created database." << std::endl;
    }
    catch ( std::exception& e ) {
        std::cerr << e.what() << std::endl;
        std::cerr << "Using existing database";
    }

    m_users = std::make_unique<UserDatabase>( *this );
}

Wt::Auth::AbstractUserDatabase& Session::GetUsers() {
    return *m_users;
}

Wt::Dbo::ptr<User> Session::GetUser() const {
    if ( m_login.loggedIn() ) {
        Wt::Dbo::ptr<AuthInfo> authInfo = m_users->find( m_login.user() );
        return authInfo->user();
    } else
        return Wt::Dbo::ptr<User>();
}

const Wt::Auth::AuthService& Session::Auth() {
    return authService;
}

const Wt::Auth::PasswordService& Session::PasswordAuth() {
    return passAuthService;
}