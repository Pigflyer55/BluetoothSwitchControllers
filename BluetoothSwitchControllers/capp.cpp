#include "capp.h"

wxIMPLEMENT_APP(capp);

capp::capp()
{
}

capp::~capp()
{
}

bool capp::OnInit()
{
	SetAppName("bluetooth_switch_controller");
	SetAppDisplayName("Switch Controller PC");
	m_frame1 = new cMain();
	
	m_frame1->Show();
	return true;
}
