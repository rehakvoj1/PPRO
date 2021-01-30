#pragma once
#include <Wt/WContainerWidget.h>

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

	int m_isogramLen;
	int m_bulls;
	int m_cows;
	bool m_gameWon;
	std::string m_hiddenWord;

	Wt::WText* m_bullsCows;
	Wt::WLineEdit* m_userInput;

	WebGamesApp* m_app;
	Session* m_session;
};

