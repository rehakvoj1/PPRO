#include "HangmanGame.h"
#include <sstream>

#include <Wt/WText.h>
#include <Wt/WAnchor.h>
#include <Wt/WPushButton.h>

HangmanGame::HangmanGame() {
	std::unique_ptr<Wt::WText> title( std::make_unique<Wt::WText>( "<h1>Hangman</h1>" ) );

	addWidget( std::move( title ) );
	addWidget( std::make_unique<Wt::WAnchor>( Wt::WLink( Wt::LinkType::InternalPath, "/list" ), "list" ) );

	std::istringstream ss( tr( "hangman.letters" ).toUTF8() );
	std::string letter;

	while ( std::getline( ss, letter ) ) {
		addWidget( std::make_unique<Wt::WPushButton>( letter ) );
	}

}
