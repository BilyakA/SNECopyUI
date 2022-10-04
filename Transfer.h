#pragma once

#include <map>
#include <ShlObj.h>

#include "ExplorerDataProvider.h"

class CTransfer
    : public ITransferSource
{
public:
    explicit CTransfer(CFolderViewImplFolder* const parent);
    virtual ~CTransfer();

    // IUnknown
    IFACEMETHODIMP QueryInterface(REFIID riid, void** ppv);
    IFACEMETHODIMP_(ULONG) AddRef();
    IFACEMETHODIMP_(ULONG) Release();

    // ITransferSource
    IFACEMETHODIMP Advise(ITransferAdviseSink* psink, DWORD* pdwCookie);
    IFACEMETHODIMP Unadvise(DWORD dwCookie);
    IFACEMETHODIMP SetProperties(IPropertyChangeArray* pproparray);
    IFACEMETHODIMP OpenItem(IShellItem* psi, TRANSFER_SOURCE_FLAGS flags, REFIID riid, void** ppv);
    IFACEMETHODIMP MoveItem(IShellItem* psi, IShellItem* psiParentDst, LPCWSTR pszNameDst, TRANSFER_SOURCE_FLAGS flags, IShellItem** ppsiNew);
    IFACEMETHODIMP RecycleItem(IShellItem* psiSource, IShellItem* psiParentDest, TRANSFER_SOURCE_FLAGS flags, IShellItem** ppsiNewDest);
    IFACEMETHODIMP RemoveItem(IShellItem* psiSource, TRANSFER_SOURCE_FLAGS flags);
    IFACEMETHODIMP RenameItem(IShellItem* psiSource, LPCWSTR pszNewName, TRANSFER_SOURCE_FLAGS flags, IShellItem** ppsiNewDest);
    IFACEMETHODIMP LinkItem(IShellItem* psiSource, IShellItem* psiParentDest, LPCWSTR pszNewName, TRANSFER_SOURCE_FLAGS flags, IShellItem** ppsiNewDest);
    IFACEMETHODIMP ApplyPropertiesToItem(IShellItem* psiSource, IShellItem** ppsiNew);
    IFACEMETHODIMP GetDefaultDestinationName(IShellItem* psiSource, IShellItem* psiParentDest, LPWSTR* ppszDestinationName);
    IFACEMETHODIMP EnterFolder(IShellItem* psiChildFolderDest);
    IFACEMETHODIMP LeaveFolder(IShellItem* psiChildFolderDest);

private:
    CFolderViewImplFolder* const p_parent;
    long m_cRef;

    std::map<DWORD, ITransferAdviseSink*> m_SinkMap;
    DWORD m_LastUsedSinkMapCookie;
};
