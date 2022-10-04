#pragma once

#include <ShlObj.h>

class CTransferMediumItem
    : public ITransferMediumItem
{
public:
    explicit CTransferMediumItem(PCIDLIST_ABSOLUTE pidl);
    virtual ~CTransferMediumItem();

    // IUnknown
    IFACEMETHODIMP QueryInterface(REFIID riid, void** ppv);
    IFACEMETHODIMP_(ULONG) AddRef();
    IFACEMETHODIMP_(ULONG) Release();

    // ITransferMediumItem - IRelatedItem
    IFACEMETHODIMP GetItemIDList(PIDLIST_ABSOLUTE* ppidl);
    IFACEMETHODIMP GetItem(IShellItem** ppsi);

private:
    long m_cRef;
    PIDLIST_ABSOLUTE p_pidl;
};
