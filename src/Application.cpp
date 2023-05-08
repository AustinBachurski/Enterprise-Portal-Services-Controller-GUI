#include "Application.hpp"


wxIMPLEMENT_APP(Application);

bool Application::OnInit()
{
	Frame* frame = new Frame("Window Title");
	frame->SetClientSize(600, 800);
	frame->Show();

	return true;
}