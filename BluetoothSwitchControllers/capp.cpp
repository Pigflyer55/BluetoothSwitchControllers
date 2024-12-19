#include "capp.h"


wxIMPLEMENT_APP(capp);

capp::capp()
{
}

capp::~capp()
{
}

cMain* capp::getFrame() {
	return m_frame1;
}

bool capp::OnInit()
{
	SetAppName("bluetooth_switch_controller");
	SetAppDisplayName("Switch Controller PC");
	//wxInitAllImageHandlers();
	SetTopWindow(m_frame1);
	m_frame1 = new cMain();
	
	
	m_frame1->Show();
	return true;
}
