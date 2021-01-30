#pragma once

#include <Wt/WContainerWidget.h>
#include <Wt/WStackedWidget.h>
#include "../Session.h"

	
class WebGamesApp : public Wt::WContainerWidget {
public:
	WebGamesApp();
	~WebGamesApp();

	void HandleInternalPath( const std::string& internalPath );
	size_t GetRandomInt( size_t max);

private:
	void OnAuthEvent();
	void ShowPage(Wt::WContainerWidget* app);

	Wt::WStackedWidget* m_mainStack;
	Wt::WContainerWidget* m_gameList;
	std::map<std::string, Wt::WContainerWidget*> m_gamesPtrs;
	Session m_session;
};

