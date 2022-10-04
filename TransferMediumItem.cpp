#include "TransferMediumItem.h"

#include <Shlwapi.h>

CTransferMediumItem::CTransferMediumItem(PCIDLIST_ABSOLUTE pidl)
    : ITransferMediumItem()
    , m_cRef(1)
    , p_pidl(::ILCloneFull(pidl))
{

}
CTransferMediumItem::~CTransferMediumItem()
{
    if (p_pidl)
        ::ILFree(p_pidl);
}
IFACEMETHODIMP CTransferMediumItem::QueryInterface(REFIID riid, void** ppv)
{
    static const QITAB qit[] =
    {
        QITABENT(CTransferMediumItem, IRelatedItem),
        QITABENT(CTransferMediumItem, ITransferMediumItem),
        { 0 },
    };

    return ::QISearch(this, qit, riid, ppv);
}
IFACEMETHODIMP_(ULONG) CTransferMediumItem::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}
IFACEMETHODIMP_(ULONG) CTransferMediumItem::Release()
{
    long cRef = InterlockedDecrement(&m_cRef);
    if (0 == cRef)
    {
        delete this;
    }
    return cRef;
}
IFACEMETHODIMP CTransferMediumItem::GetItemIDList(PIDLIST_ABSOLUTE* ppidl)
{
    *ppidl = ::ILCloneFull(p_pidl);
    if (!(*ppidl))
        return E_OUTOFMEMORY;

    return S_OK;
}
IFACEMETHODIMP CTransferMediumItem::GetItem(IShellItem** ppsi)
{
    return ::SHCreateItemFromIDList(p_pidl, IID_PPV_ARGS(ppsi));
}