#include "DataObject.h"

#include <Shlwapi.h>
#include <strsafe.h>

#include "resource.h"
#include "Utils.h"

CLIPFORMAT CDataObject::s_FILEDESCRIPTORW = static_cast<CLIPFORMAT>(::RegisterClipboardFormat(CFSTR_FILEDESCRIPTORW));
CLIPFORMAT CDataObject::s_FILECONTENTS = static_cast<CLIPFORMAT>(::RegisterClipboardFormat(CFSTR_FILECONTENTS));

HRESULT CreateHGlobalFromBlob(const void* pvData, SIZE_T cbData, UINT uFlags, HGLOBAL* phglob)
{
    HGLOBAL hglob = ::GlobalAlloc(uFlags, cbData);
    if (hglob)
    {
        void* pvAlloc = ::GlobalLock(hglob);
        if (pvAlloc)
        {
            CopyMemory(pvAlloc, pvData, cbData);
            ::GlobalUnlock(hglob);
        }
        else
        {
            ::GlobalFree(hglob);
            hglob = NULL;
        }
    }
    *phglob = hglob;
    return hglob ? S_OK : E_OUTOFMEMORY;
}

CDataObject::CDataObject(CFolderViewImplFolder* const parent, const UINT cidl, PCUITEMID_CHILD_ARRAY apidl)
    : IDataObject()
    , p_parent(parent)
    , m_cRef(1)
    , m_pidlList()
    , m_FileGroupDescriptorSize(0)
    , p_FileGroupDescriptor(nullptr)
{
    p_parent->AddRef();

    m_pidlList.reserve(cidl);

    for (UINT i = 0; i < cidl; i++)
    {
        m_pidlList.push_back(::ILCloneChild(apidl[i]));
    }

    m_FileGroupDescriptorSize = sizeof(FILEGROUPDESCRIPTORW) - sizeof(FILEDESCRIPTORW) + (sizeof(FILEDESCRIPTORW) * m_pidlList.size());
    p_FileGroupDescriptor = reinterpret_cast<LPFILEGROUPDESCRIPTOR>(::CoTaskMemAlloc(m_FileGroupDescriptorSize));

    p_FileGroupDescriptor->cItems = static_cast<UINT>(m_pidlList.size());

    for (int i = 0; i < m_pidlList.size(); i++)
    {
        ZeroMemory(&p_FileGroupDescriptor->fgd[i], sizeof(p_FileGroupDescriptor->fgd[i]));

        HRESULT hr = p_parent->_GetName(m_pidlList[i], p_FileGroupDescriptor->fgd[i].cFileName, ARRAYSIZE(p_FileGroupDescriptor->fgd[i].cFileName));
        if (FAILED(hr))
            break;

        BOOL fIsFolder = FALSE;
        hr = p_parent->_GetFolderness(m_pidlList[i], &fIsFolder);
        if (FAILED(hr))
            break;

        if (fIsFolder)
        {
            p_FileGroupDescriptor->fgd[i].dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
            p_FileGroupDescriptor->fgd[i].dwFlags = FD_ATTRIBUTES | FD_UNICODE | FD_PROGRESSUI;
        }
        else
        {
            HRSRC hResource = ::FindResource(g_hInst, MAKEINTRESOURCE(IDR_ASSET1), RT_RCDATA);
            if (!hResource)
                break;
            HGLOBAL hResourceData = ::LoadResource(g_hInst, hResource);
            if (!hResourceData)
                break;

            LARGE_INTEGER fileSize;
            fileSize.QuadPart = ::SizeofResource(g_hInst, hResource);
            p_FileGroupDescriptor->fgd[i].nFileSizeHigh = fileSize.HighPart;
            p_FileGroupDescriptor->fgd[i].nFileSizeLow = fileSize.LowPart;
            p_FileGroupDescriptor->fgd[i].dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
            p_FileGroupDescriptor->fgd[i].dwFlags = FD_ATTRIBUTES | FD_FILESIZE | FD_UNICODE | FD_PROGRESSUI;
        }
    }
}

CDataObject::~CDataObject()
{
    p_parent->Release();

    for (size_t i = 0; i < m_pidlList.size(); i++)
    {
        ::ILFree(m_pidlList[i]);
    }

    if (p_FileGroupDescriptor)
        ::CoTaskMemFree(reinterpret_cast<LPVOID>(p_FileGroupDescriptor));
}

IFACEMETHODIMP CDataObject::QueryInterface(REFIID riid, void** ppv)
{
    static const QITAB qit[] =
    {
        QITABENT(CDataObject, IDataObject),
        { 0 },
    };
    return ::QISearch(this, qit, riid, ppv);
}

IFACEMETHODIMP_(ULONG) CDataObject::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

IFACEMETHODIMP_(ULONG) CDataObject::Release()
{
    long cRef = InterlockedDecrement(&m_cRef);
    if (0 == cRef)
    {
        delete this;
    }
    return cRef;
}

