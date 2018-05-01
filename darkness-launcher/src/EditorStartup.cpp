#include "EditorStartup.h"
#include <QProcess>
#include <QFileInfo>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>

EditorStartup::EditorStartup(QObject* parent, const QString& projectPath)
{
    QString program = getEditorPath();
    qDebug() << program;
    QStringList arguments;
    arguments << "-project" << projectPath;
    qDebug() << arguments;

    QProcess* editorProcess = new QProcess(parent);
    QFileInfo info(projectPath);
    editorProcess->setWorkingDirectory(info.absolutePath());
    editorProcess->startDetached(program, arguments);
}

QString EditorStartup::getEditorPath() const
{
    return QFileInfo(
        QCoreApplication::applicationDirPath() + 
        QDir::separator() + 
        "../../../../darkness-editor/bin/x64/Release DX12/DarknessEditor.exe")
        .absoluteFilePath();
}
