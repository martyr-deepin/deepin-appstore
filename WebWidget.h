
#ifndef SHELL_WEBWIDGET_H
#define SHELL_WEBWIDGET_H

#ifdef BUILD_WITH_WEBENGINE
    #include "WebEngine/WebView.h"
    #include "WebEngine/WebPage.h"
    #define WebWidgetName "WebEngine"
#else
    #include "WebKit/WebView.h"
    #include "WebKit/WebPage.h"
    #define WebWidgetName "WebKit"
#endif


#endif //SHELL_WEBWIDGET_H
