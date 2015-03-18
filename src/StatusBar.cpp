#include "StatusBar.h"

StatusProgressBar::StatusProgressBar(wxWindow* parent) : wxStatusBar(parent, wxID_ANY, wxST_SIZEGRIP, "Statusbar") {
	const int width = 200;
	gauge = new wxGauge(this, wxID_ANY, 100, wxPoint(parent->GetSize().GetWidth() - width - 18,2), wxSize(width, GetSize().GetHeight() - 4), wxGA_SMOOTH, wxDefaultValidator);
	gauge->SetValue(0);
}

void StatusProgressBar::newProcess(int steps_, std::string description_) {
	finishProcess();
	steps = steps_;
	current = 0;
	description = description_;
}

float StatusProgressBar::progress() {
	return (current / (float) steps);
}

void StatusProgressBar::nextProcessStep() {
	current++;
	gauge->SetValue(progress() * 100);
	char text[1000] = {0};
	std::string format = description + " - " + currentstep + " (step %i/%i)";
	sprintf(text, format.c_str(), current, steps);
	this->SetStatusText(text);
	this->Update();
}

void StatusProgressBar::nextProcessStep(std::string description) {
	currentstep = description;
	nextProcessStep();
}

void StatusProgressBar::finishProcess() {
	gauge->SetValue(100);
	this->SetStatusText("Finished \"" + description + "\"");
	this->Update();
}
