#pragma once
#include "cMain.h"

class capp : public wxApp
{
public:
	capp();
	~capp();
	cMain* getFrame();
	virtual bool OnInit();

private:
	cMain* m_frame1 = nullptr;
};