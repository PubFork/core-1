/********************************************************************
 *             a  A
 *            AM\/MA
 *           (MA:MMD
 *            :: VD
 *           ::  º
 *          ::
 *         ::   **      .A$MMMMND   AMMMD     AMMM6    MMMM  MMMM6
 +       6::Z. TMMM    MMMMMMMMMDA   VMMMD   AMMM6     MMMMMMMMM6
 *      6M:AMMJMMOD     V     MMMA    VMMMD AMMM6      MMMMMMM6
 *      ::  TMMTMC         ___MMMM     VMMMMMMM6       MMMM
 *     MMM  TMMMTTM,     AMMMMMMMM      VMMMMM6        MMMM
 *    :: MM TMMTMMMD    MMMMMMMMMM       MMMMMM        MMMM
 *   ::   MMMTTMMM6    MMMMMMMMMMM      AMMMMMMD       MMMM
 *  :.     MMMMMM6    MMMM    MMMM     AMMMMMMMMD      MMMM
 *         TTMMT      MMMM    MMMM    AMMM6  MMMMD     MMMM
 *        TMMMM8       MMMMMMMMMMM   AMMM6    MMMMD    MMMM
 *       TMMMMMM$       MMMM6 MMMM  AMMM6      MMMMD   MMMM
 *      TMMM MMMM
 *     TMMM  .MMM
 *     TMM   .MMD       ARBITRARY·······XML········RENDERING
 *     TMM    MMA       ====================================
 *     TMN    MM
 *      MN    ZM
 *            MM,
 *
 *
 *      AUTHORS: see AUTHORS file
 *
 *      COPYRIGHT: ©2012 - All Rights Reserved
 *
 *      LICENSE: see LICENSE file
 *
 *      WEB: http://axr.vg
 *
 *      THIS CODE AND INFORMATION ARE PROVIDED "AS IS"
 *      WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 *      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *      IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 *      FITNESS FOR A PARTICULAR PURPOSE.
 *
 ********************************************************************/

#include "BrowserSettings.h"
#include <QtGlobal>
#include <QSettings>

#define key_fileLaunchAction "general/fileLaunchAction"
#define key_lastFileOpened "general/lastFileOpened"
#define key_debuggingChannelsMask "debug/channelsMask"

class BrowserSettings::Private
{
public:
    QSettings *settings;
};

BrowserSettings::BrowserSettings()
: d(new Private)
{
    d->settings = new QSettings();
}

BrowserSettings::~BrowserSettings()
{
    delete d->settings;
    delete d;
}

QSettings* BrowserSettings::settings() const
{
    return d->settings;
}

BrowserSettings::FileLaunchAction BrowserSettings::fileLaunchAction() const
{
    return static_cast<FileLaunchAction>(qBound(0, d->settings->value(key_fileLaunchAction, 0).toInt(), static_cast<int>(FileLaunchActionMax) - 1));
}

void BrowserSettings::setFileLaunchAction(FileLaunchAction action)
{
    d->settings->setValue(key_fileLaunchAction, static_cast<int>(action));
}

QString BrowserSettings::lastFileOpened() const
{
    return d->settings->value(key_lastFileOpened).toString();
}

void BrowserSettings::setLastFileOpened(const QString &filePath)
{
    d->settings->setValue(key_lastFileOpened, filePath);
}

quint32 BrowserSettings::debuggingChannelsMask() const
{
    return static_cast<quint32>(d->settings->value(key_debuggingChannelsMask, 0).toULongLong());
}

void BrowserSettings::setDebuggingChannelsMask(quint32 mask)
{
    d->settings->setValue(key_debuggingChannelsMask, static_cast<qulonglong>(mask));
}
