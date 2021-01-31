#include "HangmanGame.h"
#include <sstream>
#include <chrono>
#include <random>

#include "../DAO/User.h"
#include "../DAO/Word.h"
#include "../DAO/DAO.h"
#include <Wt/WText.h>
#include <Wt/WAnchor.h>
#include <Wt/WPushButton.h>
#include <Wt/WBreak.h>
#include <Wt/WDialog.h>
#include <Wt/WApplication.h>
#include "WebGamesApp.h"
#include <Wt/Dbo/Transaction.h>


HangmanGame::HangmanGame(WebGamesApp* app, Session* session) : m_app(app), m_session(session), m_imageIdx(0), m_gameName( "Hangman" ){
	std::unique_ptr<Wt::WText> title( std::make_unique<Wt::WText>( "<h1>Hangman</h1>" ) );

	// TITLE
	addWidget( std::move( title ) );

	// GAME LIST LINK
	addWidget( std::make_unique<Wt::WAnchor>( Wt::WLink( Wt::LinkType::InternalPath, "/list" ), "list" ) );
	
	// PARAGRAPH BREAK
	addWidget( std::make_unique<Wt::WBreak>() );
	
//	auto btnCZPtr = std::make_unique<Wt::WPushButton>( "CZ" );
//	auto btnCZ = btnCZPtr.get();
//	btnCZ->clicked().connect( [=] {
//			Wt::WApplication::instance()->setLocale( "cs" );
//			m_app->HandleInternalPath( Wt::WApplication::instance()->internalPath() );
//		} );
//	addWidget( std::move(btnCZPtr) );
//
//
//	auto btnENPtr = std::make_unique<Wt::WPushButton>( "EN" );
//	auto btnEN = btnENPtr.get();
//	btnEN->clicked().connect( [=] {
//			Wt::WApplication::instance()->setLocale( "en" );
//			m_app->HandleInternalPath( Wt::WApplication::instance()->internalPath() );
//		} );
//	addWidget( std::move( btnENPtr ) );

	// NEW GAME BUTTON
	auto btnNewGamePtr = std::make_unique<Wt::WPushButton>( "New Game" );
	auto btnNewGame = btnNewGamePtr.get();
	btnNewGame->clicked().connect( std::bind(&HangmanGame::NewGame,this) );
	addWidget( std::move( btnNewGamePtr ) );
	
	// HIGH SCORES BUTTON
	auto btnHighScorePtr = std::make_unique<Wt::WPushButton>( "High Scores" );
	auto btnHighScore = btnHighScorePtr.get();
	btnHighScore->clicked().connect( std::bind( &HangmanGame::ShowHighScores, this ) );
	addWidget( std::move( btnHighScorePtr ) );

	m_hiddenWordContainer = new Wt::WContainerWidget();
	m_lettersContainer = new Wt::WContainerWidget();
	
	LoadImages();
	addWidget( std::unique_ptr<Wt::WContainerWidget>( m_hiddenWordContainer ) );
	addWidget( std::make_unique<Wt::WBreak>() );
	addWidget( std::unique_ptr<Wt::WContainerWidget>( m_lettersContainer ) );
	LoadLetters();

	NewGame();
}

//=====================================================================
void HangmanGame::ResolveLetter(Wt::WPushButton* btn) {
	btn->disable();
	
	bool letterHit = LetterHit(btn);
	bool gameWon = false;

	if ( !letterHit ) {
		m_imageIdx++;
	} else {
		gameWon = IsGameWon();
	}

	if ( IsGameLost() ) {
		for ( auto it = m_hiddenWord.begin(); it != m_hiddenWord.end(); ++it ) {
			it->second = true;
		}
		for ( auto& it : m_lettersContainer->children() ) {
			it->disable();
		}
	}

	if ( gameWon ) {
		SaveScore();
		m_imageIdx = 12;
		for ( auto& it : m_lettersContainer->children() ) {
			it->disable();
		}
	}

	UpdateHiddenWord();
	m_hangmanImg->setCurrentIndex( m_imageIdx );
}

//===============================================================================
bool HangmanGame::LetterHit( Wt::WPushButton* btn ) {
	bool hit = false;
	char letter = std::tolower( btn->text().toUTF8()[0] );
	for ( auto it = m_hiddenWord.begin(); it != m_hiddenWord.end(); ++it ) {
		if ( letter == it->first ) {
			it->second = true;
			hit = true;
		}
	}
	return hit;
}

//============================================================
bool HangmanGame::IsGameWon() {
	for ( auto& pair : m_hiddenWord ) {
		if ( !pair.second ) {
			return pair.second;
		}
	}
	return true;
}

//============================================================
bool HangmanGame::IsGameLost() {
	return m_imageIdx == 11;
}

