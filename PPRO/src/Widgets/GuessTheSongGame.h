#pragma once
#include <Wt/WContainerWidget.h>
#include <Wt/WAudio.h>
#include <Wt/WPushButton.h>
#include <Wt/WLineEdit.h>
#include <Wt/WTimer.h>

class WebGamesApp;
class Session;

class GuessTheSongGame : public Wt::WContainerWidget {
public:
	GuessTheSongGame( WebGamesApp* app, Session* session );

private:
	void NewGame();
	void CheckAnswer();
	void NewRandomSong();
	void HintUpdate();
	void RevealSong();
	void GameEnd();

	void PlaySound();
	void StopSound();
	
	std::string m_gameName;

	std::string m_songName;
	std::string m_songPath;
	int m_hintPhase;
	bool m_gameWon;

	Wt::WContainerWidget* m_songContainer;
	Wt::WAudio* m_song;
	Wt::WPushButton* m_playButton;
	Wt::WPushButton* m_stopButton;
	Wt::WText* m_artist;
	Wt::WText* m_songWidget;
	Wt::WLineEdit* m_userInput;
	Wt::WPushButton* m_hintBtn;
	Wt::WTimer* m_timer;


	WebGamesApp* m_app;
	Session* m_session;
};

