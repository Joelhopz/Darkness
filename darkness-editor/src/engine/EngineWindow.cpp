#include "EngineWindow.h"
#include <QTimer>
#include <QMouseEvent>
#include <QStyle>

EngineClientWindow::EngineClientWindow(QWidget* parent)
	: QWidget(parent)
{
	setMouseTracking(true);
	setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

QPoint EngineClientWindow::titleBarHeightAndMargin() const
{
	int titleBarMargin = style()->pixelMetric(QStyle::PM_DockWidgetTitleMargin);
	int titleBarHeight = style()->pixelMetric(QStyle::PM_TitleBarHeight);
	return { titleBarMargin, titleBarHeight };
}

void EngineClientWindow::mouseDoubleClickEvent(QMouseEvent* mouseEvent)
{
	auto border = titleBarHeightAndMargin();
	border.setX(0);
	border.setY(0);
	switch (mouseEvent->button())
	{
	case Qt::MouseButton::LeftButton: { emit onMouseDoubleClick(engine::MouseButton::Left, mouseEvent->pos().x() - border.x(), mouseEvent->pos().y() - border.y()); break; }
	case Qt::MouseButton::RightButton: { emit onMouseDoubleClick(engine::MouseButton::Right, mouseEvent->pos().x() - border.x(), mouseEvent->pos().y() - border.y()); break; }
	case Qt::MouseButton::MiddleButton: { emit onMouseDoubleClick(engine::MouseButton::Center, mouseEvent->pos().x() - border.x(), mouseEvent->pos().y() - border.y()); break; }
	default: break;
	}
}

void EngineClientWindow::mouseMoveEvent(QMouseEvent* mouseEvent)
{
	emit onMouseMove(mouseEvent->pos().x(), mouseEvent->pos().y());
}

void EngineClientWindow::mousePressEvent(QMouseEvent* mouseEvent)
{
	auto border = titleBarHeightAndMargin();
	border.setX(0);
	border.setY(0);
	switch (mouseEvent->button())
	{
	case Qt::MouseButton::LeftButton: { emit onMouseDown(engine::MouseButton::Left, mouseEvent->pos().x() - border.x(), mouseEvent->pos().y() - border.y()); break; }
	case Qt::MouseButton::RightButton: { emit onMouseDown(engine::MouseButton::Right, mouseEvent->pos().x() - border.x(), mouseEvent->pos().y() - border.y()); break; }
	case Qt::MouseButton::MiddleButton: { emit onMouseDown(engine::MouseButton::Center, mouseEvent->pos().x() - border.x(), mouseEvent->pos().y() - border.y()); break; }
	default: break;
	}
}

void EngineClientWindow::mouseReleaseEvent(QMouseEvent* mouseEvent)
{
	auto border = titleBarHeightAndMargin();
	border.setX(0);
	border.setY(0);
	switch (mouseEvent->button())
	{
	case Qt::MouseButton::LeftButton: { emit onMouseUp(engine::MouseButton::Left, mouseEvent->pos().x() - border.x(), mouseEvent->pos().y() - border.y()); break; }
	case Qt::MouseButton::RightButton: { emit onMouseUp(engine::MouseButton::Right, mouseEvent->pos().x() - border.x(), mouseEvent->pos().y() - border.y()); break; }
	case Qt::MouseButton::MiddleButton: { emit onMouseUp(engine::MouseButton::Center, mouseEvent->pos().x() - border.x(), mouseEvent->pos().y() - border.y()); break; }
	default: break;
	}
}

engine::Key qtKeyToEngineKey(Qt::Key key)
{
	switch (key)
	{
	case Qt::Key_0: return engine::Key::Num0;
	case Qt::Key_1: return engine::Key::Num1;
	case Qt::Key_2: return engine::Key::Num2;
	case Qt::Key_3: return engine::Key::Num3;
	case Qt::Key_4: return engine::Key::Num4;
	case Qt::Key_5: return engine::Key::Num5;
	case Qt::Key_6: return engine::Key::Num6;
	case Qt::Key_7: return engine::Key::Num7;
	case Qt::Key_8: return engine::Key::Num8;
	case Qt::Key_9: return engine::Key::Num9;
					
	case Qt::Key_A: return engine::Key::A;
	case Qt::Key_B: return engine::Key::B;
	case Qt::Key_C: return engine::Key::C;
	case Qt::Key_D: return engine::Key::D;
	case Qt::Key_E: return engine::Key::E;
	case Qt::Key_F: return engine::Key::F;
	case Qt::Key_G: return engine::Key::G;
	case Qt::Key_H: return engine::Key::H;
	case Qt::Key_I: return engine::Key::I;
	case Qt::Key_J: return engine::Key::J;
	case Qt::Key_K: return engine::Key::K;
	case Qt::Key_L: return engine::Key::L;
	case Qt::Key_M: return engine::Key::M;
	case Qt::Key_N: return engine::Key::N;
	case Qt::Key_O: return engine::Key::O;
	case Qt::Key_P: return engine::Key::P;
	case Qt::Key_Q: return engine::Key::Q;
	case Qt::Key_R: return engine::Key::R;
	case Qt::Key_S: return engine::Key::S;
	case Qt::Key_T: return engine::Key::T;
	case Qt::Key_U: return engine::Key::U;
	case Qt::Key_V: return engine::Key::V;
	case Qt::Key_W: return engine::Key::W;
	case Qt::Key_X: return engine::Key::X;
	case Qt::Key_Y: return engine::Key::Y;
	case Qt::Key_Z: return engine::Key::Z;

	case Qt::Key_F1: return engine::Key::F1;
	case Qt::Key_F2: return engine::Key::F2;
	case Qt::Key_F3: return engine::Key::F3;
	case Qt::Key_F4: return engine::Key::F4;
	case Qt::Key_F5: return engine::Key::F5;
	case Qt::Key_F6: return engine::Key::F6;
	case Qt::Key_F7: return engine::Key::F7;
	case Qt::Key_F8: return engine::Key::F8;
	case Qt::Key_F9: return engine::Key::F9;
	case Qt::Key_F10: return engine::Key::F10;
	case Qt::Key_F11: return engine::Key::F11;
	case Qt::Key_F12: return engine::Key::F12;

	case Qt::Key_Print: return engine::Key::PrintScreen;
	case Qt::Key_ScrollLock: return engine::Key::ScrollLock;
	case Qt::Key_Pause: return engine::Key::Pause;

	case Qt::Key_Left: return engine::Key::ArrowLeft;
	case Qt::Key_Right: return engine::Key::ArrowRight;
	case Qt::Key_Up: return engine::Key::ArrowUp;
	case Qt::Key_Down: return engine::Key::ArrowDown;
	default: return engine::Key::Unknown;
	}
}

#define EXTENDED_KEY_MASK   0x01000000

quint32 interpretKeyEvent(QKeyEvent* e)
{
	quint32 vk = e->nativeVirtualKey();
	
	quint32 mods = e->nativeModifiers();
	bool extended = (bool)(mods & EXTENDED_KEY_MASK);

	quint32 scancode = e->nativeScanCode();

	switch (vk)
	{
	case VK_CONTROL:
		vk = extended ? VK_RCONTROL : VK_LCONTROL;
		break;
	case VK_MENU:
		// VK_MENU = ALT virtual key
		vk = extended ? VK_RMENU : VK_LMENU;
		break;
	case VK_SHIFT:
		vk = MapVirtualKey(scancode, MAPVK_VSC_TO_VK_EX);
		break;
	default:
		break;
	};
	return vk;
}


void EngineClientWindow::keyPressEvent(QKeyEvent* keyEvent)
{
	auto engineKey = qtKeyToEngineKey(static_cast<Qt::Key>(keyEvent->key()));
#ifdef _WIN32
	if (engineKey == engine::Key::Unknown)
	{
		auto nativeMods = interpretKeyEvent(keyEvent);

		if ((nativeMods & VK_LSHIFT) == VK_LSHIFT)
			m_modState[engine::KeyModifier::ShiftLeft] = true;

		if ((nativeMods & VK_RSHIFT) == VK_RSHIFT)
			m_modState[engine::KeyModifier::ShiftRight] = true;

		if ((nativeMods & VK_LMENU) == VK_LMENU)
			m_modState[engine::KeyModifier::AltLeft] = true;

		if ((nativeMods & VK_RMENU) == VK_RMENU)
			m_modState[engine::KeyModifier::AltRight] = true;

		if ((nativeMods & VK_LCONTROL) == VK_LCONTROL)
			m_modState[engine::KeyModifier::CtrlLeft] = true;

		if ((nativeMods & VK_RCONTROL) == VK_RCONTROL)
			m_modState[engine::KeyModifier::CtrlRight] = true;
	}
#else
	auto mods = keyEvent->modifiers();
	modState[engine::KeyModifier::ShiftLeft] = (mods & Qt::ShiftModifier) == Qt::ShiftModifier;
	modState[engine::KeyModifier::ShiftRight] = modState[engine::KeyModifier::ShiftLeft];

	modState[engine::KeyModifier::AltLeft] = (mods & Qt::AltModifier) == Qt::AltModifier;
	modState[engine::KeyModifier::AltRight] = modState[engine::KeyModifier::AltLeft];

	modState[engine::KeyModifier::CtrlLeft] = (mods & Qt::ControlModifier) == Qt::ControlModifier;
	modState[engine::KeyModifier::CtrlRight] = modState[engine::KeyModifier::CtrlLeft];
#endif
	emit onKeyDown(engineKey, m_modState);
}

void EngineClientWindow::keyReleaseEvent(QKeyEvent* keyEvent)
{
	auto engineKey = qtKeyToEngineKey(static_cast<Qt::Key>(keyEvent->key()));
#ifdef _WIN32
	if (engineKey == engine::Key::Unknown)
	{
		auto nativeMods = interpretKeyEvent(keyEvent);

		if ((nativeMods & VK_LSHIFT) == VK_LSHIFT)
			m_modState[engine::KeyModifier::ShiftLeft] = false;

		if ((nativeMods & VK_RSHIFT) == VK_RSHIFT)
			m_modState[engine::KeyModifier::ShiftRight] = false;

		if ((nativeMods & VK_LMENU) == VK_LMENU)
			m_modState[engine::KeyModifier::AltLeft] = false;

		if ((nativeMods & VK_RMENU) == VK_RMENU)
			m_modState[engine::KeyModifier::AltRight] = false;

		if ((nativeMods & VK_LCONTROL) == VK_LCONTROL)
			m_modState[engine::KeyModifier::CtrlLeft] = false;

		if ((nativeMods & VK_RCONTROL) == VK_RCONTROL)
			m_modState[engine::KeyModifier::CtrlRight] = false;
	}
#else
	auto mods = keyEvent->modifiers();
	modState[engine::KeyModifier::ShiftLeft] = (mods & Qt::ShiftModifier) == Qt::ShiftModifier;
	modState[engine::KeyModifier::ShiftRight] = modState[engine::KeyModifier::ShiftLeft];

	modState[engine::KeyModifier::AltLeft] = (mods & Qt::AltModifier) == Qt::AltModifier;
	modState[engine::KeyModifier::AltRight] = modState[engine::KeyModifier::AltLeft];

	modState[engine::KeyModifier::CtrlLeft] = (mods & Qt::ControlModifier) == Qt::ControlModifier;
	modState[engine::KeyModifier::CtrlRight] = modState[engine::KeyModifier::CtrlLeft];
#endif
	emit onKeyUp(engineKey, m_modState);
}


EngineWindow::EngineWindow(QWidget* parent, Qt::WindowFlags flags, const QString& shaderRootPath)
    : QDockWidget(parent, flags)
    , m_lastSize{ this->width(), this->height() }
    , m_mainWidget{ new EngineClientWindow(this) }
	, m_mouseLeft{ false }
	, m_mouseRight{ false }
	, m_mouseCenter{ false }
{
    setWindowTitle("Scene");
    setObjectName("Scene");
	
	QObject::connect(
		m_mainWidget.get(), SIGNAL(onMouseMove(int, int)),
		this, SLOT(onMouseMove(int, int)));
	QObject::connect(
		m_mainWidget.get(), SIGNAL(onMouseDown(engine::MouseButton, int, int)),
		this, SLOT(onMouseDown(engine::MouseButton, int, int)));
	QObject::connect(
		m_mainWidget.get(), SIGNAL(onMouseUp(engine::MouseButton, int, int)),
		this, SLOT(onMouseUp(engine::MouseButton, int, int)));
	QObject::connect(
		m_mainWidget.get(), SIGNAL(onMouseDoubleClick(engine::MouseButton, int, int)),
		this, SLOT(onMouseDoubleClick(engine::MouseButton, int, int)));

	QObject::connect(
		m_mainWidget.get(), SIGNAL(onKeyDown(engine::Key, const engine::ModifierState&)),
		this, SLOT(onKeyDown(engine::Key, const engine::ModifierState&)));
	QObject::connect(
		m_mainWidget.get(), SIGNAL(onKeyUp(engine::Key, const engine::ModifierState&)),
		this, SLOT(onKeyUp(engine::Key, const engine::ModifierState&)));

    m_engineWrap = std::make_shared<EngineWrap>(
        m_mainWidget->winId(), this->width(), this->height(), shaderRootPath);

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(ping()));
    m_timer->start(1000.0f / 600.0f);

    setWidget(m_mainWidget.get());
}

