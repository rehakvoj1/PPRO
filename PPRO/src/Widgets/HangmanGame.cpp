#include "HangmanGame.h"
#include <sstream>

#include <Wt/WText.h>
#include <Wt/WAnchor.h>
#include <Wt/WPushButton.h>

HangmanGame::HangmanGame() {
	std::unique_ptr<Wt::WText> title( std::make_unique<Wt::WText>( "<h1>Hangman</h1>" ) );

	addWidget( std::move( title ) );
	addWidget( std::make_unique<Wt::WAnchor>( Wt::WLink( Wt::LinkType::InternalPath, "/list" ), "list" ) );

	m_hangmanImg = new Wt::WStackedWidget();
	for ( int i = 0; i < 12; ++i ) {
		std::string name = "resources/hangman" + std::to_string( i ) + ".png";
		Wt::WImage* image = m_hangmanImg->addNew<Wt::WImage>( Wt::WLink( name ) );
		image->setAlternateText( "HANGMAN" );
		image->resize( 500, 500 );
		m_images.emplace_back( image );
	}
	addWidget( std::unique_ptr<Wt::WStackedWidget>( m_hangmanImg ) );

	std::istringstream ss( tr( "hangman.letters" ).toUTF8() );
	std::string letter;

	while ( std::getline( ss, letter ) ) {
		addWidget( std::make_unique<Wt::WPushButton>( letter ) );
	}

}
