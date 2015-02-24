wxToolBox
=========

wxToolBox

wxToolBox is a powerful and extremely customizable skinnable component which allows you to incorporate VS.NET-like toolbox or Outlook-like side bar into your applications
wxToolBox is a good choice if you want to develop professional-looking software. Here are some features of wxToolBox:

* Multiple tabs
* Multiple items on each tab
* Different item types: List, Icons, Thumbnails
* Loading/Saving tabs and items from/to file
* Skins, loading/saving skins from/to file
* Customizable background colours, background images, shadows and other graphics
* Tabs can contain wxWidgets controls instead of items
* Online documentation and samples
* Visual skin editor

Compilation under OS X
----------------------

The component has been tested under OS X 10.10 (Yosemite)
You need to have a shared build of wxWidgets compiled with following flags (with support of C++11)

`../configure --with-osx_cocoa --with-macosx-version-min=10.7 --with-macosx-sdk=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.10.sdk CC=clang CXX=clang++ CXXFLAGS="-stdlib=libc++ -std=c++11" OBJCXXFLAGS="-stdlib=libc++ -std=c++11" LDFLAGS=-stdlib=libc++ --enable-shared --disable-static --enable-unicode --enable-debug`
