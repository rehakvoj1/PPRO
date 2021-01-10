#pragma once
#include <Wt/WContainerWidget.h>
#include <Wt/WImage.h>
#include <Wt/WStackedWidget.h>
#include <vector>

class HangmanGame : public Wt::WContainerWidget {
public:
	HangmanGame();

private:
	Wt::WStackedWidget* m_hangmanImg;
	std::vector<Wt::WImage*> m_images;
};

