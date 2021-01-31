#include "GuessTheSongGame.h"
#include "../Session.h"
#include "../DAO/Song.h"
#include "WebGamesApp.h"

#include <thread>
#include <chrono>

#include <Wt/Dbo/Transaction.h>
#include <Wt/WText.h>
#include <Wt/WAnchor.h>
#include <Wt/WSound.h>
#include <Wt/WPushButton.h>
#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>

GuessTheSongGame::GuessTheSongGame( WebGamesApp* app, Session* session ) :	m_app( app ), 
																			m_session( session ),
																			m_hintPhase( 0 ),
																			m_gameWon( false ),
																			m_gameName( "GuessTheSong" ) {
	// TITLE
	std::unique_ptr<Wt::WText> title( std::make_unique<Wt::WText>( "<h1>Guess the Song</h1>" ) );
	addWidget( std::move( title ) );

	// LINK - game list
	addWidget( std::make_unique<Wt::WAnchor>( Wt::WLink( Wt::LinkType::InternalPath, "/list" ), "list" ) );

	// PARAGRAPH BREAK
	addWidget( std::make_unique<Wt::WBreak>() );

	// NEW GAME BUTTON
	auto btnNewGame = std::make_unique<Wt::WPushButton>( "New Game" );
	btnNewGame->enterPressed().connect( std::bind( &GuessTheSongGame::NewGame, this ) );
	btnNewGame->clicked().connect( std::bind( &GuessTheSongGame::NewGame, this ) );
	addWidget( std::move( btnNewGame ) );
	
	// PARAGRAPH BREAK
	addWidget( std::make_unique<Wt::WBreak>() );

	// PLAY BUTTON
	m_playButton = new Wt::WPushButton( "Play sample" );
	m_playButton->clicked().connect( std::bind( &GuessTheSongGame::PlaySound, this ) );
	addWidget( std::unique_ptr<Wt::WPushButton>( m_playButton ) );

	// STOP BUTTON
	m_stopButton = new Wt::WPushButton( "Stop" );
	m_stopButton->clicked().connect( std::bind( &GuessTheSongGame::StopSound, this ) );
	addWidget( std::unique_ptr<Wt::WPushButton>( m_stopButton ) );

	// SONG CONTAINER + TIMER
	m_songContainer = new Wt::WContainerWidget();
	m_timer = m_songContainer->addChild( std::make_unique<Wt::WTimer>() );
	m_timer->setInterval( std::chrono::seconds( 3 ) );
	m_timer->timeout().connect( std::bind( &GuessTheSongGame::StopSound, this ) );
	addWidget( std::unique_ptr<Wt::WContainerWidget>( m_songContainer ) );

	// PARAGRAPH BREAK
	addWidget( std::make_unique<Wt::WBreak>() );

	// ARTIST TEXT
	m_artist = new Wt::WText( "ARTIST" );
	m_artist->setStyleClass( "artistHidden" );
	addWidget( std::unique_ptr<Wt::WText>( m_artist ) );

	// PARAGRAPH BREAK
	addWidget( std::make_unique<Wt::WBreak>() );

	// SONG TITLE
	m_songWidget = new Wt::WText( "SONG NAME" );
	m_songWidget->setStyleClass( "songHidden" );
	addWidget( std::unique_ptr<Wt::WText>( m_songWidget ) );

	// PARAGRAPH BREAK
	addWidget( std::make_unique<Wt::WBreak>() );

	// USER INPUT
	m_userInput = new Wt::WLineEdit();
	m_userInput->setWidth( 300 );
	m_userInput->enterPressed().connect( std::bind( &GuessTheSongGame::CheckAnswer, this ) );
	m_userInput->clicked().connect( [&] {
		if ( m_userInput->hasStyleClass( "inputWrong" ) ) {
			m_userInput->removeStyleClass( "inputWrong" );
		}
		if ( m_userInput->hasStyleClass( "inputCorrect" ) ) {
			m_userInput->removeStyleClass( "inputCorrect" );
		}
	} );
	addWidget( std::unique_ptr<Wt::WLineEdit>( m_userInput ) );

	// CONFIRM BUTTON
	auto btnOK = std::make_unique<Wt::WPushButton>( "OK" );
	btnOK->clicked().connect( std::bind( &GuessTheSongGame::CheckAnswer, this ) );
	addWidget( std::move( btnOK ) );

	// HINT BUTTON
	m_hintBtn = new Wt::WPushButton( "Show next hint" );
	m_hintBtn->clicked().connect( std::bind( &GuessTheSongGame::HintUpdate, this ) );
	addWidget( std::unique_ptr<Wt::WPushButton>( m_hintBtn ) );


	NewGame();
}

