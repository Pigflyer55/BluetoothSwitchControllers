#pragma once
#include "wx/wx.h"
#include "cMain.h"

class capp : public wxApp
{
public:
	capp();
	~capp();

private:
	cMain* m_frame1 = nullptr;

public:
	virtual bool OnInit();
};