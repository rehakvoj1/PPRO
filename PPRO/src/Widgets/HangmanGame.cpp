#include "HangmanGame.h"
#include <sstream>
#include <chrono>
#include <random>

#include <Wt/WText.h>
#include <Wt/WAnchor.h>
#include <Wt/WPushButton.h>
#include <Wt/WApplication.h>
#include "WebGamesApp.h"


HangmanGame::HangmanGame(WebGamesApp* app, Session* session) : m_app(app), m_session(session), m_imageIdx(0) {
	std::unique_ptr<Wt::WText> title( std::make_unique<Wt::WText>( "<h1>Hangman</h1>" ) );

	addWidget( std::move( title ) );
	addWidget( std::make_unique<Wt::WAnchor>( Wt::WLink( Wt::LinkType::InternalPath, "/list" ), "list" ) );
	
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

	auto btnNewGamePtr = std::make_unique<Wt::WPushButton>( "New Game" );
	auto btnNewGame = btnNewGamePtr.get();
	btnNewGame->clicked().connect( std::bind(&HangmanGame::NewGame,this) );
	addWidget( std::move( btnNewGamePtr ) );
	
	m_hiddenWordContainer = new Wt::WContainerWidget();
	m_lettersContainer = new Wt::WContainerWidget();
	
	LoadImages();
	addWidget( std::unique_ptr<Wt::WContainerWidget>( m_hiddenWordContainer ) );
	addWidget( std::make_unique<Wt::WBreak>() );
	addWidget( std::unique_ptr<Wt::WContainerWidget>( m_lettersContainer ) );
	LoadLetters();

	NewGame();
}

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
		m_imageIdx = 12;
		for ( auto& it : m_lettersContainer->children() ) {
			it->disable();
		}
	}

	UpdateHiddenWord();
	m_hangmanImg->setCurrentIndex( m_imageIdx );
}

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

bool HangmanGame::IsGameWon() {
	for ( auto& pair : m_hiddenWord ) {
		if ( !pair.second ) {
			return pair.second;
		}
	}
	return true;
}

bool HangmanGame::IsGameLost() {
	return m_imageIdx == 11;
}

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

void HangmanGame::NewRandomWord() {
	Wt::Dbo::Transaction transaction{ *m_session };
	typedef Wt::Dbo::collection< Wt::Dbo::ptr<Word> > Words;
	Words words = m_session->find<Word>();

	int random = m_app->GetRandomInt( words.size() );
	Wt::Dbo::ptr<Word> word = m_session->find<Word>().where( "id = ?" ).bind( random );

	for ( char c : word->m_word ) {
		m_hiddenWord.emplace_back( std::pair<char, bool>( c, false ) );
	}
}

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

void HangmanGame::LoadLetters() {
	std::istringstream ss( tr( "hangman.letters" ).toUTF8() );
	char letter;

	for ( int i = 0; i < 26; ++i ) {
		std::string btnText(1, 'A'+i);
		auto button = m_lettersContainer->addWidget( std::make_unique<Wt::WPushButton>( btnText ) );
		button->clicked().connect( std::bind( &HangmanGame::ResolveLetter, this, button ) );

	}
}


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