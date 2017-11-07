#include "Hierarchy.h"

#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QMainWindow>
#include <QMenu>
#include <QHeaderView>
#include <QStandardItem>
#include <QProcess>
#include <QDropEvent>
#include <QMimeData> 

using namespace std;

Hierarchy::Hierarchy(
    const Settings& settings,
    Engine& engine,
    QMainWindow* mainWindow,
    QWidget* parent,
    Qt::WindowFlags flags)
    : QDockWidget(parent, flags)
    , m_engine{ engine }
    , m_contentPath{ settings.contentPathAbsolute() }
    , m_mainWindow{ mainWindow }
    , m_sceneModel{ std::make_unique<HierarchyTreeModel>(m_engine, settings.contentPathAbsolute(), settings.processedAssetsPathAbsolute()) }
    , m_sceneView{ std::make_unique<QTreeView>(this) }
{
    manager.addFolder("C:\\work\\darkness\\darkness-coreplugins\\bin\\x64\\Debug\\");

    setWindowTitle("Hierarchy");
    setObjectName("Hierarchy");
    setWidget(m_sceneView.get());

    m_sceneView->setModel(m_sceneModel.get());
    m_sceneView->setRootIsDecorated(true);
    m_sceneView->setHeaderHidden(true);
    m_sceneView->hideColumn(1);
    m_sceneView->hideColumn(2);
    m_sceneView->hideColumn(3);
    m_sceneView->setDragDropMode(QAbstractItemView::DragDropMode::DropOnly);
    m_sceneView->setDropIndicatorShown(true);
    m_sceneView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectItems);
    m_sceneView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    m_mainWindow->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, this);

    QObject::connect(
        m_sceneView.get(), SIGNAL(clicked(QModelIndex)),
        this, SLOT(treeDirClicked(QModelIndex)));

    m_sceneView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_sceneView.get(), SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(ShowContextMenu(const QPoint&)));

    QObject::connect(
        m_sceneView.get(), SIGNAL(expanded(const QModelIndex&)),
        this, SLOT(expanded(const QModelIndex&)));

}

void Hierarchy::dropEvent(QDropEvent* dropEvent)
{
    QModelIndex index = m_sceneView->currentIndex();
    if (dropEvent->mimeData()->hasUrls())
    {
        for each (QUrl url in dropEvent->mimeData()->urls())
        {
            auto sceneIndex = m_sceneModel->index(index.row(), index.column());
            auto node = m_sceneModel->node(sceneIndex);

            auto newNode = std::make_shared<engine::SceneNode>();
            newNode->name("droppedNode");
            node->addChild(newNode);
            
            m_sceneView->update(index);
            /*emit processDroppedItem(
                url.toLocalFile(),
                m_fileSystemDirModel->filePath(index));*/
        }
    }
}

