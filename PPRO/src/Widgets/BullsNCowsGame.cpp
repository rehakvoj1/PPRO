#include "BullsNCowsGame.h"

#include <Wt/WText.h>
#include <Wt/WAnchor.h>
#include <Wt/WPushButton.h>
#include <Wt/WComboBox.h>
#include <Wt/WBreak.h>
#include <Wt/WLineEdit.h>
#include <Wt/Dbo/Transaction.h>
#include <Wt/WCssDecorationStyle.h>

#include "../DAO/DAO.h"
#include "../DAO/Word.h"
#include "../Session.h"
#include "WebGamesApp.h"
#include "../DAO/Score.h"
#include "../DAO/User.h"

BullsNCowsGame::BullsNCowsGame( WebGamesApp* app, Session* session ) :	m_isogramLen( 3 ), 
																		m_bulls( 0 ), 
																		m_cows( 0 ),
																		m_score( 0 ),
																		m_app( app ), 
																		m_session( session ),
																		m_gameName( "BullsNCows" ) {
	// TITLE
	std::unique_ptr<Wt::WText> title( std::make_unique<Wt::WText>( "<h1>Bulls &amp; Cows</h1>" ) );
	addWidget( std::move( title ) );

	// LINK TO GAME LIST
	addWidget( std::make_unique<Wt::WAnchor>( Wt::WLink( Wt::LinkType::InternalPath, "/list" ), "list" ) );
	
	// PARAGRAPH BREAK
	addWidget( std::make_unique<Wt::WBreak>() );

	// NUM OF LETTERS TEXT
	addWidget( std::make_unique<Wt::WText>( "Number of letters: " ) );

	// NUM OF LETTERS COMBOBOX
	auto comboBox = new Wt::WComboBox();
	comboBox->addItem( "3" );
	comboBox->addItem( "4" );
	comboBox->addItem( "5" );
	comboBox->addItem( "6" );
	comboBox->changed().connect( [=] { m_isogramLen = std::stoi(comboBox->valueText().toUTF8() ); } );
	addWidget( std::unique_ptr<Wt::WComboBox>( comboBox ) );

	// NEW GAME BUTTON
	auto btnNewGame = std::make_unique<Wt::WPushButton>( "New Game" );
	btnNewGame->clicked().connect( std::bind( &BullsNCowsGame::NewGame, this ) );
	addWidget( std::move( btnNewGame ) );

	// PARAGRAPH BREAK
	addWidget( std::make_unique<Wt::WBreak>() );

	// BULLS N COWS TEXT
	m_bullsCows = new Wt::WText("0 Bull & 0 Cows");
	m_bullsCows->addStyleClass( "bullsNCows" );
	addWidget( std::unique_ptr<Wt::WText>( m_bullsCows ) );
	
	// PARAGRAPH BREAK
	addWidget( std::make_unique<Wt::WBreak>() );

	// USER INPUT
	m_userInput = new Wt::WLineEdit();
	m_userInput->enterPressed().connect( std::bind( &BullsNCowsGame::CheckAnswer, this ) );
	addWidget( std::unique_ptr<Wt::WLineEdit>( m_userInput ) );

	// CONFIRM BUTTON
	auto btnOK = std::make_unique<Wt::WPushButton>( "OK" );
	btnOK->clicked().connect( std::bind( &BullsNCowsGame::CheckAnswer, this ) );
	addWidget( std::move( btnOK ) );


	NewGame();
}

//==================================================
void BullsNCowsGame::NewGame() {
	m_bullsCows->setText( "0 Bulls & 0 Cows" );
	m_userInput->setText( "" );
	m_userInput->enable();
	m_userInput->setFocus();
	Wt::WCssDecorationStyle style;
	style.setBackgroundColor( { 255, 255, 255 } );
	m_userInput->setDecorationStyle( style );
	m_cows = 0;
	m_bulls = 0;
	
	NewRandomWord();
}

//======================================================
bool BullsNCowsGame::IsIsogram( std::string word ) {
	for ( int i = 0; i < word.length(); ++i ) {
		for ( int j = i; j < word.length(); ++j ) {
			if ( word[i] == word[j] && i != j ) {
				return false;
			}
		}
	}
	return true;
}

//=======================================================
void BullsNCowsGame::CheckAnswer() {
	// correct letters (cows)
	// correct positions (bulls)

	m_score++;

	std::string userGuess = m_userInput->valueText().toUTF8();
	m_cows = 0;
	m_bulls = 0;

	if ( userGuess.length() != m_hiddenWord.length() ) {
		return;
	}

	for ( int i = 0; i < userGuess.length(); ++i ) {
		for ( int j = 0; j < userGuess.length(); ++j ) {
			if ( userGuess[i] == m_hiddenWord[j] ) {
				if ( i == j ) {
					m_bulls++;
				} else {
					m_cows++;
				}
			}
		}
	}

	if ( m_bulls == m_isogramLen ) {
		m_userInput->disable();
		Wt::WCssDecorationStyle style;
		style.setBackgroundColor( { 0, 255, 0 } );
		m_userInput->setDecorationStyle( style );
		SaveScore();
	}

	std::string result = std::to_string( m_bulls ) + " Bulls & " + std::to_string( m_cows ) + " Cows";
	m_bullsCows->setText( result );
}

//=======================================================================================
void BullsNCowsGame::NewRandomWord() {
	DAO dao( m_session );
	typedef Wt::Dbo::collection< Wt::Dbo::ptr<Word> > Words;
	Words words = dao.FindCollectionByCondition<Word, int>( "length = ?", m_isogramLen );
	
	Wt::Dbo::Transaction transaction{ *m_session };
	std::vector< Wt::Dbo::ptr<Word> > wordsOfLen;
	for ( auto it = words.begin(); it != words.end(); ++it ) {
		wordsOfLen.emplace_back( *it );
	}

	
	int random = 0;
	do {
		random = static_cast<int>( m_app->GetRandomInt( words.size() ) );
		m_wordPtr = wordsOfLen[random];
	} while ( !IsIsogram( m_wordPtr->m_word ) );

	m_hiddenWord = m_wordPtr->m_word;
}

//===============================================================================
void BullsNCowsGame::SaveScore() {
	DAO dao( m_session );
	Wt::Dbo::ptr<Game> game = dao.FindOneByCondition<Game, std::string>( "name = ?", m_gameName );
	Wt::Dbo::ptr<User> user = dao.FindOneByCondition<User, std::string>( "name = ?", m_app->GetLoggedUserName() );

	std::unique_ptr<Score> score{ new Score() };
	score->m_guessCnt = m_score;
	score->m_game = game;
	score->m_user = user;
	score->m_word = m_wordPtr;
	dao.Create<Score>(std::move(score));

}
