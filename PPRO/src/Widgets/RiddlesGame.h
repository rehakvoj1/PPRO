#pragma once
#include <Wt/WContainerWidget.h>

class Session;
class WebGamesApp;


class RiddlesGame : public Wt::WContainerWidget {
public:
	RiddlesGame( std::string answer ) : m_answer( answer ) { }
	RiddlesGame( WebGamesApp* app, Session* session );

	bool CheckAnswer(std::string input);
private:
	void NewGame();
	void NewRandomRiddle();
	void processInput();
	void ToggleSolution();

	std::string m_gameName;

	int m_score;
	std::string m_riddle;
	std::string m_answer;

	Wt::WContainerWidget* m_riddleContainer;
	Wt::WLineEdit* m_userInput;
	WebGamesApp* m_app;
	Session* m_session;

};

