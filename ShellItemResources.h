#pragma once

#include <ShlObj.h>

#include "ExplorerDataProvider.h"

class CShellItemResources
    : public IShellItemResources
{
public:
    explicit CShellItemResources(CFolderViewImplFolder* const parent, PCUITEMID_CHILD pidl);
    virtual ~CShellItemResources();

    // IUnknown
    IFACEMETHODIMP QueryInterface(REFIID riid, void** ppv);
    IFACEMETHODIMP_(ULONG) AddRef();
    IFACEMETHODIMP_(ULONG) Release();

    // IShellItemResources
    IFACEMETHODIMP GetAttributes(DWORD* pdwAttributes);
    IFACEMETHODIMP GetSize(ULONGLONG* pullSize);
    IFACEMETHODIMP GetTimes(FILETIME* pftCreation, FILETIME* pftWrite, FILETIME* pftAccess);
    IFACEMETHODIMP SetTimes(const FILETIME* pftCreation, const FILETIME* pftWrite, const FILETIME* pftAccess);
    IFACEMETHODIMP GetResourceDescription(const SHELL_ITEM_RESOURCE* pcsir, LPWSTR* ppszDescription);
    IFACEMETHODIMP EnumResources(IEnumResources** ppenumr);
    IFACEMETHODIMP SupportsResource(const SHELL_ITEM_RESOURCE* pcsir);
    IFACEMETHODIMP OpenResource(const SHELL_ITEM_RESOURCE* pcsir, REFIID riid, void** ppv);
    IFACEMETHODIMP CreateResource(const SHELL_ITEM_RESOURCE* pcsir, REFIID riid, void** ppv);
    IFACEMETHODIMP MarkForDelete(void);

private:
    long m_cRef;
    CFolderViewImplFolder* const p_parent;
    PUITEMID_CHILD p_pidl;
};
