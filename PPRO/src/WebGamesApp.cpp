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
#include <Wt/WLink.h>

WebGamesApp::WebGamesApp() {
    m_session.login().changed().connect( this, &WebGamesApp::OnAuthEvent );

    std::unique_ptr<Wt::Auth::AuthModel> authModel
        = std::make_unique<Wt::Auth::AuthModel>( Session::Auth(), m_session.GetUsers() );
    authModel->addPasswordAuth( &Session::PasswordAuth() );
    
    std::unique_ptr<Wt::Auth::AuthWidget> authWidget
        = std::make_unique<Wt::Auth::AuthWidget>( m_session.login() );
    auto authWidgetPtr = authWidget.get();
    authWidget->setModel( std::move( authModel ) );
    authWidget->setRegistrationEnabled( true );
  
    m_mainStack = new Wt::WStackedWidget();
  
    std::unique_ptr<Wt::WContainerWidget> m_links = std::make_unique<Wt::WContainerWidget>();
    m_links->addWidget( std::make_unique<Wt::WAnchor>( Wt::WLink( Wt::LinkType::InternalPath, "/hangman" ) , "hangman" ) );
    m_links->addWidget( std::make_unique<Wt::WAnchor>( Wt::WLink( Wt::LinkType::InternalPath, "/bullsAndCows" ) , "bullsAndCows" ) );
    m_links->addWidget( std::make_unique<Wt::WAnchor>( Wt::WLink( Wt::LinkType::InternalPath, "/riddles" ) , "riddles" ) );
    m_links->addWidget( std::make_unique<Wt::WAnchor>( Wt::WLink( Wt::LinkType::InternalPath, "/guessTheSong" ) , "guessTheSong" ) );
    Wt::WContainerWidget* widget = m_mainStack->addWidget( std::move( m_links ) );
    
    m_mainStack->hide();

    addWidget( std::move( authWidget ) );
    addWidget( std::unique_ptr<Wt::WStackedWidget>( m_mainStack ) );

    Wt::WApplication::instance()->internalPathChanged().connect( this, &WebGamesApp::HandleInternalPath );
    authWidgetPtr->processEnvironment();
}

void WebGamesApp::HandleInternalPath( const std::string& internalPath ) {
    if ( m_session.login().loggedIn() ) {
        if ( internalPath == "/list" ) {
            ShowGameList();
        } else if ( internalPath == "/bullsAndCows" ) {

        } else if ( internalPath == "/riddles" ) {

        } else if ( internalPath == "/guessTheSong" ) {

        } else if ( internalPath == "/hangman" ) {

        } else {
            Wt::WApplication::instance()->setInternalPath( "/list", true );
        }
    }
}


void WebGamesApp::OnAuthEvent() {
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

        HandleInternalPath( Wt::WApplication::instance()->internalPath() );
    }
}

void WebGamesApp::ShowGameList() {
    m_mainStack->show();
}
