#include "Stream.h"

#include <Shlwapi.h>

#include "Utils.h"
#include "resource.h"

#define SLEEP_MS 10

CStream::CStream()
    : IStream()
    , p_stream(nullptr)
    , m_cRef(1)
{
    HRSRC myResource = ::FindResource(g_hInst, MAKEINTRESOURCE(IDR_ASSET1), RT_RCDATA);
    if (!myResource)
        return;
    HGLOBAL myResourceData = ::LoadResource(g_hInst, myResource);
    if (!myResourceData)
        return;

    p_stream = ::SHCreateMemStream((LPBYTE)::LockResource(myResourceData), ::SizeofResource(g_hInst, myResource));
}
CStream::~CStream()
{
    if (p_stream)
        p_stream->Release();
}
IFACEMETHODIMP CStream::QueryInterface(REFIID riid, void** ppv)
{
    static const QITAB qit[] =
    {
        QITABENT(CStream, ISequentialStream),
        QITABENT(CStream, IStream),
        { 0 },
    };
    return ::QISearch(this, qit, riid, ppv);
}
IFACEMETHODIMP_(ULONG) CStream::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}
IFACEMETHODIMP_(ULONG) CStream::Release()
{
    long cRef = InterlockedDecrement(&m_cRef);
    if (0 == cRef)
    {
        delete this;
    }
    return cRef;
}
IFACEMETHODIMP CStream::Read(void* pv, ULONG cb, ULONG* pcbRead)
{
    ::Sleep(SLEEP_MS);
    return p_stream->Read(pv, cb, pcbRead);
}
IFACEMETHODIMP CStream::Write(const void* pv, ULONG cb, ULONG* pcbWritten)
{
    ::Sleep(SLEEP_MS);
    return p_stream->Write(pv, cb, pcbWritten);
}
IFACEMETHODIMP CStream::Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER* plibNewPosition)
{
    ::Sleep(SLEEP_MS);
    return p_stream->Seek(dlibMove, dwOrigin, plibNewPosition);
}
IFACEMETHODIMP CStream::SetSize(ULARGE_INTEGER libNewSize)
{
    ::Sleep(SLEEP_MS);
    return p_stream->SetSize(libNewSize);
}
IFACEMETHODIMP CStream::CopyTo(IStream* pstm, ULARGE_INTEGER cb, ULARGE_INTEGER* pcbRead, ULARGE_INTEGER* pcbWritten)
{
    ::Sleep(SLEEP_MS);
    return p_stream->CopyTo(pstm, cb, pcbRead, pcbWritten);
}
IFACEMETHODIMP CStream::Commit(DWORD grfCommitFlags)
{
    ::Sleep(SLEEP_MS);
    return p_stream->Commit(grfCommitFlags);
}
IFACEMETHODIMP CStream::Revert(void)
{
    ::Sleep(SLEEP_MS);
    return p_stream->Revert();
}
IFACEMETHODIMP CStream::LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
{
    ::Sleep(SLEEP_MS);
    return p_stream->LockRegion(libOffset, cb, dwLockType);
}
IFACEMETHODIMP CStream::UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
{
    ::Sleep(SLEEP_MS);
    return p_stream->UnlockRegion(libOffset, cb, dwLockType);
}
IFACEMETHODIMP CStream::Stat(STATSTG* pstatstg, DWORD grfStatFlag)
{
    ::Sleep(SLEEP_MS);
    return p_stream->Stat(pstatstg, grfStatFlag);
}
IFACEMETHODIMP CStream::Clone(IStream** ppstm)
{
    ::Sleep(SLEEP_MS);
    return p_stream->Clone(ppstm);
}