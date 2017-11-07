#pragma once

#include <QDockWidget>
#include <QVBoxLayout>
#include <QList>
#include <QLabel>
#include <QScrollArea>
#include <memory>
#include "../settings/Settings.h"
#include "engine/EngineComponent.h"
#include "engine/Scene.h"
#include "../drawers/DragableLabel.h"

class PropertyWidget : public QWidget
{
    Q_OBJECT
public:
    PropertyWidget(
        engine::PluginProperty typeProperty,
		const QString& contentPath,
		const QString& processedPath,
        QWidget *parent = Q_NULLPTR,
        Qt::WindowFlags f = Qt::WindowFlags());

    void drawText(engine::Rect rect, const std::string& str);
protected:
    void paintEvent(QPaintEvent *);
private:
    std::unique_ptr<QHBoxLayout>            m_layout;
    std::unique_ptr<QLabel>                 m_label;
    std::unique_ptr<QLineEdit>              m_lineEdit;
    std::unique_ptr<engine::DragableLabel>  m_dragable;

    void updateProperty();

    int ValueHeight = 15;
    engine::PluginProperty m_typeProperty;
    std::shared_ptr<engine::PluginDrawer> m_drawer;

private slots:
    void onEditFinished();
};

#if 0
class PropertyWidget : public QWidget
{
    Q_OBJECT
public:
    PropertyWidget(
        std::shared_ptr<engine::Component> component,
        std::string propertyName,
        QWidget *parent = Q_NULLPTR,
        Qt::WindowFlags f = Qt::WindowFlags());
protected:
    void paintEvent(QPaintEvent *);
private:
    int ValueHeight = 15;
    std::string m_propertyName;
};

class FloatValueWidget : public QWidget
{
    Q_OBJECT
public:
    FloatValueWidget(
        QString valueName,
        float value,
        QWidget *parent = Q_NULLPTR,
        Qt::WindowFlags f = Qt::WindowFlags());
protected:
    void paintEvent(QPaintEvent *);
private:
    QString m_valueName;
    float m_value;
};

class Vector3fPropertyWidget : public QWidget
{
    Q_OBJECT
public:
    Vector3fPropertyWidget(
        std::shared_ptr<engine::Component> component,
        std::string propertyName,
        QWidget *parent = Q_NULLPTR,
        Qt::WindowFlags f = Qt::WindowFlags());
protected:
    void paintEvent(QPaintEvent *);
private:
    int ValueHeight = 15;
    std::shared_ptr<engine::Component> m_component;
    std::string m_propertyName;
    std::unique_ptr<QHBoxLayout> m_layout;
};
#endif

class ComponentWidget : public QWidget
{
    Q_OBJECT
public:
    ComponentWidget(
        std::shared_ptr<engine::EngineComponent> component,
		const QString& contentPath,
		const QString& processedPath,
        QWidget *parent = Q_NULLPTR,
        Qt::WindowFlags f = Qt::WindowFlags());

    ComponentWidget(
        engine::TypeInstance& component,
		const QString& contentPath,
		const QString& processedPath,
        QWidget *parent = Q_NULLPTR,
        Qt::WindowFlags f = Qt::WindowFlags());

    ~ComponentWidget();

    void populate();
    void clear();

signals:
    void refreshProperties();

private slots:
    void onRefreshProperties();

protected:
    void paintEvent(QPaintEvent *);
private:
    std::unique_ptr<QVBoxLayout> m_layout;
    std::shared_ptr<engine::EngineComponent> m_component;
    std::shared_ptr<engine::TypeInstance> m_componentInstance;
    std::unique_ptr<QLabel> m_componentLabel;
    QString m_contentPath;
    QString m_processedPath;
    QList<std::shared_ptr<Drawer>> m_drawers;
};

class Inspector : public QDockWidget
{
    Q_OBJECT
public:
    explicit Inspector(
        const Settings& settings,
        QMainWindow* mainWindow,
        QWidget *parent = Q_NULLPTR,
        Qt::WindowFlags flags = Qt::WindowFlags());
    ~Inspector();

public slots:
    void ShowContextMenu(const QPoint& point);
    void nodeSelected(std::shared_ptr<engine::SceneNode> node);
	void fileSelected(const QString& filePath);

signals:
    void processDroppedItem(const QString& sourceFilePath, const QString& targetPath);

private:
    QString m_contentPath;
	QString m_processedAssetsPathAbsolute;
    QMainWindow* m_mainWindow;
    //std::unique_ptr<QScrollArea> m_scrollArea;
    std::unique_ptr<QWidget> m_mainWidget;
    std::unique_ptr<QVBoxLayout> m_layout;
    QList<std::shared_ptr<ComponentWidget>> m_components;

    std::shared_ptr<engine::SceneNode> m_selectedNode;
};
