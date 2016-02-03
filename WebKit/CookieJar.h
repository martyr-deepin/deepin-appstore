/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/
#ifndef SHELL_COOKIEJAR_H
#define SHELL_COOKIEJAR_H

#include <QNetworkCookieJar>

class CookieJar : public QNetworkCookieJar {
public:
    explicit CookieJar(QObject* parent = nullptr);
    ~CookieJar();
};


#endif //SHELL_COOKIEJAR_H
