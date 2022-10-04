#include "Transfer.h"

#include <Shlwapi.h>
#include <intsafe.h>

#include "ShellItemResources.h"

CTransfer::CTransfer(CFolderViewImplFolder* const parent)
    : ITransferSource()
    , p_parent(parent)
    , m_cRef(1)
    , m_SinkMap()
    , m_LastUsedSinkMapCookie(0)
{
    p_parent->AddRef();
}
CTransfer::~CTransfer()
{
    p_parent->Release();
}
IFACEMETHODIMP CTransfer::QueryInterface(REFIID riid, void** ppv)
{
    static const QITAB qit[] =
    {
        QITABENT(CTransfer, ITransferSource),
        { 0 },
    };
    return ::QISearch(this, qit, riid, ppv);
}
IFACEMETHODIMP_(ULONG) CTransfer::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}
IFACEMETHODIMP_(ULONG) CTransfer::Release()
{
    long cRef = InterlockedDecrement(&m_cRef);
    if (0 == cRef)
    {
        delete this;
    }
    return cRef;
}
IFACEMETHODIMP CTransfer::Advise(ITransferAdviseSink* psink, DWORD* pdwCookie)
{
    if (m_LastUsedSinkMapCookie == DWORD_MAX)
        m_LastUsedSinkMapCookie = 0;
    else
        m_LastUsedSinkMapCookie++;

    m_SinkMap.insert(std::pair<DWORD, ITransferAdviseSink*>(m_LastUsedSinkMapCookie, psink));
    psink->AddRef();

    *pdwCookie = m_LastUsedSinkMapCookie;

    return S_OK;
}
IFACEMETHODIMP CTransfer::Unadvise(DWORD dwCookie)
{
    m_SinkMap[dwCookie]->Release();
    return m_SinkMap.erase(dwCookie) ? S_OK : E_FAIL;
}
IFACEMETHODIMP CTransfer::SetProperties(IPropertyChangeArray* pproparray)
{
    return E_NOTIMPL;
}
IFACEMETHODIMP CTransfer::OpenItem(IShellItem* psi, TRANSFER_SOURCE_FLAGS flags, REFIID riid, void** ppv)
{
    if (riid == IID_IShellItemResources)
    {
        PIDLIST_ABSOLUTE pidl;
        HRESULT hr = ::SHGetIDListFromObject(psi, &pidl);
        if (FAILED(hr))
            return hr;

        PUITEMID_CHILD pidlChild = ::ILFindLastID(pidl);

        CShellItemResources* pShellItemResources = new (std::nothrow) CShellItemResources(p_parent, pidlChild);
        hr = pShellItemResources ? S_OK : E_OUTOFMEMORY;
        if (SUCCEEDED(hr))
        {
            hr = pShellItemResources->QueryInterface(riid, ppv);
            pShellItemResources->Release();
        }

        ::ILFree(pidl);

        return hr;
    }

    return E_NOINTERFACE;
}
IFACEMETHODIMP CTransfer::MoveItem(IShellItem* psi, IShellItem* psiParentDst, LPCWSTR pszNameDst, TRANSFER_SOURCE_FLAGS flags, IShellItem** ppsiNew)
{
    return E_NOTIMPL;
}
IFACEMETHODIMP CTransfer::RecycleItem(IShellItem* psiSource, IShellItem* psiParentDest, TRANSFER_SOURCE_FLAGS flags, IShellItem** ppsiNewDest)
{
    return E_NOTIMPL;
}
IFACEMETHODIMP CTransfer::RemoveItem(IShellItem* psiSource, TRANSFER_SOURCE_FLAGS flags)
{
    return E_NOTIMPL;
}
IFACEMETHODIMP CTransfer::RenameItem(IShellItem* psiSource, LPCWSTR pszNewName, TRANSFER_SOURCE_FLAGS flags, IShellItem** ppsiNewDest)
{
    return E_NOTIMPL;
}
IFACEMETHODIMP CTransfer::LinkItem(IShellItem* psiSource, IShellItem* psiParentDest, LPCWSTR pszNewName, TRANSFER_SOURCE_FLAGS flags, IShellItem** ppsiNewDest)
{
    return E_NOTIMPL;
}
IFACEMETHODIMP CTransfer::ApplyPropertiesToItem(IShellItem* psiSource, IShellItem** ppsiNew)
{
    return E_NOTIMPL;
}
IFACEMETHODIMP CTransfer::GetDefaultDestinationName(IShellItem* psiSource, IShellItem* psiParentDest, LPWSTR* ppszDestinationName)
{
    return psiSource->GetDisplayName(SIGDN_NORMALDISPLAY, ppszDestinationName);
}
IFACEMETHODIMP CTransfer::EnterFolder(IShellItem* psiChildFolderDest)
{
    return S_OK;
}
IFACEMETHODIMP CTransfer::LeaveFolder(IShellItem* psiChildFolderDest)
{
    return S_OK;
}
