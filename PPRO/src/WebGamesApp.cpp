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
        } else {
    }
}