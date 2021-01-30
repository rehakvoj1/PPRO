#include "WebGamesApp.h"
#include <random>

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
  
    m_gameList->addWidget( std::make_unique<Wt::WAnchor>( Wt::WLink( Wt::LinkType::InternalPath, "/hangman" ) , "hangman" ) );
    m_gameList->addWidget( std::make_unique<Wt::WAnchor>( Wt::WLink( Wt::LinkType::InternalPath, "/bullsAndCows" ) , "bullsAndCows" ) );
    m_gameList->addWidget( std::make_unique<Wt::WAnchor>( Wt::WLink( Wt::LinkType::InternalPath, "/riddles" ) , "riddles" ) );
    m_gameList->addWidget( std::make_unique<Wt::WAnchor>( Wt::WLink( Wt::LinkType::InternalPath, "/guessTheSong" ) , "guessTheSong" ) );
    
    m_mainStack->addWidget( std::unique_ptr<WContainerWidget>( m_gameList ) );
    m_mainStack->hide();
    
    addWidget( std::move( authWidget ) );
    addWidget( std::unique_ptr<Wt::WStackedWidget>( m_mainStack ) );

    Wt::WApplication::instance()->internalPathChanged().connect( this, &WebGamesApp::HandleInternalPath );
    authWidgetPtr->processEnvironment();
}

WebGamesApp::~WebGamesApp() {
}

void WebGamesApp::HandleInternalPath( const std::string& internalPath ) {
    if ( m_session.login().loggedIn() ) {
        auto currentWidget = m_mainStack->currentWidget();
        Wt::WContainerWidget* newWidget = nullptr;
        auto it = m_gamesPtrs.find( currentWidget->id() );

        if ( currentWidget->id() != m_gameList->id() ) {
            auto widgetPtr = m_mainStack->removeWidget( currentWidget );
            widgetPtr.reset();
            m_gamesPtrs.erase( it );
        }
        
        if ( internalPath == "/list" ) {
            ShowPage( m_gameList );
            return;

        } else if ( internalPath == "/bullsAndCows" ) {
            newWidget = new BullsNCowsGame( this, &m_session );

        } else if ( internalPath == "/riddles" ) {
            newWidget = new RiddlesGame( this, &m_session );

        } else if ( internalPath == "/guessTheSong" ) {
            newWidget = new GuessTheSongGame();

        } else if ( internalPath == "/hangman" ) {
            newWidget = new HangmanGame( this, &m_session );

        } else {
            Wt::WApplication::instance()->setInternalPath( "/list", true );
            return;
        }

        m_gamesPtrs.emplace( newWidget->id(), newWidget );
        m_mainStack->addWidget( std::unique_ptr<WContainerWidget>( m_gamesPtrs[newWidget->id()] ) );
        ShowPage( m_gamesPtrs[newWidget->id()] );
    }
}

size_t WebGamesApp::GetRandomInt(size_t max) {
    std::random_device rd;
    std::mt19937 gen( rd() );
    std::uniform_int_distribution<size_t> distrib( 1, max );
    return distrib( gen );
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
