#include <Wt/WApplication.h>
#include <Wt/WServer.h>
#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Sqlite3.h>


#include "Session.h"

#include "DAO/Game.h"
#include "DAO/Riddle.h"
#include "DAO/Score.h"
#include "DAO/User.h"
#include "DAO/Word.h"
#include "Widgets/WebGamesApp.h"



void CreateDB() {
	std::unique_ptr<Wt::Dbo::backend::Sqlite3> sqlite3 = std::make_unique<Wt::Dbo::backend::Sqlite3>( "ppro.db" );
	sqlite3->setProperty( "show-queries", "true" );
	Wt::Dbo::Session session;
	session.setConnection( std::move( sqlite3 ) );

	session.mapClass<Game>( "game" );
	session.mapClass<Riddle>( "riddle" );
	session.mapClass<Score>( "score" );
	session.mapClass<User>( "user" );
	session.mapClass<Word>( "word" );

	session.createTables();
}

void FillDB() {

}

std::unique_ptr<Wt::WApplication> createApplication( const Wt::WEnvironment& env ) {
	auto app = std::make_unique<Wt::WApplication>( env );
	app->setCssTheme( "polished" );
	app->root()->addWidget( std::make_unique<WebGamesApp>() );

	return app;
}


int main( int argc, char** argv ) {

	//CreateDB();
	//FillDB();
	Session::ConfigureAuth();
	try {
		Wt::WServer server( argc, argv, WTHTTP_CONFIGURATION );

		server.addEntryPoint( Wt::EntryPointType::Application, createApplication );

		Session::ConfigureAuth();

		server.run();
	}
	catch ( Wt::WServer::Exception& e ) {
		std::cerr << e.what() << std::endl;
	}
	catch ( std::exception& e ) {
		std::cerr << "exception: " << e.what() << std::endl;
	}
	return 0;
}
