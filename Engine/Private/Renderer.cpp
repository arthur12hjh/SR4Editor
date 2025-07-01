#include "Renderer.h"

#include "GameObject.h"
#include "AlphaObject.h"
#include "UIObject.h"

CRenderer::CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device)
    : m_pGraphic_Device { pGraphic_Device }
{
    Safe_AddRef(m_pGraphic_Device);
}

HRESULT CRenderer::Initialize()
{
    return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDER eRenderGroup, CGameObject* pRenderObject)
{
	if (nullptr == pRenderObject)
		return E_FAIL;

 
  	m_RenderObjects[ENUM_CLASS(eRenderGroup)].push_back(pRenderObject);

	Safe_AddRef(pRenderObject);

	return S_OK;
}

void CRenderer::Render()
{
	Render_Priority();
	Render_NonBlend();
	Render_Blend();
	Render_Stencil();
	Render_Particle();
	Render_UI();
	Render_BUFFERUI();
}

void CRenderer::Render_Priority()
{
	for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDER::PRIORITY)])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[ENUM_CLASS(RENDER::PRIORITY)].clear();
}

void CRenderer::Render_NonBlend()
{
	for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDER::NONBLEND)])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[ENUM_CLASS(RENDER::NONBLEND)].clear();
}

void CRenderer::Render_Blend()
{
	m_RenderObjects[ENUM_CLASS(RENDER::BLEND)].sort([](CGameObject* pSour, CGameObject* pDest)->_bool
		{
			return static_cast<CAlphaObject*>(pSour)->Get_CamDistance() > static_cast<CAlphaObject*>(pDest)->Get_CamDistance();
		});


	for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDER::BLEND)])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[ENUM_CLASS(RENDER::BLEND)].clear();
}

void CRenderer::Render_Stencil()
{
	m_RenderObjects[ENUM_CLASS(RENDER::STENCIL)].sort([](CGameObject* pSour, CGameObject* pDest)->_bool
		{
			return static_cast<CAlphaObject*>(pSour)->Get_CamDistance() > static_cast<CAlphaObject*>(pDest)->Get_CamDistance();
		});


	for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDER::STENCIL)])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[ENUM_CLASS(RENDER::STENCIL)].clear();
}

void CRenderer::Render_Particle()
{
	m_RenderObjects[ENUM_CLASS(RENDER::PARTICLE)].sort([](CGameObject* pSour, CGameObject* pDest)->_bool
		{
			return static_cast<CAlphaObject*>(pSour)->Get_CamDistance() > static_cast<CAlphaObject*>(pDest)->Get_CamDistance();
		});


	for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDER::PARTICLE)])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[ENUM_CLASS(RENDER::PARTICLE)].clear();
}

void CRenderer::Render_UI()
{
	for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDER::UI)])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[ENUM_CLASS(RENDER::UI)].clear();
}

void CRenderer::Render_BUFFERUI()
{
	m_RenderObjects[ENUM_CLASS(RENDER::BUFFERUI)].sort([](CGameObject* pSour, CGameObject* pDest)->_bool
		{
			return static_cast<CUIObject*>(pSour)->Get_ZBuffer() < static_cast<CUIObject*>(pDest)->Get_ZBuffer();
		});

	for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDER::BUFFERUI)])
	{
		pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[ENUM_CLASS(RENDER::BUFFERUI)].clear();
}


CRenderer* CRenderer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRenderer* pInstance = new CRenderer(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CRenderer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRenderer::Free()
{
    __super::Free();

	for (auto& RenderObjects : m_RenderObjects)
	{
		for (auto& pRenderObject : RenderObjects)
			Safe_Release(pRenderObject);
		RenderObjects.clear();
	}

	Safe_Release(m_pGraphic_Device);


}