void Hierarchy::ShowContextMenu(const QPoint& point)
{
    QModelIndex index = m_sceneView->indexAt(point);
    QPoint globalPos = m_sceneView->mapToGlobal(point);

    QMenu myMenu;
    auto selectedIndexes = m_sceneView->selectionModel()->selectedIndexes();
    if (selectedIndexes.size() > 1)
    {
        // multiple selection
        myMenu.addAction("Copy");
        myMenu.addAction("Paste");
        myMenu.addSeparator();
        myMenu.addAction("Rename");
        myMenu.addAction("Duplicate");
        myMenu.addAction("Delete");
    }
    else if (index.isValid())
    {
        // single selection
        myMenu.addAction("Copy");
        myMenu.addAction("Paste");
        myMenu.addSeparator();
        myMenu.addAction("Rename");
        myMenu.addAction("Duplicate");
        myMenu.addAction("Delete");
        myMenu.addSeparator();
        myMenu.addAction("Create Empty");
        QMenu* objects3d = myMenu.addMenu("3D Object");
        objects3d->addAction("Cube");
        objects3d->addAction("Sphere");
        objects3d->addAction("Capsule");
        objects3d->addAction("Cylinder");
        objects3d->addAction("Plane");
        objects3d->addAction("Quad");
        objects3d->addAction("Terrain");
        QMenu* objects2d = myMenu.addMenu("2D Object");
        objects2d->addAction("Sprite");
        QMenu* lights = myMenu.addMenu("Light");
        lights->addAction("Directional Light");
        lights->addAction("Point Light");
        lights->addAction("Spot Light");
        myMenu.addAction("Camera");
    }
    else
    {
        // no selection
        myMenu.addAction("Create Empty");
        QMenu* objects3d = myMenu.addMenu("3D Object");
        objects3d->addAction("Cube");
        objects3d->addAction("Sphere");
        objects3d->addAction("Capsule");
        objects3d->addAction("Cylinder");
        objects3d->addAction("Plane");
        objects3d->addAction("Quad");
        objects3d->addAction("Terrain");
        QMenu* objects2d = myMenu.addMenu("2D Object");
        objects2d->addAction("Sprite");
        QMenu* lights = myMenu.addMenu("Light");
        lights->addAction("Directional Light");
        lights->addAction("Point Light");
        lights->addAction("Spot Light");
        myMenu.addAction("Camera");
    }
    QAction* selectedItem = myMenu.exec(globalPos);

    if (selectedItem)
    {
        auto newNode = std::make_shared<engine::SceneNode>();
        bool nodeAdded = false;
        if (selectedItem->text().startsWith("Camera"))
        {
            newNode->name("Camera");
            newNode->addComponent(std::make_shared<engine::Transform>());
            newNode->addComponent(std::make_shared<engine::Camera>());
            newNode->addComponent(std::make_shared<engine::PostprocessComponent>());

            //newNode->addComponent(manager.createType("Transform"));

            m_sceneModel->addNode(newNode);
            m_sceneView->update(index);

            nodeAdded = true;
        }
        else if (selectedItem->text().startsWith("Directional Light"))
        {
            newNode->name("Directional Light");
            newNode->addComponent(std::make_shared<engine::Transform>());
            auto light = std::make_shared<engine::LightComponent>();
            light->lightType(engine::LightType::Directional);
            newNode->addComponent(light);

            m_sceneModel->addNode(newNode);
            m_sceneView->update(index);

            nodeAdded = true;
        }
        else if (selectedItem->text().startsWith("Spot Light"))
        {
            newNode->name("Spot Light");
            newNode->addComponent(std::make_shared<engine::Transform>());
            auto light = std::make_shared<engine::LightComponent>();
            light->lightType(engine::LightType::Spot);
            newNode->addComponent(light);

            m_sceneModel->addNode(newNode);
            m_sceneView->update(index);

            nodeAdded = true;
        }
        else if (selectedItem->text().startsWith("Point Light"))
        {
            newNode->name("Point Light");
            newNode->addComponent(std::make_shared<engine::Transform>());
            auto light = std::make_shared<engine::LightComponent>();
            light->lightType(engine::LightType::Point);
            newNode->addComponent(light);

            m_sceneModel->addNode(newNode);
            m_sceneView->update(index);

            nodeAdded = true;
        }

        if (selectedItem->text().startsWith("Delete"))
        {
            for (auto&& ind : selectedIndexes)
            {
                auto sceneIndex = m_sceneModel->index(ind.row(), ind.column());
                m_sceneModel->removeNode(index);
                m_sceneView->update(index);
            }
        }

        if (selectedItem->text().startsWith("Duplicate"))
        {
            m_sceneModel->beginModelReset();
            for (auto&& ind : selectedIndexes)
            {
                auto sceneIndex = m_sceneModel->index(ind.row(), ind.column());
                auto node = m_sceneModel->node(sceneIndex);

                node->parent()->addChild(duplicate(node));
            }
            m_sceneModel->endModelReset();
        }

        if (nodeAdded && index.isValid())
        {
            auto sceneIndex = m_sceneModel->index(index.row(), index.column());
            auto node = m_sceneModel->node(sceneIndex);
            node->addChild(newNode);
        }

        qDebug() << "selected: " << selectedItem->text();
    }
    else
    {
        qDebug() << "no selection";
    }
}

std::shared_ptr<engine::SceneNode> Hierarchy::duplicate(std::shared_ptr<engine::SceneNode> src)
{
    auto node = std::make_shared<engine::SceneNode>();
    node->name(src->name());
    for (int i = 0; i < src->componentCount(); ++i)
    {
        node->addComponent(src->component(i)->clone());
    }
    

    for (int i = 0; i < src->childCount(); ++i)
    {
        auto newNode = std::make_shared<engine::SceneNode>();
        node->addChild(duplicate(src->child(i)));
    }
    return node;
}

void Hierarchy::expanded(const QModelIndex &index)
{
    if (index == m_expandIndex)
    {
        /*QModelIndex newFolder = m_fileSystemDirModel->mkdir(index, "New Folder");
        m_sceneView->edit(newFolder);*/
    }
}

void Hierarchy::treeDirClicked(QModelIndex index)
{
    emit nodeSelected(m_sceneModel->node(index));
}

