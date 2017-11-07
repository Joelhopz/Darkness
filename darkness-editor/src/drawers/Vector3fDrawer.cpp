#include "Vector3fDrawer.h"
#include <QStyleOption>
#include <QPainter>
#include <QMouseEvent>
#include "engine/primitives/Vector3.h"

namespace engine
{
    Vector3fWidget::Vector3fWidget(
        Property& value,
        QWidget *parent,
        Qt::WindowFlags f)
        : QWidget(parent, f)
        , m_value{ value }
        , m_layout { std::make_unique<QHBoxLayout>() }
        , m_nameLabel{ std::make_unique<QLabel>(this) }
        , m_xLine{ std::make_unique<QLineEdit>(this) }
        , m_yLine{ std::make_unique<QLineEdit>(this) }
        , m_zLine{ std::make_unique<QLineEdit>(this) }
        , m_xLabel{ std::make_unique<DragableLabel>(*m_xLine.get(), this) }
        , m_yLabel{ std::make_unique<DragableLabel>(*m_yLine.get(), this) }
        , m_zLabel{ std::make_unique<DragableLabel>(*m_zLine.get(), this) }
        , m_valueValid{ true }
    {
        setLayout(m_layout.get());
        m_layout->setSpacing(0);
        m_layout->setMargin(0);
        m_layout->setContentsMargins(QMargins(2, 0, 2, 0));

        m_xLabel->setMaximumWidth(10);
        m_yLabel->setMaximumWidth(10);
        m_zLabel->setMaximumWidth(10);

        m_xLine->setMaximumWidth(60);
        m_yLine->setMaximumWidth(60);
        m_zLine->setMaximumWidth(60);

        m_nameLabel->sizePolicy().setHorizontalStretch(QSizePolicy::Expanding);
        // grabMouse
        // releaseMouse
        m_layout->addWidget(m_nameLabel.get());
        m_layout->addWidget(m_xLabel.get());
        m_layout->addWidget(m_xLine.get());
        m_layout->addWidget(m_yLabel.get());
        m_layout->addWidget(m_yLine.get());
        m_layout->addWidget(m_zLabel.get());
        m_layout->addWidget(m_zLine.get());

        m_nameLabel->setText(value.name().data());
        m_xLabel->setText(" X ");
        m_yLabel->setText(" Y ");
        m_zLabel->setText(" Z ");

        m_xLine->setText(QString::number(value.value<Vector3f>().x));
        m_yLine->setText(QString::number(value.value<Vector3f>().y));
        m_zLine->setText(QString::number(value.value<Vector3f>().z));

        QObject::connect(
            m_xLine.get(), SIGNAL(editingFinished()),
            this, SLOT(onXEditFinished()));

        QObject::connect(
            m_yLine.get(), SIGNAL(editingFinished()),
            this, SLOT(onYEditFinished()));

        QObject::connect(
            m_zLine.get(), SIGNAL(editingFinished()),
            this, SLOT(onZEditFinished()));

        value.registerForChangeNotification(this, [this]() 
        {
            if (m_canReceiveUpdate)
            {
                m_xLine->setText(QString::number(m_value.value<Vector3f>().x));
                m_yLine->setText(QString::number(m_value.value<Vector3f>().y));
                m_zLine->setText(QString::number(m_value.value<Vector3f>().z));
            }
        });
        value.registerForRemovalNotification(this, [this]() { this->m_valueValid = false; });
    }

    Vector3fWidget::~Vector3fWidget()
    {
        if(m_valueValid)
            m_value.unregisterForChangeNotification(this);
        
    }

    void Vector3fWidget::updateProperty()
    {
        m_canReceiveUpdate = false;
        m_value.value<Vector3f>(Vector3f{
            m_xLine->text().toFloat(),
            m_yLine->text().toFloat(),
            m_zLine->text().toFloat() });
        m_canReceiveUpdate = true;
    }

    void Vector3fWidget::onXEditFinished()
    {
        updateProperty();
    }

    void Vector3fWidget::onYEditFinished()
    {
        updateProperty();
    }

    void Vector3fWidget::onZEditFinished()
    {
        updateProperty();
    }

    Vector3fDrawer::Vector3fDrawer(Property& value)
        : m_value{ value }
        , m_widget{ nullptr }
    {
    }

    void Vector3fDrawer::setParent(void* parent)
    {
        if (parent)
        {
            m_parent = reinterpret_cast<QWidget*>(parent);

            m_widget = std::make_shared<Vector3fWidget>(m_value, m_parent);
            m_widget->setParent(m_parent);
            m_widget->resize(m_parent->width(), 15);
            m_widget->setMaximumHeight(15);
            m_widget->setMinimumWidth(15);
        }
        else
        {
            if (m_widget)
            {
                m_widget->setParent(nullptr);
                m_widget = nullptr;
            }
            m_parent = nullptr;
        }
    }

    void* Vector3fDrawer::native()
    {
        return m_widget.get();
    }

    template <>
    std::shared_ptr<Drawer> createDrawer<Vector3f>(Property& value)
    {
        return std::make_shared<Vector3fDrawer>(value);
    }
}
