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

#include "BullsNCowsGame.h"
#include "HangmanGame.h"
#include "GuessTheSongGame.h"
#include "RiddlesGame.h"

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
    m_gameList = new Wt::WContainerWidget();
    m_bullCows = new BullsNCowsGame();
    m_hangman = new HangmanGame();
    m_riddles = new RiddlesGame();
    m_guessSong = new GuessTheSongGame();
  
    m_gameList->addWidget( std::make_unique<Wt::WAnchor>( Wt::WLink( Wt::LinkType::InternalPath, "/hangman" ) , "hangman" ) );
    m_gameList->addWidget( std::make_unique<Wt::WAnchor>( Wt::WLink( Wt::LinkType::InternalPath, "/bullsAndCows" ) , "bullsAndCows" ) );
    m_gameList->addWidget( std::make_unique<Wt::WAnchor>( Wt::WLink( Wt::LinkType::InternalPath, "/riddles" ) , "riddles" ) );
    m_gameList->addWidget( std::make_unique<Wt::WAnchor>( Wt::WLink( Wt::LinkType::InternalPath, "/guessTheSong" ) , "guessTheSong" ) );
    
    m_mainStack->addWidget( std::unique_ptr<WContainerWidget>( m_gameList ) );
    m_mainStack->addWidget( std::unique_ptr<WContainerWidget>( m_bullCows ) );
    m_mainStack->addWidget( std::unique_ptr<WContainerWidget>( m_riddles ) );
    m_mainStack->addWidget( std::unique_ptr<WContainerWidget>( m_guessSong ) );
    m_mainStack->addWidget( std::unique_ptr<WContainerWidget>( m_hangman ) );

    m_mainStack->hide();

    addWidget( std::move( authWidget ) );
    addWidget( std::unique_ptr<Wt::WStackedWidget>( m_mainStack ) );

    Wt::WApplication::instance()->internalPathChanged().connect( this, &WebGamesApp::HandleInternalPath );
    authWidgetPtr->processEnvironment();
}

WebGamesApp::~WebGamesApp() {
    delete m_mainStack;
    delete m_gameList;
    delete m_guessSong;
    delete m_hangman;
    delete m_riddles;
    delete m_bullCows;

}

void WebGamesApp::HandleInternalPath( const std::string& internalPath ) {
    if ( m_session.login().loggedIn() ) {
        if ( internalPath == "/list" ) {
            ShowPage( m_gameList );
        } else if ( internalPath == "/bullsAndCows" ) {
            ShowPage( m_bullCows );
        } else if ( internalPath == "/riddles" ) {
            ShowPage( m_riddles );
        } else if ( internalPath == "/guessTheSong" ) {
            ShowPage( m_guessSong );
        } else if ( internalPath == "/hangman" ) {
            ShowPage( m_hangman );
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

void WebGamesApp::ShowPage(Wt::WContainerWidget* app) {
    m_mainStack->setCurrentWidget(app);
    m_mainStack->show();
}
