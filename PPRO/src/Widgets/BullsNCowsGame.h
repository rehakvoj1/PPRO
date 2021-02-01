#pragma once
#include <Wt/WContainerWidget.h>
#include "../DAO/Word.h"

class WebGamesApp;
class Session;

class BullsNCowsGame : public Wt::WContainerWidget {
public:
	BullsNCowsGame(){ }
	BullsNCowsGame( WebGamesApp* app, Session* session );
	bool IsIsogram( std::string word );

private:
	void NewGame();
	void CheckAnswer();
	void NewRandomWord();
	void SaveScore();
	void ShowHighScores();

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

