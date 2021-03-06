/****************************************************************************
**
** Copyright (C) 2016 BlackBerry Limited. All rights reserved.
** Contact: KDAB (info@kdab.com)
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#include "qnxrunconfigurationfactory.h"

#include "qnxconstants.h"
#include "qnxrunconfiguration.h"
#include "qnxdevicefactory.h"

#include <projectexplorer/kitinformation.h>
#include <projectexplorer/target.h>
#include <qmakeprojectmanager/qmakeproject.h>

namespace Qnx {
namespace Internal {

static Utils::FileName pathFromId(Core::Id id)
{
    return Utils::FileName::fromString(id.suffixAfter(Constants::QNX_QNX_RUNCONFIGURATION_PREFIX));
}

QnxRunConfigurationFactory::QnxRunConfigurationFactory(QObject *parent) :
    ProjectExplorer::IRunConfigurationFactory(parent)
{
    registerRunConfiguration<QnxRunConfiguration>();
    setSupportedTargetDeviceTypes({Constants::QNX_QNX_OS_TYPE});
}

QList<Core::Id> QnxRunConfigurationFactory::availableCreationIds(ProjectExplorer::Target *parent, CreationMode mode) const
{
    using QmakeProjectManager::QmakeProject;
    if (!canHandle(parent))
        return QList<Core::Id>();

    auto project = qobject_cast<QmakeProject *>(parent->project());
    if (!project)
        return QList<Core::Id>();

    return project->creationIds(Constants::QNX_QNX_RUNCONFIGURATION_PREFIX, mode);
}

QString QnxRunConfigurationFactory::displayNameForId(Core::Id id) const
{
    const Utils::FileName path = pathFromId(id);
    if (path.isEmpty())
        return QString();

    if (id.name().startsWith(Constants::QNX_QNX_RUNCONFIGURATION_PREFIX))
        return tr("%1 on QNX Device").arg(path.toFileInfo().completeBaseName());

    return QString();
}

bool QnxRunConfigurationFactory::canCreate(ProjectExplorer::Target *parent, Core::Id id) const
{
    if (!canHandle(parent) || !id.name().startsWith(Constants::QNX_QNX_RUNCONFIGURATION_PREFIX))
        return false;

    QmakeProjectManager::QmakeProject *qt4Project = qobject_cast<QmakeProjectManager::QmakeProject *>(parent->project());
    if (!qt4Project)
        return false;

    return qt4Project->hasApplicationProFile(pathFromId(id));
}

bool QnxRunConfigurationFactory::canRestore(ProjectExplorer::Target *parent, const QVariantMap &map) const
{
    return canHandle(parent)
            && ProjectExplorer::idFromMap(map).name().startsWith(Constants::QNX_QNX_RUNCONFIGURATION_PREFIX);
}

bool QnxRunConfigurationFactory::canClone(ProjectExplorer::Target *parent, ProjectExplorer::RunConfiguration *source) const
{
    return canCreate(parent, source->id());
}

} // namespace Internal
} // namespace Qnx
