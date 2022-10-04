#pragma once

#include <vector>

#include <ShlObj.h>

#include "ExplorerDataProvider.h"

class CDataObject : public IDataObject
{
public:
    explicit CDataObject(CFolderViewImplFolder* const parent, const UINT cidl, PCUITEMID_CHILD_ARRAY apidl);
    virtual ~CDataObject();

    // IUnknown
    IFACEMETHODIMP QueryInterface(REFIID riid, void** ppv);
    IFACEMETHODIMP_(ULONG) AddRef();
    IFACEMETHODIMP_(ULONG) Release();

    // IDataObject
    IFACEMETHODIMP GetData(FORMATETC* pformatetcIn, STGMEDIUM* pmedium);
    IFACEMETHODIMP GetDataHere(FORMATETC* pformatetc, STGMEDIUM* pmedium);
    IFACEMETHODIMP QueryGetData(FORMATETC* pformatetc);
    IFACEMETHODIMP GetCanonicalFormatEtc(FORMATETC* pformatectIn, FORMATETC* pformatetcOut);
    IFACEMETHODIMP SetData(FORMATETC* pformatetc, STGMEDIUM* pmedium, BOOL fRelease);
    IFACEMETHODIMP EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC** ppenumFormatEtc);
    IFACEMETHODIMP DAdvise(FORMATETC* pformatetc, DWORD advf, IAdviseSink* pAdvSink, DWORD* pdwConnection);
    IFACEMETHODIMP DUnadvise(DWORD dwConnection);
    IFACEMETHODIMP EnumDAdvise(IEnumSTATDATA** ppenumAdvise);

private:
    // Used formats
    static CLIPFORMAT s_FILEDESCRIPTORW; // Clipboard format for CFSTR_FILEDESCRIPTORW
    static CLIPFORMAT s_FILECONTENTS;    // Clipboard format for CFSTR_FILECONTENTS

    CFolderViewImplFolder* const p_parent;
    long m_cRef;

    std::vector<PUITEMID_CHILD> m_pidlList;
    SIZE_T m_FileGroupDescriptorSize;
    LPFILEGROUPDESCRIPTOR p_FileGroupDescriptor;
};
