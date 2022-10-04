#include "EnumResources.h"

#include <Shlwapi.h>
#include <strsafe.h>

#include "Guid.h"

CEnumResources::CEnumResources()
    : IEnumResources()
    , m_cRef(1)
    , m_Resources()
    , m_CurrentResource(m_Resources.begin())
{
    // File items have stream resource
    SHELL_ITEM_RESOURCE streamResource;
    streamResource.guidType = GUID_StreamResource;
    ::StringCchCopy(streamResource.szName, ARRAYSIZE(streamResource.szName), L"");

    m_Resources.reserve(1);
    m_Resources.push_back(streamResource);

    m_CurrentResource = m_Resources.begin();
}
CEnumResources::~CEnumResources()
{

}
IFACEMETHODIMP CEnumResources::QueryInterface(REFIID riid, void** ppv)
{
    static const QITAB qit[] =
    {
        QITABENT(CEnumResources, IEnumResources),
        { 0 },
    };
    return ::QISearch(this, qit, riid, ppv);
}
IFACEMETHODIMP_(ULONG) CEnumResources::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}
IFACEMETHODIMP_(ULONG) CEnumResources::Release()
{
    long cRef = InterlockedDecrement(&m_cRef);
    if (0 == cRef)
    {
        delete this;
    }
    return cRef;
}
IFACEMETHODIMP CEnumResources::Next(ULONG celt, SHELL_ITEM_RESOURCE* psir, ULONG* pceltFetched)
{
    if (celt < 1)
        return E_INVALIDARG;

    if (!psir)
        return E_INVALIDARG;

    if (celt > 1 && !pceltFetched)
        return E_INVALIDARG;

    ULONG fetched = 0;

    for (; (m_CurrentResource != m_Resources.end()) && (fetched < celt); m_CurrentResource++)
    {
        psir[fetched] = *m_CurrentResource;
        fetched++;
    }

    if (pceltFetched)
        *pceltFetched = fetched;

    if (fetched != celt)
    {
        // It is normal to return S_FALSE to indicate end of list
        return S_FALSE;
    }

    return S_OK;

}
IFACEMETHODIMP CEnumResources::Skip(ULONG celt)
{
    for (ULONG i = 0; i < celt; i++)
    {
        if (m_CurrentResource == m_Resources.end())
        {
            return E_INVALIDARG;
        }
        m_CurrentResource++;
    }
    return S_OK;
}
IFACEMETHODIMP CEnumResources::Reset(void)
{
    m_CurrentResource = m_Resources.begin();
    return S_OK;
}
IFACEMETHODIMP CEnumResources::Clone(IEnumResources** ppenumr)
{
    *ppenumr = NULL;
    return E_NOTIMPL;
}