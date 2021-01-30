#include "RiddlesGame.h"

#include <Wt/WText.h>
#include <Wt/WAnchor.h>
#include <Wt/Dbo/Transaction.h>
#include "../Session.h"
#include "WebGamesApp.h"
#include "../DAO/Riddle.h"
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WBreak.h>

RiddlesGame::RiddlesGame( WebGamesApp* app, Session* session ) : m_app( app ), m_session( session ) {
	std::unique_ptr<Wt::WText> title( std::make_unique<Wt::WText>( "<h1>Riddles</h1>" ) );
	
	addWidget( std::move( title ) );
	addWidget( std::make_unique<Wt::WAnchor>( Wt::WLink( Wt::LinkType::InternalPath, "/list" ), "list" ) );
	
	auto btnNewGame = std::make_unique<Wt::WPushButton>( "New Game" );
	btnNewGame->clicked().connect( std::bind( &RiddlesGame::NewGame, this ) );
	addWidget( std::move( btnNewGame ) );

	m_riddleContainer = new Wt::WContainerWidget();
	addWidget( std::unique_ptr<Wt::WContainerWidget>( m_riddleContainer ) );

	m_riddleContainer->addWidget( std::make_unique<Wt::WText>("This is the riddle") );
	m_riddleContainer->addWidget( std::make_unique<Wt::WBreak>() );
	m_riddleContainer->addWidget( std::make_unique<Wt::WText>("This is the answer") );
	m_riddleContainer->widget( 2 )->setStyleClass("answer");
	addWidget( std::make_unique<Wt::WBreak>() );
	
	m_userInput = new Wt::WLineEdit();
	m_userInput->clicked().connect( [&] {
		if ( m_userInput->hasStyleClass( "inputWrong" ) ) {
			m_userInput->removeStyleClass( "inputWrong" );
		}
		if ( m_userInput->hasStyleClass( "inputCorrect" ) ) {
			m_userInput->removeStyleClass( "inputCorrect" );
		}
	} );
	addWidget( std::unique_ptr<Wt::WLineEdit>( m_userInput ) );

	auto btnOK = std::make_unique<Wt::WPushButton>("OK");
	btnOK->clicked().connect( std::bind( &RiddlesGame::CheckAnswer, this ) );
	addWidget( std::move( btnOK ) );
	this->enterPressed().connect( std::bind( &RiddlesGame::CheckAnswer, this ) );

	auto btnSolution = std::make_unique<Wt::WPushButton>("Show Solution");
	btnSolution->clicked().connect( std::bind( &RiddlesGame::ToggleSolution, this ) );
	addWidget( std::move( btnSolution ) );


	NewGame();
}

void RiddlesGame::NewGame() {
	if ( !m_riddleContainer->widget( 2 )->hasStyleClass( "answer" ) ) {
		ToggleSolution();
	}
	m_userInput->removeStyleClass( "inputCorrect" );
	m_userInput->removeStyleClass( "inputWrong" );
	m_userInput->setText("");
	NewRandomRiddle();

	static_cast<Wt::WText*>( m_riddleContainer->widget( 0 ) )->setText( m_riddle );
	static_cast<Wt::WText*>( m_riddleContainer->widget( 2 ) )->setText( m_answer );
}

void RiddlesGame::NewRandomRiddle() {
	Wt::Dbo::Transaction transaction{ *m_session };
	typedef Wt::Dbo::collection< Wt::Dbo::ptr<Riddle> > Riddles;
	Riddles riddles = m_session->find<Riddle>();

	int random = m_app->GetRandomInt( riddles.size() );
	Wt::Dbo::ptr<Riddle> riddle = m_session->find<Riddle>().where( "id = ?" ).bind( random );
	
	m_riddle = riddle->m_riddle;
	m_answer = riddle->m_answer;
}

void RiddlesGame::CheckAnswer() {
	std::string userAnswer = m_userInput->valueText().toUTF8();

	if ( userAnswer == m_answer ) {
		m_userInput->setStyleClass( "inputCorrect" );
		ToggleSolution();
	} else {
		m_userInput->setStyleClass( "inputWrong" );
	}
}

void RiddlesGame::ToggleSolution() {
	if ( m_riddleContainer->widget( 2 )->hasStyleClass("answer") ) {
		m_riddleContainer->widget( 2 )->removeStyleClass("answer");
	} else {
		m_riddleContainer->widget( 2 )->setStyleClass("answer");
	}
	
}
