#pragma once
#include <Wt/WContainerWidget.h>
#include <Wt/WImage.h>
#include <Wt/WStackedWidget.h>
#include <vector>

class Session;
class WebGamesApp;

class HangmanGame : public Wt::WContainerWidget {
public:
	HangmanGame( WebGamesApp* app, Session* session);

private:
	void ResolveLetter(Wt::WPushButton* btn);
	
	bool LetterHit( Wt::WPushButton* btn );
	bool IsGameWon();
	bool IsGameLost();
	void UpdateHiddenWord();
	void NewRandomWord();

	void NewGame();
	void LoadLetters();
	void LoadImages();

	std::vector<Wt::WImage*> m_images;

	int m_imageIdx;
	std::vector< std::pair<char,bool> > m_hiddenWord;

	Wt::WContainerWidget* m_hiddenWordContainer;
	Wt::WContainerWidget* m_lettersContainer;
	Wt::WStackedWidget* m_hangmanImg;
	WebGamesApp* m_app;
	Session* m_session;
};

