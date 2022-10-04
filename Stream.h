#pragma once

#include <ShlObj.h>

class CStream : public IStream
{
public:
    explicit CStream();
    virtual ~CStream();

    // IUnknown
    IFACEMETHODIMP QueryInterface(REFIID riid, void** ppv);
    IFACEMETHODIMP_(ULONG) AddRef();
    IFACEMETHODIMP_(ULONG) Release();

    // ISequentialStream
    IFACEMETHODIMP Read(void* pv, ULONG cb, ULONG* pcbRead);
    IFACEMETHODIMP Write(const void* pv, ULONG cb, ULONG* pcbWritten);

    // IStream
    IFACEMETHODIMP Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER* plibNewPosition);
    IFACEMETHODIMP SetSize(ULARGE_INTEGER libNewSize);
    IFACEMETHODIMP CopyTo(IStream* pstm, ULARGE_INTEGER cb, ULARGE_INTEGER* pcbRead, ULARGE_INTEGER* pcbWritten);
    IFACEMETHODIMP Commit(DWORD grfCommitFlags);
    IFACEMETHODIMP Revert(void);
    IFACEMETHODIMP LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType);
    IFACEMETHODIMP UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType);
    IFACEMETHODIMP Stat(STATSTG* pstatstg, DWORD grfStatFlag);
    IFACEMETHODIMP Clone(IStream** ppstm);

private:
    IStream* p_stream;

    long m_cRef;
};
