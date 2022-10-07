#pragma once

#include "d3d11.h"


namespace kepler {

	class DX11Debug
	{
	private:
		ID3D11Debug* m_pDebug;
		ID3D11InfoQueue* m_pInfoQueue;


	public:
		DX11Debug();
		~DX11Debug();

		bool Init();
		void CheckReferenceCount();
	};


}