//=========================================================
void GuessTheSongGame::NewGame() {
	m_hintPhase = 0;
	m_timer->stop();
	
	m_userInput->enable();
	m_userInput->removeStyleClass( "inputCorrect" );
	m_userInput->removeStyleClass( "inputWrong" );
	m_userInput->setValueText( "" );
	Wt::WCssDecorationStyle style;
	m_userInput->enable();
	m_userInput->setFocus( true );
	style.setBackgroundColor( { 255, 255, 255 } );
	m_userInput->setDecorationStyle( style );
	
	m_playButton->setText( "Play sample" );
	m_playButton->enable();
	
	m_hintBtn->setText( "Next hint" );
	m_hintBtn->enable();

	m_artist->setStyleClass( "artistHidden" );
	m_songWidget->setStyleClass( "songHidden" );
	m_songContainer->clear();
	m_songName.clear();

	NewRandomSong();
}

//=====================================================================
void GuessTheSongGame::CheckAnswer() {
	std::string userAnswer = m_userInput->valueText().toUTF8();
	for ( char& c : userAnswer ) {
		c = tolower( c );
	}

	if ( userAnswer == m_songName ) {
		m_gameWon = true;
		GameEnd();
		PlaySound();
	} else {
		m_userInput->setStyleClass( "inputWrong" );
	}

}

//=============================================================
void GuessTheSongGame::NewRandomSong() {
	Wt::Dbo::Transaction transaction{ *m_session };
	typedef Wt::Dbo::collection< Wt::Dbo::ptr<Song> > Songs;
	Songs songs = m_session->find<Song>();

	size_t random = m_app->GetRandomInt( songs.size() );
	while ( random > std::numeric_limits<int>::max() ) {
		random = m_app->GetRandomInt( songs.size() );
	}

	std::cout << random << std::endl;
	Wt::Dbo::ptr<Song> song = m_session->find<Song>().where( "id = ?" ).bind( static_cast<int>( random ) );

	m_artist->setText( song->m_artist );
	m_songWidget->setText( song->m_title );
	for ( char c : song->m_title ) {
		m_songName.push_back( tolower( c ) );
	}

	m_songPath = "resources/songs/" + song->m_artist + "_" + song->m_title + ".mp3";
	m_song = m_songContainer->addNew<Wt::WAudio>();
	m_song->addSource( Wt::WLink( m_songPath ) );
	m_song->ended().connect( std::bind( &GuessTheSongGame::StopSound, this ) );
	
	
}

//==========================================================
void GuessTheSongGame::HintUpdate() {
	m_hintPhase++;

	switch ( m_hintPhase ) {
		case 1:
			m_artist->removeStyleClass( "artistHidden" );
			break;
		case 2:
			m_playButton->setText( "Play full song" );
			m_hintBtn->setText( "Reveal song title" );
			break;
		case 3:
			GameEnd();
			break;
		default:
			break;
	}
}

//==========================================================
void GuessTheSongGame::RevealSong() {
	m_songWidget->removeStyleClass( "songHidden" );
	m_artist->removeStyleClass( "artistHidden" );
	m_playButton->setText( "Play full song" );
	m_hintBtn->setText( "Song REVEALED " );
	m_hintBtn->disable();
}

//==========================================================
void GuessTheSongGame::GameEnd() {
	m_timer->stop();

	m_hintPhase = 3;

	RevealSong();

	m_stopButton->setFocus();
	
	Wt::WCssDecorationStyle style;
	m_userInput->disable();
	if ( m_gameWon ) {
		style.setBackgroundColor( { 0, 255, 0 } );
		m_userInput->setDecorationStyle( style );
	} else {
		style.setBackgroundColor( { 255, 0, 0 } );
		m_userInput->setDecorationStyle( style );
	}
}

//================================================
void GuessTheSongGame::PlaySound() {
	if ( m_hintPhase < 2 ) {
		m_timer->start();
	} 

	m_song->play();
	m_playButton->disable();
	m_userInput->disable();
}

//===============================================
void GuessTheSongGame::StopSound() {
	if ( m_hintPhase < 2 ) {
		m_timer->stop();
	} 

	m_song->pause();
	m_song->clearSources();
	m_song->addSource( m_songPath );
	m_playButton->enable();

	if ( m_hintPhase != 3 ) {
		m_userInput->enable();
		m_userInput->setFocus( true );
	}
}


