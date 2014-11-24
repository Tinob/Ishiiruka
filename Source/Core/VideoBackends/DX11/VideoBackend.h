
#ifndef DX11_VIDEO_BACKEND_H_
#define DX11_VIDEO_BACKEND_H_

#include "VideoCommon/VideoBackendBase.h"

namespace DX11
{

class VideoBackend : public VideoBackendHardware
{
	bool Initialize(void *)  override;
	void Shutdown()  override;

	std::string GetName() const  override;
	std::string GetDisplayName() const  override;

	void Video_Prepare()  override;
	void Video_Cleanup()  override;

	void ShowConfig(void* parent)  override;

	unsigned int PeekMessages()  override;
	void* m_window_handle;
};

}

#endif
