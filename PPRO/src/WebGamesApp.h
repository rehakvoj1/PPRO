#pragma once

#include <Wt/WContainerWidget.h>
#include <Wt/WStackedWidget.h>
#include "Session.h"

	
class WebGamesApp : public Wt::WContainerWidget {
public:
	WebGamesApp();

	void HandleInternalPath( const std::string& internalPath );

private:
	void OnAuthEvent();
	void ShowGameList();

	Wt::WStackedWidget* m_mainStack;
	Session m_session;
};

