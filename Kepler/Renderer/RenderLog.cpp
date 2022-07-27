#include "kepch.h"
#include "RenderLog.h"

namespace kepler {

	ImPlotLineData::ImPlotLineData()
		: m_pKey{ new float[s_size] }
		, m_pValue{ new float[s_size] }
		, m_curSize{ 0 }
	{
		for (int i = 0; i < s_size; i++)
		{
			m_pKey[i] = 0;
			m_pValue[i] = 0;
		}
	}

	ImPlotLineData::~ImPlotLineData()
	{
		delete[] m_pKey;
		delete[] m_pValue;
	}

	void ImPlotLineData::Add(const float value)
	{
		if (m_curSize >= s_size - 1)
		{
			m_curSize = s_size - 1;
			for (int i = 0; i < ImPlotLineData::s_size - 1; i++)
			{
				m_pKey[i] = (float)i;
				m_pValue[i] = m_pValue[i + 1];
			}
		}
		m_pKey[m_curSize] = (float)m_curSize;
		m_pValue[m_curSize] = value;
		m_curSize++;
	}
}
