#pragma once

#include <QDockWidget>
#include <QSplitter>
#include <QTreeView>
#include <QListView>
#include <QFileSystemModel>
#include <QHBoxLayout>
#include <QDialog>
#include <QLabel>
#include <QTextEdit>
#include <QComboBox>
#include <memory>
#include "../settings/Settings.h"
#include "HierarchyTreeModel.h"
#include "engine/Engine.h"
#include "plugins/PluginManager.h"
#include "tools/Property.h"
#include "engine/primitives/Vector3.h"
#include "engine/primitives/Quaternion.h"

class ImportSettings : public QDialog
{
    Q_OBJECT
public:
    explicit ImportSettings(
        QList<QPair<QString, QString>>& images,
        QList<QPair<QString, QString>>& models,
        QWidget *parent = Q_NULLPTR,
        Qt::WindowFlags flags = Qt::WindowFlags());

    engine::Vector3f scale() const
    {
        return m_modelScaleProperty.value<engine::Vector3f>();
    }

    engine::Quaternionf rotation() const
    {
        return m_modelRotationProperty.value<engine::Quaternionf>();
    }

    std::string preferredEncoding() const
    {
        return m_preferredEncodingProperty.value<std::string>();
    }
private:
    std::unique_ptr<QVBoxLayout> m_layout;
    
    engine::Property m_modelScaleProperty;
    engine::Property m_modelRotationProperty;

    engine::Property m_preferredEncodingProperty;

    engine::Property m_okButton;

    std::vector<std::shared_ptr<Drawer>> m_drawers;
};
