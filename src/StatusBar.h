#pragma once

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class StatusProgressBar : public wxStatusBar {
	wxGauge* gauge;
	std::string description, currentstep;
	int steps, current;
	float progress();
public:
	StatusProgressBar(wxWindow* parent);
	
	void newProcess(int steps, std::string description);
	void nextProcessStep();
	void nextProcessStep(std::string description);
	void finishProcess();
};
