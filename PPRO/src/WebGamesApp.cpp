#include "WebGamesApp.h"

#include <Wt/WAnchor.h>
#include <Wt/WText.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WApplication.h>
#include <Wt/Auth/AuthWidget.h>
#include <Wt/Auth/PasswordService.h>
#include <Wt/Auth/RegistrationModel.h>

WebGamesApp::WebGamesApp() {
    m_session.login().changed().connect( this, &WebGamesApp::onAuthEvent );

    std::unique_ptr<Wt::Auth::AuthModel> authModel
        = std::make_unique<Wt::Auth::AuthModel>( Session::Auth(), m_session.GetUsers() );
    authModel->addPasswordAuth( &Session::PasswordAuth() );
    
    std::unique_ptr<Wt::Auth::AuthWidget> authWidget
        = std::make_unique<Wt::Auth::AuthWidget>( m_session.login() );
    auto authWidgetPtr = authWidget.get();
    authWidget->setModel( std::move( authModel ) );
    authWidget->setRegistrationEnabled( true );

    std::unique_ptr<Wt::WText> title( std::make_unique<Wt::WText>( "<h1>A Witty game: Hangman</h1>" ) );
    addWidget( std::move( title ) );

    addWidget( std::move( authWidget ) );
    authWidgetPtr->processEnvironment();
}


void WebGamesApp::onAuthEvent() {
    if ( m_session.login().loggedIn() ) {
        Wt::Dbo::ptr<User> pPotentialNewUser = nullptr;
        Wt::WString loggedUserName;
        
        {
            Wt::Dbo::Transaction transaction{ m_session };
            loggedUserName = m_session.login().user().identity(Wt::Auth::Identity::LoginName);
            pPotentialNewUser = m_session.find<User>().where( "name = ?" ).bind( loggedUserName.toUTF8() );
        }

        if ( !pPotentialNewUser ) {
            Wt::Dbo::Transaction transaction{ m_session };
            std::unique_ptr<User> user{ new User( loggedUserName.toUTF8() ) };
            m_session.add( std::move( user ) );   
        }
    }
}