#pragma once

#include <vector>
#include <string>
#include <d2d1.h>

class Obj;
class Graphics;

class Layer
{
	std::vector<Obj*>		m_vecObj;

	std::wstring		m_layerName;
	uint32_t			m_zOrder;

public:
	Layer(const std::wstring& _layerName, uint32_t _zOrder);
	~Layer();

	void AddObj(Obj* _pObj);

	template <typename T>
	T* FindObj(const std::wstring& _strName);

	const std::wstring& GetName() const { return m_layerName;}
	uint32_t GetZOrder() const { return m_zOrder; }

	void Update();
	void Render();

private:
	bool CompareObjName(Obj* _pObj, const std::wstring& _strName);
};

template<typename T>
inline T* Layer::FindObj(const std::wstring& _strName)
{
	std::vector<Obj*>::iterator iter = m_vecObj.begin();
	std::vector<Obj*>::iterator iterEnd = m_vecObj.end();

	for (; iter != iterEnd; ++iter)
	{
		if (CompareObjName(*iter, _strName))
		{
			T* pObj = dynamic_cast<T*>(*iter);
			return pObj;
		}
	}
	return nullptr;
}
