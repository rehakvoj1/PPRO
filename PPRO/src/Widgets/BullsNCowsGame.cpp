#include "BullsNCowsGame.h"

#include <Wt/WText.h>
#include <Wt/WAnchor.h>


BullsNCowsGame::BullsNCowsGame() {
	std::unique_ptr<Wt::WText> title( std::make_unique<Wt::WText>( "<h1>Bulls &amp; Cows</h1>" ) );
	
	addWidget( std::move( title ) );
	addWidget( std::make_unique<Wt::WAnchor>( Wt::WLink( Wt::LinkType::InternalPath, "/list" ), "list" ) );
}
