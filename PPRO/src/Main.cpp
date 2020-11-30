#include <Wt/WApplication.h>
#include <Wt/WServer.h>
#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Sqlite3.h>

#include "Game.h"
#include "Riddle.h"
#include "Score.h"
#include "User.h"
#include "Word.h"



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



int main( int argc, char** argv ) {

	CreateDB();
	FillDB();
	return 0;
}
