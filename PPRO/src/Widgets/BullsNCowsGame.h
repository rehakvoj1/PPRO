#pragma once
#include <Wt/WContainerWidget.h>
#include "../DAO/Word.h"

class WebGamesApp;
class Session;

class BullsNCowsGame : public Wt::WContainerWidget {
public:
	BullsNCowsGame( WebGamesApp* app, Session* session );

private:
	void NewGame();
	bool IsIsogram( std::string word );
	void CheckAnswer();
	void NewRandomWord();
	void SaveScore();

	std::string m_gameName;

	int m_isogramLen;
	int m_bulls;
	int m_cows;
	int m_score;
	std::string m_hiddenWord;
	Wt::Dbo::ptr<Word> m_wordPtr;

	Wt::WText* m_bullsCows;
	Wt::WLineEdit* m_userInput;

	WebGamesApp* m_app;
	Session* m_session;
};

