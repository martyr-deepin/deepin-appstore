/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QDebug>
#include <QNetworkProxyFactory>

#include "Shell.h"

int main(int argc, char *argv[]) {
    QNetworkProxyFactory::setUseSystemConfiguration(true);

    // The matching client version code in frontend(jssrc/services/AboutContentService.js)
    // should be sync with the ApplicationName here.
    Shell::setApplicationName("deepin-appstore");
    Shell::setApplicationDisplayName(QObject::tr("Deepin Store"));
    Shell::setApplicationVersion(SHELL_VERSION);
    Shell::setOrganizationDomain("deepin.org");
    Shell::setOrganizationName("deepin");

    Shell::loadDXcbPlugin();

    Shell shell(argc, argv);
    int result = shell.exec();
    return result;
}
