#include "ShellItemResources.h"

#include <new>

#include <Shlwapi.h>

#include "Stream.h"
#include "EnumResources.h"
#include "Guid.h"
#include "resource.h"

CShellItemResources::CShellItemResources(CFolderViewImplFolder* const parent, PCUITEMID_CHILD pidl)
    : IShellItemResources()
    , m_cRef(1)
    , p_parent(parent)
    , p_pidl(::ILCloneChild(pidl))
{

}
CShellItemResources::~CShellItemResources()
{
    if (p_pidl)
        ::ILFree(p_pidl);
}
IFACEMETHODIMP CShellItemResources::QueryInterface(REFIID riid, void** ppv)
{
    static const QITAB qit[] =
    {
        QITABENT(CShellItemResources, IShellItemResources),
        { 0 },
    };

    return ::QISearch(this, qit, riid, ppv);
}
IFACEMETHODIMP_(ULONG) CShellItemResources::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}
IFACEMETHODIMP_(ULONG) CShellItemResources::Release()
{
    long cRef = InterlockedDecrement(&m_cRef);
    if (0 == cRef)
    {
        delete this;
    }
    return cRef;
}
IFACEMETHODIMP CShellItemResources::GetAttributes(DWORD* pdwAttributes)
{
    BOOL fIsFolder = FALSE;
    HRESULT hr = p_parent->_GetFolderness(p_pidl, &fIsFolder);
    if (SUCCEEDED(hr))
    {
        if (fIsFolder)
        {
            *pdwAttributes = FILE_ATTRIBUTE_DIRECTORY;
        }
        else
        {
            *pdwAttributes = FILE_ATTRIBUTE_NORMAL;
        }
    }

    return hr;
}
IFACEMETHODIMP CShellItemResources::GetSize(ULONGLONG* pullSize)
{
    BOOL fIsFolder = FALSE;
    HRESULT hr = p_parent->_GetFolderness(p_pidl, &fIsFolder);
    if (SUCCEEDED(hr))
    {
        if (fIsFolder)
        {
            *pullSize = 0;
        }
        else
        {
            HRSRC hResource = ::FindResource(g_hInst, MAKEINTRESOURCE(IDR_ASSET1), RT_RCDATA);
            if (!hResource)
                return E_FAIL;
            HGLOBAL hResourceData = ::LoadResource(g_hInst, hResource);
            if (!hResourceData)
                return E_FAIL;

            *pullSize = ::SizeofResource(g_hInst, hResource);
        }
    }

    return hr;
}
IFACEMETHODIMP CShellItemResources::GetTimes(FILETIME* pftCreation, FILETIME* pftWrite, FILETIME* pftAccess)
{
    return E_NOTIMPL;
}
IFACEMETHODIMP CShellItemResources::SetTimes(const FILETIME* pftCreation, const FILETIME* pftWrite, const FILETIME* pftAccess)
{
    return E_NOTIMPL;
}
IFACEMETHODIMP CShellItemResources::GetResourceDescription(const SHELL_ITEM_RESOURCE* pcsir, LPWSTR* ppszDescription)
{
    return E_NOTIMPL;
}
IFACEMETHODIMP CShellItemResources::EnumResources(IEnumResources** ppenumr)
{
    CEnumResources* pStream = new (std::nothrow) CEnumResources();
    HRESULT hr = pStream ? S_OK : E_OUTOFMEMORY;
    if (SUCCEEDED(hr))
    {
        hr = pStream->QueryInterface(IID_PPV_ARGS(ppenumr));
        pStream->Release();
    }

    return hr;
}
IFACEMETHODIMP CShellItemResources::SupportsResource(const SHELL_ITEM_RESOURCE* pcsir)
{
    if (pcsir->guidType == GUID_StreamResource)
        return S_OK;

    return E_FAIL;
}
IFACEMETHODIMP CShellItemResources::OpenResource(const SHELL_ITEM_RESOURCE* pcsir, REFIID riid, void** ppv)
{
    if (pcsir->guidType == GUID_StreamResource)
    {
        CStream* pStream = new (std::nothrow) CStream();
        HRESULT hr = pStream ? S_OK : E_OUTOFMEMORY;
        if (SUCCEEDED(hr))
        {
            hr = pStream->QueryInterface(riid, ppv);
            pStream->Release();
        }

        return hr;
    }

    return E_NOINTERFACE;
}
IFACEMETHODIMP CShellItemResources::CreateResource(const SHELL_ITEM_RESOURCE* pcsir, REFIID riid, void** ppv)
{
    return E_NOTIMPL;
}
IFACEMETHODIMP CShellItemResources::MarkForDelete(void)
{
    return E_NOTIMPL;
}
