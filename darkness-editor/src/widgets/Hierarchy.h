#pragma once

#include <QDockWidget>
#include <QSplitter>
#include <QTreeView>
#include <QListView>
#include <QFileSystemModel>
#include <QHBoxLayout>
#include <memory>
#include "../settings/Settings.h"
#include "HierarchyTreeModel.h"
#include "engine/Engine.h"
#include "plugins/PluginManager.h"

class Hierarchy : public QDockWidget
{
    Q_OBJECT
public:
    explicit Hierarchy(
        const Settings& settings,
        Engine& engine,
        QMainWindow* mainWindow,
        QWidget *parent = Q_NULLPTR,
        Qt::WindowFlags flags = Qt::WindowFlags());

	HierarchyTreeModel& model()
	{
		return *m_sceneModel;
	}

	void beginModelReset()
	{
		m_sceneModel->beginModelReset();
	}
	void endModelReset()
	{
		m_sceneModel->endModelReset();
	}

public slots:
    void treeDirClicked(QModelIndex index);
    void ShowContextMenu(const QPoint& point);
    void expanded(const QModelIndex &index);

protected:
    void dropEvent(QDropEvent *dropEvent) override;

signals:
    void processDroppedItem(const QString& sourceFilePath, const QString& targetPath);
    void nodeSelected(std::shared_ptr<engine::SceneNode> node);

private:
    Engine& m_engine;
    QString m_contentPath;
    QMainWindow* m_mainWindow;
    std::unique_ptr<HierarchyTreeModel> m_sceneModel;

    std::unique_ptr<QTreeView> m_sceneView;

    QModelIndex m_expandIndex;
    QModelIndex m_newFolderIndex;

    engine::PluginManager manager;

    std::shared_ptr<engine::SceneNode> duplicate(
        std::shared_ptr<engine::SceneNode> src);
};
