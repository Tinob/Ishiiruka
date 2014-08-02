
#ifndef DX11_VIDEO_BACKEND_H_
#define DX11_VIDEO_BACKEND_H_

#include "VideoCommon/VideoBackendBase.h"

namespace DX11
{

class VideoBackend : public VideoBackendHardware
{
	bool Initialize(void *&);
	void Shutdown();

	std::string GetName();
	std::string GetDisplayName();

	void Video_Prepare();
	void Video_Cleanup();

	void ShowConfig(void* parent);

	void UpdateFPSDisplay(const std::string&);
	unsigned int PeekMessages();
	void* m_window_handle;
};

}

#endif
