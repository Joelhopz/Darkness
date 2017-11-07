#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>

#include "tools/Property.h"
#include "engine/Drawer.h"
#include "DragableLabel.h"
#include "engine/primitives/Vector3.h"

namespace engine
{
    class IntWidget : public QWidget
    {
        Q_OBJECT
    public:
        IntWidget(Property& value, QWidget *parent = Q_NULLPTR,
            Qt::WindowFlags f = Qt::WindowFlags());
        virtual ~IntWidget();
    private:
        Property& m_value;
        bool m_valueValid;
        bool m_canReceiveUpdate;
        std::unique_ptr<QHBoxLayout> m_layout;

        std::unique_ptr<QLabel> m_nameLabel;
        std::unique_ptr<QLineEdit> m_xLine;
        std::unique_ptr<DragableLabel> m_xLabel;

        void updateProperty();

    private slots:
        void onEditFinished();
    };

    class IntDrawer : public Drawer
    {
    public:
        IntDrawer(Property& value);
        void setParent(void* parent) override;
        void* native() override;

    private:
        Property& m_value;
        std::shared_ptr<QWidget> m_widget;
        QWidget* m_parent;
    };

    template <>
    std::shared_ptr<Drawer> createDrawer<int>(Property& value);

}
