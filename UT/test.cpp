#include "pch.h"
#include "../PPRO/src/Session.h"
#include "../PPRO/src/Widgets/WebGamesApp.h"
#include "../PPRO/src/Widgets/BullsNCowsGame.h"
#include "Wt/WServer.h"
#include "../PPRO/src/DAO/DAO.h"
#include "../PPRO/src/Session.h"
#include "../PPRO/src/Widgets/RiddlesGame.h"


namespace UnitTests { 
	TEST( BullsNCows, Isogram ) {
		BullsNCowsGame game;
		EXPECT_FALSE( game.IsIsogram( "hello" ) );
		EXPECT_TRUE( game.IsIsogram( "ahoj" ) );
	}
	
	TEST( DAO, dao ) {
		Session session;
		
		DAO dao(&session);
		auto word = dao.Create<Word>( std::make_unique<Word>( "TESTWORD", "TEST" ) );
		EXPECT_TRUE( word.get() != nullptr );
		
		auto queryWord = dao.FindOneByCondition<Word, std::string>( "word = ?", "TESTWORD" );
		EXPECT_TRUE( word->m_word == queryWord->m_word );
	
		
		dao.Delete<Word>( queryWord.id() );
		queryWord = dao.FindOneByCondition<Word, std::string>( "word = ?", "TESTWORD" );
		EXPECT_TRUE( queryWord.get() == nullptr );
	
	
	}

	TEST( RIDDLES, CHECK_ANSWER_CORRECT ) {
		RiddlesGame game( "answer" );
		EXPECT_TRUE( game.CheckAnswer( "answer" ) );
	}

	TEST( RIDDLES, CHECK_ANSWER_INCORRECT ) {
		RiddlesGame game( "answer" );
		EXPECT_FALSE( game.CheckAnswer( "nonsense" ) );
	}
}