IFACEMETHODIMP CDataObject::GetData(FORMATETC* pformatetcIn, STGMEDIUM* pmedium)
{
    HRESULT hr;
    ZeroMemory(pmedium, sizeof(*pmedium));
    if (pformatetcIn->cfFormat == s_FILEDESCRIPTORW)
    {
        if (pformatetcIn->dwAspect != DVASPECT_CONTENT)
            return DV_E_DVASPECT;
        if (!(pformatetcIn->tymed & TYMED_HGLOBAL))
            return DV_E_TYMED;

        //// Common sense tells that we should only accept -1 meaning
        //// format is about all the data stored. But default explorer CDataObject accepts
        //// any lindex values still returning same info about all objects.
        //// We will stick to the default behavior.
        //if (pformatetcIn->lindex != -1)
        //    return DV_E_LINDEX;


        pmedium->tymed = TYMED_HGLOBAL;
        hr = CreateHGlobalFromBlob(p_FileGroupDescriptor, m_FileGroupDescriptorSize, GMEM_MOVEABLE, &pmedium->hGlobal);
        if (FAILED(hr))
            return hr;

        return hr;
    }
    if (pformatetcIn->cfFormat == s_FILECONTENTS)
    {
        if (pformatetcIn->dwAspect != DVASPECT_CONTENT)
            return DV_E_DVASPECT;
        if (!(pformatetcIn->tymed & TYMED_ISTREAM))
            return DV_E_TYMED;

        // We should not accept -1 here since caller should be very explicit about
        // which exact element index is requested. This is default Explorer's
        // CDataObject behavior.
        if ((pformatetcIn->lindex < 0) || (pformatetcIn->lindex >= m_pidlList.size()))
            return DV_E_LINDEX;

        const int idx = pformatetcIn->lindex;

        BOOL fIsFolder = FALSE;
        hr = p_parent->_GetFolderness(m_pidlList[idx], &fIsFolder);
        if (FAILED(hr))
            return hr;

        if (fIsFolder)
        {
            // Folder should return empty IStream
            pmedium->pstm = ::SHCreateMemStream(NULL, 0);
            if (!pmedium->pstm)
                return E_OUTOFMEMORY;
        }
        else
        {
            hr = p_parent->BindToObject(m_pidlList[idx], NULL, IID_PPV_ARGS(&pmedium->pstm));
            if (FAILED(hr))
                return hr;
        }

        pmedium->tymed = TYMED_ISTREAM;
        // set the stream position properly
        LARGE_INTEGER liZero = { 0, 0 };
        hr = pmedium->pstm->Seek(liZero, STREAM_SEEK_END, NULL);
        if (FAILED(hr))
            return hr;

        return S_OK;
    }

    return DV_E_FORMATETC;
}

IFACEMETHODIMP CDataObject::GetDataHere(FORMATETC* pformatetc, STGMEDIUM* pmedium)
{
    return E_NOTIMPL;
}

IFACEMETHODIMP CDataObject::QueryGetData(FORMATETC* pformatetc)
{
    // lindex value is not checked within QueryGetData for default Explorer's
    // CDataObject. We will stick to the default behavior.
    if (pformatetc->cfFormat == s_FILEDESCRIPTORW)
    {
        if (pformatetc->dwAspect != DVASPECT_CONTENT)
            return DV_E_DVASPECT;
        if (!(pformatetc->tymed & TYMED_HGLOBAL))
            return DV_E_TYMED;
        return S_OK;
    }
    if (pformatetc->cfFormat == s_FILECONTENTS)
    {
        if (pformatetc->dwAspect != DVASPECT_CONTENT)
            return DV_E_DVASPECT;
        if (!(pformatetc->tymed & TYMED_ISTREAM))
            return DV_E_TYMED;
        return S_OK;
    }

    return DV_E_FORMATETC;
}

IFACEMETHODIMP CDataObject::GetCanonicalFormatEtc(FORMATETC* pformatectIn, FORMATETC* pformatetcOut)
{
    *pformatetcOut = *pformatectIn;
    pformatetcOut->ptd = NULL;
    return DATA_S_SAMEFORMATETC;
}

IFACEMETHODIMP CDataObject::SetData(FORMATETC* pformatetc, STGMEDIUM* pmedium, BOOL fRelease)
{
    return E_NOTIMPL;
}

IFACEMETHODIMP CDataObject::EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC** ppenumFormatEtc)
{
    if (dwDirection == DATADIR_GET)
    {
        std::vector<FORMATETC> formatetclist;
        formatetclist.reserve(2);

        FORMATETC formatFileDescriptor;
        formatFileDescriptor.cfFormat = s_FILEDESCRIPTORW;
        formatFileDescriptor.ptd = NULL;
        formatFileDescriptor.dwAspect = DVASPECT_CONTENT;
        formatFileDescriptor.lindex = -1;
        formatFileDescriptor.tymed = TYMED_HGLOBAL;

        FORMATETC formatFileContents;
        formatFileContents.cfFormat = s_FILECONTENTS;
        formatFileContents.ptd = NULL;
        formatFileContents.dwAspect = DVASPECT_CONTENT;
        formatFileContents.lindex = -1;
        formatFileContents.tymed = TYMED_ISTREAM;

        formatetclist.push_back(formatFileDescriptor);
        formatetclist.push_back(formatFileContents);

        return ::SHCreateStdEnumFmtEtc(formatetclist.size(), formatetclist.data(), ppenumFormatEtc);
    }
    *ppenumFormatEtc = NULL;
    return E_NOTIMPL;
}

IFACEMETHODIMP CDataObject::DAdvise(FORMATETC* pformatetc, DWORD advf, IAdviseSink* pAdvSink, DWORD* pdwConnection)
{
    return OLE_E_ADVISENOTSUPPORTED;
}

IFACEMETHODIMP CDataObject::DUnadvise(DWORD dwConnection)
{
    return OLE_E_ADVISENOTSUPPORTED;
}

IFACEMETHODIMP CDataObject::EnumDAdvise(IEnumSTATDATA** ppenumAdvise)
{
    return OLE_E_ADVISENOTSUPPORTED;
}
