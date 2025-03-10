#include "App.h"
#include "MainWindow.h"


wxIMPLEMENT_APP(App);

App::App() {

}

App::~App() {

}

bool App::OnInit() {
	mWindow = new MainWindow();
	mWindow->Show();
	return true;
}