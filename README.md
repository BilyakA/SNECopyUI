
Minimal reproducible example for https://stackoverflow.com/q/73938149
================================
In order to see the problem:

1. build, register NSE as-is and copy any file from it (folder copy is not supported) to local filesystem.
You will see old-style copy dialog (as in Win XP).
![IDataObject copy UI](https://i.stack.imgur.com/kwAkP.png)

2. Edit `ExplorerDataProvider.cpp:607` : uncomment `return E_NOINTERFACE;`. This will disable use of `IDataObject`.

3. build and copy any file from NSE. You will see new-style copy dialog (as in Win >Vista).
![ITransferSource copy UI](https://i.stack.imgur.com/I2BEa.png)

Some notes:
* Tested under Windows 11 21H2 64bit

* Accessing the file via `IStream` is done intentionally slow (via `Sleep`) because otherwise the copy  process was too fast for old-style dialog to appear.

* Code is far away from production quality. This is just a minimal reproducible example of the 
 problem based on the microsot sample.
https://github.com/microsoft/Windows-classic-samples/tree/main/Samples/Win7Samples/winui/shell/shellextensibility/explorerdataprovider
