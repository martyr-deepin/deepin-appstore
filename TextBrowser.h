/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/
#ifndef SHELL_TEXTBROWSER_H
#define SHELL_TEXTBROWSER_H

#include <QTextBrowser>

class TextBrowser : public QTextBrowser {
    Q_OBJECT
public:
    TextBrowser(QWidget* parent = nullptr);
    ~TextBrowser();

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    unsigned int borderRadius = 0;
};


#endif //SHELL_TEXTBROWSER_H
