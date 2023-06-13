#ifndef PSC_ABOUT_WINDOW_HPP
#define PSC_ABOUT_WINDOW_HPP

#include <wx/frame.h>
#include <wx/hyperlink.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/wx.h>

class AboutWindow : public wxFrame
{
public:
	AboutWindow(wxWindow* parent);
};

#endif