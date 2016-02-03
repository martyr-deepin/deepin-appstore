/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

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
