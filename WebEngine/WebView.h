/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef SHELL_WEBENGINE_VIEW_H
#define SHELL_WEBENGINE_VIEW_H

#include <QWebEngineView>

class WebView : public QWebEngineView {
    Q_OBJECT

public:
    explicit WebView(QWidget* parent = nullptr);
    ~WebView();

};


#endif //SHELL_WEBENGINE_VIEW_H
