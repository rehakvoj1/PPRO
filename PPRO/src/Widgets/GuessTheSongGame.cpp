#include "GuessTheSongGame.h"

#include <Wt/WText.h>
#include <Wt/WAnchor.h>
#include <Wt/WSound.h>

GuessTheSongGame::GuessTheSongGame() {
	std::unique_ptr<Wt::WText> title( std::make_unique<Wt::WText>( "<h1>Guess the Song</h1>" ) );
	
	addWidget( std::move( title ) );
	addWidget( std::make_unique<Wt::WAnchor>( Wt::WLink( Wt::LinkType::InternalPath, "/list" ), "list" ) );

	
}
