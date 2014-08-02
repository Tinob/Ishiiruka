// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#pragma once

#ifdef _WIN32
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/wglew.h>
#endif

#include <string>
#include "DolphinWX/GLInterface/InterfaceBase.h"

class cInterfaceWGL : public cInterfaceBase
{
public:
	void SwapInterval(int Interval);
	void Swap();
	void UpdateFPSDisplay(const std::string& text);
	void* GetFuncAddress(const std::string& name);
	bool Create(void *&window_handle);
	bool MakeCurrent();
	bool ClearCurrent();
	void Shutdown();

	void Update();
	bool PeekMessages();

	void* m_window_handle;
};
