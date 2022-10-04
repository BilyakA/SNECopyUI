#pragma once

#include <vector>

#include <ShlObj.h>

class CEnumResources
    : public IEnumResources
{
public:
    explicit CEnumResources();
    virtual ~CEnumResources();

    // IUnknown
    IFACEMETHODIMP QueryInterface(REFIID riid, void** ppv);
    IFACEMETHODIMP_(ULONG) AddRef();
    IFACEMETHODIMP_(ULONG) Release();

    // IEnumResources
    IFACEMETHODIMP Next(ULONG celt, SHELL_ITEM_RESOURCE* psir, ULONG* pceltFetched);
    IFACEMETHODIMP Skip(ULONG celt);
    IFACEMETHODIMP Reset(void);
    IFACEMETHODIMP Clone(IEnumResources** ppenumr);

private:
    long m_cRef;
    std::vector<SHELL_ITEM_RESOURCE> m_Resources;
    std::vector<SHELL_ITEM_RESOURCE>::iterator m_CurrentResource;
};
