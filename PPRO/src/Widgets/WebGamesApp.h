#pragma once

#include <Wt/WContainerWidget.h>
#include <Wt/WStackedWidget.h>
#include "../Session.h"

	
class WebGamesApp : public Wt::WContainerWidget {
public:
	WebGamesApp();
	~WebGamesApp();

	void HandleInternalPath( const std::string& internalPath );

private:
	void OnAuthEvent();
	void ShowPage(Wt::WContainerWidget* app);

	Wt::WStackedWidget* m_mainStack;
	Wt::WContainerWidget* m_gameList;
	Wt::WContainerWidget* m_hangman;
	Wt::WContainerWidget* m_bullCows;
	Wt::WContainerWidget* m_riddles;
	Wt::WContainerWidget* m_guessSong;
	Session m_session;
};

