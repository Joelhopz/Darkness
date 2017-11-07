#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLabel>

#include "tools/Property.h"
#include "engine/Drawer.h"
#include "DragableLabel.h"
#include "engine/primitives/Vector3.h"
#include "components/Camera.h"

namespace engine
{
    class ProjectionWidget : public QWidget
    {
        Q_OBJECT
    public:
        ProjectionWidget(Property& value, QWidget *parent = Q_NULLPTR,
            Qt::WindowFlags f = Qt::WindowFlags());

        virtual ~ProjectionWidget();
    private:
        Property& m_value;
        bool m_valueValid;
        bool m_canReceiveUpdate;
        std::unique_ptr<QHBoxLayout> m_layout;

        std::unique_ptr<QLabel> m_nameLabel;
        std::unique_ptr<QComboBox> m_values;
        std::unique_ptr<QLabel> m_xLabel;

        void updateProperty();

    private slots:
        void onActivated(int index);
    };

    class ProjectionDrawer : public Drawer
    {
    public:
        ProjectionDrawer(Property& value);
        void setParent(void* parent) override;
        void* native() override;

    private:
        Property& m_value;
        std::shared_ptr<QWidget> m_widget;
        QWidget* m_parent;
    };

    template <>
    std::shared_ptr<Drawer> createDrawer<Projection>(Property& value);

}
