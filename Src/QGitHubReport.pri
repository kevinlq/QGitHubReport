# Config Para
CONFIG(debug, debug|release):{
        DIR_COMPILEMODE = Debug
}
else:CONFIG(release, debug|release):{
        DIR_COMPILEMODE = Release
}

win32:{
        DIR_PLATFORM = Win32

        DESTDIR	= $$PWD/../Bin/$${DIR_PLATFORM}/$${DIR_COMPILEMODE}
}
else:mac:{
        DIR_PLATFORM = MAC
        DESTDIR  = $$PWD/../Bin/$${DIR_PLATFORM}/$${DIR_COMPILEMODE}
}
else:linux:{
        DIR_PLATFORM = Linux
        DESTDIR  = $$PWD/../Bin/$${DIR_PLATFORM}/$${DIR_COMPILEMODE}
}


DIR_DEPEND_DEST = $$PWD/../Bin/$${DIR_PLATFORM}/$${DIR_COMPILEMODE}
