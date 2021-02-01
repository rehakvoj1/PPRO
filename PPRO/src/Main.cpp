#include <Wt/WApplication.h>
#include <Wt/WServer.h>
#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Sqlite3.h>
#include <fstream>


#include "Session.h"

#include "DAO/Game.h"
#include "DAO/Riddle.h"
#include "DAO/Score.h"
#include "DAO/User.h"
#include "DAO/Word.h"
#include "DAO/Song.h"
#include "Widgets/WebGamesApp.h"

void FillDB() {
	Session session;
	Wt::Dbo::Transaction transaction{ session };
	
	std::ifstream file( "nouns.txt" );
	std::string str;
	while ( std::getline( file, str ) ) {
		std::unique_ptr<Word> word{ new Word(str, "en") };
		Wt::Dbo::ptr<Word> wordPtr = session.add( std::move( word ) );
	}

	std::ifstream file2( "riddles.txt" );
	std::string riddleStr;
	std::string answerStr;
	while ( std::getline( file2, riddleStr ) && std::getline( file2, answerStr ) ) {
		std::unique_ptr<Riddle> riddle{ new Riddle( riddleStr, "en", answerStr ) };
		Wt::Dbo::ptr<Riddle> riddlePtr = session.add( std::move( riddle ) );
	}

	std::ifstream file3( "songlist.txt" );
	std::string artistStr;
	std::string titleStr;
	while ( std::getline( file3, artistStr ) && std::getline( file3, titleStr ) ) {
		std::unique_ptr<Song> song{ new Song( artistStr, titleStr ) };
		Wt::Dbo::ptr<Song> riddlePtr = session.add( std::move( song ) );
	}

	std::unique_ptr<Game> hangman{ new Game( "Hangman", "Try not to hang!" ) };
	Wt::Dbo::ptr<Game> hangmanPtr = session.add( std::move( hangman ) );

	std::unique_ptr<Game> BullsNCows{ new Game( "BullsNCows", "Weee!!! Animals!!" ) };
	Wt::Dbo::ptr<Game> BullsNCowsPtr = session.add( std::move( BullsNCows ) );

	std::unique_ptr<Game> riddles{ new Game( "Riddles", "That is the riddle!" ) };
	Wt::Dbo::ptr<Game> riddlesPtr = session.add( std::move( riddles ) );

	std::unique_ptr<Game> guessTheSong{ new Game( "GuessTheSong", "Daba Deee Daba dAAA!!!" ) };
	Wt::Dbo::ptr<Game> guessTheSongPtr = session.add( std::move( guessTheSong ) );
}



std::unique_ptr<Wt::WApplication> createApplication( const Wt::WEnvironment& env ) {
	auto app = std::make_unique<Wt::WApplication>( env );
	app->setCssTheme( "bootstrap" );
	app->useStyleSheet( "css/hangman.css" );
	app->useStyleSheet( "css/riddles.css" );
	app->useStyleSheet( "css/bullsNCows.css" );
	app->useStyleSheet( "css/guessTheSong.css" );
	app->root()->addWidget( std::make_unique<WebGamesApp>() );

	return app;
}


int main( int argc, char** argv ) {

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
