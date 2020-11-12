# EosCueListLabeller
## Introduction
EosCueListLabeller is a demo project that shows how to use EosSyncLib to obtain a list of cue lists and edit their labels.

![Example Screenshot](screenshot.png?raw=true "Example Screenshot")

## Dependencies
This app was written using C++11, MSVC2019, 64-bit, Qt 5.15.1, and targeted for Windows systems. 

To compile, you will need to have a Qt5 installation (an open-source version is available at www.qt.io).
Next, define a system variable QTROOT to be the root directory for your Qt installation (usually the same directory that you have the Qt maintenance tool installed - e.g. "C:\Qt\").

Modify the QtCommon.props file (use the "Property Manager" in Visual Studio, and modify the QtCommon property group under the EosCueListLabeller project) for the particular Qt version you are using - e.g. $(QTROOT)\5.15.1\msvc2019_64

If you wish, you may also modify QtCommon.props to make $(QTDIR) target a specific directory and avoid setting system variables with QTROOT. 

The project file will automatically run Qt moc and windeployqt during the build. 


## Limitations
As a quick demo application, there are a few limitations.

The app is hard-coded to connect to 127.0.0.1 (localhost) and attempt to communicate with a running system using OSC TCP version 1.0 at port 3032. 

The app also assumes Eos is running at application launch. 

See EosCueListLabeller\MainWindow.cpp's Tick() function for the IP address and port to change.

This application also uses a copy of EosSyncLib - it would be better for it to pull this from that repo directly as a separately managed dependency or a submodule. 