Engine& EngineWindow::engine()
{
    return m_engineWrap->engine();
}

void EngineWindow::resetCameraSize()
{
    m_engineWrap->resetCameraSize();
}

void EngineWindow::playClicked(bool value)
{
    m_engineWrap->playClicked(value);
}

EngineWindow::~EngineWindow()
{
    m_timer->stop();
}

void EngineWindow::ping()
{
    QSize size = QSize(this->width(), this->height());
    if (m_lastSize != size)
    {
        m_lastSize = size;
        m_engineWrap->refresh();
    }
    m_engineWrap->update();
}

void EngineWindow::onMouseMove(int x, int y)
{
	m_engineWrap->onMouseMove(x, y);
}

void EngineWindow::onMouseDown(engine::MouseButton button, int x, int y)
{
	m_engineWrap->onMouseDown(button, x, y);
	
	if (button == engine::MouseButton::Left)
		m_mouseLeft = true;
	if (button == engine::MouseButton::Center)
		m_mouseCenter = true;
	if (button == engine::MouseButton::Right)
		m_mouseRight = true;

	emit mouseGrabbed(true);
    m_engineWrap->engine().cameraInputActive(true);
}

void EngineWindow::onMouseUp(engine::MouseButton button, int x, int y)
{
	m_engineWrap->onMouseUp(button, x, y);

	if (button == engine::MouseButton::Left)
		m_mouseLeft = false;
	if (button == engine::MouseButton::Center)
		m_mouseCenter = false;
	if (button == engine::MouseButton::Right)
		m_mouseRight = false;

    if (!m_mouseLeft && !m_mouseCenter && !m_mouseRight)
    {
        emit mouseGrabbed(false);
        m_engineWrap->engine().cameraInputActive(false);
    }
}

void EngineWindow::onMouseDoubleClick(engine::MouseButton button, int x, int y)
{
	m_engineWrap->onMouseDoubleClick(button, x, y);
}

void EngineWindow::onKeyDown(engine::Key key, const engine::ModifierState& modifierState)
{
	m_engineWrap->onKeyDown(key, modifierState);
}

void EngineWindow::onKeyUp(engine::Key key, const engine::ModifierState& modifierState)
{
	m_engineWrap->onKeyUp(key, modifierState);
}

