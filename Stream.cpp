#include "Stream.h"

#include <Shlwapi.h>

#include "Utils.h"
#include "resource.h"

#define SLEEP_MS 10

Stream::Stream()
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
Stream::~Stream()
{
    if (p_stream)
        p_stream->Release();
}
IFACEMETHODIMP Stream::QueryInterface(REFIID riid, void** ppv)
{
    static const QITAB qit[] =
    {
        QITABENT(Stream, ISequentialStream),
        QITABENT(Stream, IStream),
        { 0 },
    };
    return ::QISearch(this, qit, riid, ppv);
}
IFACEMETHODIMP_(ULONG) Stream::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}
IFACEMETHODIMP_(ULONG) Stream::Release()
{
    long cRef = InterlockedDecrement(&m_cRef);
    if (0 == cRef)
    {
        delete this;
    }
    return cRef;
}
IFACEMETHODIMP Stream::Read(void* pv, ULONG cb, ULONG* pcbRead)
{
    ::Sleep(SLEEP_MS);
    return p_stream->Read(pv, cb, pcbRead);
}
IFACEMETHODIMP Stream::Write(const void* pv, ULONG cb, ULONG* pcbWritten)
{
    ::Sleep(SLEEP_MS);
    return p_stream->Write(pv, cb, pcbWritten);
}
IFACEMETHODIMP Stream::Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER* plibNewPosition)
{
    ::Sleep(SLEEP_MS);
    return p_stream->Seek(dlibMove, dwOrigin, plibNewPosition);
}
IFACEMETHODIMP Stream::SetSize(ULARGE_INTEGER libNewSize)
{
    ::Sleep(SLEEP_MS);
    return p_stream->SetSize(libNewSize);
}
IFACEMETHODIMP Stream::CopyTo(IStream* pstm, ULARGE_INTEGER cb, ULARGE_INTEGER* pcbRead, ULARGE_INTEGER* pcbWritten)
{
    ::Sleep(SLEEP_MS);
    return p_stream->CopyTo(pstm, cb, pcbRead, pcbWritten);
}
IFACEMETHODIMP Stream::Commit(DWORD grfCommitFlags)
{
    ::Sleep(SLEEP_MS);
    return p_stream->Commit(grfCommitFlags);
}
IFACEMETHODIMP Stream::Revert(void)
{
    ::Sleep(SLEEP_MS);
    return p_stream->Revert();
}
IFACEMETHODIMP Stream::LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
{
    ::Sleep(SLEEP_MS);
    return p_stream->LockRegion(libOffset, cb, dwLockType);
}
IFACEMETHODIMP Stream::UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
{
    ::Sleep(SLEEP_MS);
    return p_stream->UnlockRegion(libOffset, cb, dwLockType);
}
IFACEMETHODIMP Stream::Stat(STATSTG* pstatstg, DWORD grfStatFlag)
{
    ::Sleep(SLEEP_MS);
    return p_stream->Stat(pstatstg, grfStatFlag);
}
IFACEMETHODIMP Stream::Clone(IStream** ppstm)
{
    ::Sleep(SLEEP_MS);
    return p_stream->Clone(ppstm);
}