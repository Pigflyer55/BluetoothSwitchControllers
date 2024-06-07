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
	m_frame1 = new cMain();
	m_frame1->Show();
	return true;
}
