#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <string>
#include <wx/wx.h>
#include <wx/clipbrd.h>



class Application : public wxApp
{
public:
	virtual bool OnInit();
};

#endif