//============================================================
void HangmanGame::UpdateHiddenWord() {
	std::string wordPlaceholder;
	for ( auto& pair : m_hiddenWord ) {
		if ( pair.second ) {
			wordPlaceholder.push_back( pair.first );
		} else {
			wordPlaceholder.push_back( '-' );
		}
		wordPlaceholder.push_back( ' ' );
	}
	Wt::WText* hiddenWord = dynamic_cast<Wt::WText*>( m_hiddenWordContainer->widget( 0 ) );
	hiddenWord->setText( wordPlaceholder );
}

//============================================================
void HangmanGame::NewRandomWord() {
	DAO dao( m_session );
	typedef Wt::Dbo::collection< Wt::Dbo::ptr<Word> > Words;
	Words words = dao.ReadAll<Word>();

	int random = m_app->GetRandomInt( words.size() );
	m_wordPtr = dao.ReadOne<Word>( static_cast<Wt::Dbo::dbo_default_traits::IdType>( random ) );
	for ( char c : m_wordPtr->m_word ) {
		m_hiddenWord.emplace_back( std::pair<char, bool>( c, false ) );
	}
}

//=============================================================
void HangmanGame::NewGame() {
	m_imageIdx = 0;
	m_hangmanImg->setCurrentIndex( m_imageIdx );
	for ( auto& it : m_lettersContainer->children() ) {
		it->enable();
	}
	m_hiddenWord.clear();
	m_hiddenWordContainer->clear();

	NewRandomWord();
	
	std::string wordPlaceholder;
	for ( auto it = m_hiddenWord.begin(); it != m_hiddenWord.end(); ++it ) {
		wordPlaceholder.push_back( '-' );
		wordPlaceholder.push_back( ' ' );
	}
	auto text = m_hiddenWordContainer->addWidget( std::make_unique<Wt::WText>( wordPlaceholder ) );
	text->addStyleClass( "hiddenWord" );
}

//=======================================================================
void HangmanGame::LoadLetters() {
	std::istringstream ss( tr( "hangman.letters" ).toUTF8() );
	char letter;

	for ( int i = 0; i < 26; ++i ) {
		std::string btnText(1, 'A'+i);
		auto button = m_lettersContainer->addWidget( std::make_unique<Wt::WPushButton>( btnText ) );
		button->clicked().connect( std::bind( &HangmanGame::ResolveLetter, this, button ) );

	}
}

//================================================================================
void HangmanGame::LoadImages() {
	m_hangmanImg = new Wt::WStackedWidget();
	for ( int i = 0; i < 13; ++i ) {
		std::string name = "resources/hangman" + std::to_string( i ) + ".png";
		Wt::WImage* image = m_hangmanImg->addNew<Wt::WImage>( Wt::WLink( name ) );
		image->setAlternateText( "HANGMAN" );
		image->resize( 500, 500 );
		m_images.emplace_back( image );
	}
	addWidget( std::unique_ptr<Wt::WStackedWidget>( m_hangmanImg ) );
}

//=============================================================================
void HangmanGame::SaveScore() {
	DAO dao( m_session );
	Wt::Dbo::ptr<Game> game = dao.FindOneByCondition<Game, std::string>( "name = ?", m_gameName );
	Wt::Dbo::ptr<User> user = dao.FindOneByCondition<User, std::string>( "name = ?", m_app->GetLoggedUserName() );

	std::unique_ptr<Score> score{ new Score() };
	score->m_guessCnt = m_imageIdx;
	score->m_game = game;
	score->m_user = user;
	score->m_word = m_wordPtr;
	dao.Create<Score>( std::move( score ) );

}

//==============================================================================
void HangmanGame::ShowHighScores() {
	DAO dao( m_session );
	Wt::Dbo::Transaction transaction { *m_session };
	auto game = dao.FindOneByCondition<Game, std::string>( "name = ?", m_gameName );
	auto scores = dao.FindCollectionByCondition<Score,int>( "game_id = ?", static_cast<int>(game.id()) );

	auto dialog = this->addChild( std::make_unique<Wt::WDialog>( "HighScores" ) );
	
	for ( auto& score : scores ) {
		auto user = dao.ReadOne<User>( score->m_user.id() );
		auto word = dao.ReadOne<Word>( score->m_word.id() );
		dialog->contents()->addNew<Wt::WText>( user->m_name + ": " + word->m_word + " | " + std::to_string( score->m_guessCnt ) );
		dialog->contents()->addNew<Wt::WBreak>();
	}

	Wt::WPushButton* ok =
		dialog->footer()->addNew<Wt::WPushButton>( "OK" );
	ok->setDefault( true );

	ok->clicked().connect( [=] {
			dialog->accept();
	} );

	dialog->finished().connect( [=] {
		this->removeChild( dialog );
	} );

	dialog->show();
}
