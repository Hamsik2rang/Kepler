#include "kepch.h"
#include "RenderProfiler.h"

namespace kepler {

	void RenderProfiler::Create()
	{
		if (s_pInstance)
		{
			return;
		}
		s_pInstance = new RenderProfiler;
	}

	RenderProfiler* RenderProfiler::Get()
	{
		if (!s_pInstance)
		{
			s_pInstance = new RenderProfiler;
		}
		return s_pInstance;
	}

	void RenderProfiler::Begin()
	{
	}

	void RenderProfiler::End()
	{
	}

	void RenderProfiler::OnGUIRender()
	{
	}


}
