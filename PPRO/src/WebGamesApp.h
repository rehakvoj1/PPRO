#pragma once

#include <Wt/WContainerWidget.h>
#include "Session.h"

	
class WebGamesApp : public Wt::WContainerWidget {
public:
	WebGamesApp();


private:
	void onAuthEvent();
	Session m_session;
};

