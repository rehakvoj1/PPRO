#pragma once
#include <Wt/WContainerWidget.h>
#include <Wt/WImage.h>
#include <Wt/WStackedWidget.h>
#include <vector>

class WebGamesApp;

class HangmanGame : public Wt::WContainerWidget {
public:
	HangmanGame(WebGamesApp* app);

private:
	Wt::WStackedWidget* m_hangmanImg;
	std::vector<Wt::WImage*> m_images;
	WebGamesApp* m_app;
};

