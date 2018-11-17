#include "SimpleApp.h"
#include "Debug/NeptuneDebug.h"
#include "Graphics/View.h"
#include "System/Type/Integers.h"
#include "Input/EventSystemInterface.h"
#include "Input/InputProducer.h"
#include "Input/InputProducerFactory.h"
#include <functional>

using namespace Neptune;


// TOD: MUST BE MOVED SOMEWHERE ELSE
#include "Math/Vectors/Mat4x4.h"
#include "Math/Vectors/Vec4.h"
#include "Math/Vectors/MatrixTransform.h"
#include "Math/Math.h"

static void CameraControllerCallback(const Input& _input, Camera& _camera)
{
	const SDL_Event& e = static_cast<SDL_Event>(_input);

	const float DEFAULT_ANGLE = 1.0f;
	const float DEFAULT_STEP = 0.1f;

	const Mat4& orientation_matrix = Transpose(_camera.getOrientation());
	Vec4 step(0.0f, 0.0f, 0.0f, 0.0f);
	float step_distance = DEFAULT_STEP;


	switch (e.key.keysym.scancode)
	{
		// Change acceleration
	case SDL_SCANCODE_Z:
		step_distance += 0.1f;
		break;

	case SDL_SCANCODE_X:
		step_distance = Max(0.1f, step_distance - 0.1f);
		break;

		// Change position
	case SDL_SCANCODE_W:
		step.z() = step_distance;
		step = orientation_matrix * step;
		_camera.translate(step.x(), step.y(), step.z());
		break;

	case SDL_SCANCODE_S:
		step.z() = -step_distance;
		step = orientation_matrix * step;
		_camera.translate(step.x(), step.y(), step.z());
		break;

	case SDL_SCANCODE_A:
		step.x() = -step_distance;
		step = orientation_matrix * step;
		_camera.translate(step.x(), step.y(), step.z());
		break;

	case SDL_SCANCODE_D:
		step.x() = step_distance;
		step = orientation_matrix * step;
		_camera.translate(step.x(), step.y(), step.z());
		break;

	case SDL_SCANCODE_Q:
		step.y() = -step_distance;
		step = orientation_matrix * step;
		_camera.translate(step.x(), step.y(), step.z());
		break;

	case SDL_SCANCODE_E:
		step.y() = step_distance;
		step = orientation_matrix * step;
		_camera.translate(step.x(), step.y(), step.z());
		break;


		// Change orientation
	case SDL_SCANCODE_UP:
		_camera.rotate(DEFAULT_ANGLE, Vec3(1.0f, 0.0f, 0.0f));
		break;

	case SDL_SCANCODE_DOWN:
		_camera.rotate(-DEFAULT_ANGLE, Vec3(1.0f, 0.0f, 0.0f));
		break;

	case SDL_SCANCODE_LEFT:
		_camera.rotate(-DEFAULT_ANGLE, Vec3(0.0f, 1.0f, 0.0f));
		break;

	case SDL_SCANCODE_RIGHT:
		_camera.rotate(DEFAULT_ANGLE, Vec3(0.0f, 1.0f, 0.0f));
		break;
	}
}
/////////////////////////////////////////////////////////////////////////////


SimpleApp::SimpleApp(u32 _windowWidth, u32 _windowHeight, const char* _appName, bool _fullScreen /* = false */):
	m_is_update_enabled(true),
	m_onFrameStartCallBack([](FrameData){return true; }),
	m_onFrameEndCallBack([](FrameData){return true; }),
	m_onViewUpdateCallBack([](View*){return true; }),
	m_inputProducer(InputProducerFactory::CreateDefaultProducer())
{
	// Set app's name
	const char* app_name = (_appName != nullptr) ? _appName : "Simple App";

	// Set up the graphical context
	auto gcontext = DisplayDeviceInterface::OffScreenRenderingSettings();
	gcontext.m_antiAliasing = DisplayDeviceInterface::MULTI_SAMPLE_ANTI_ALLIASING::X16;
	gcontext.m_frameBufferWidth = _windowWidth;
	gcontext.m_frameBufferHeight = _windowHeight;
	gcontext.m_enableReversedZ = true;

	// GRAPHICAL SYSTEM INIT
	// Init viewport
	m_window = DisplayDeviceInterface::CreateWindow(app_name,
													_windowWidth,
													_windowHeight,
													DisplayDeviceInterface::MULTI_SAMPLE_ANTI_ALLIASING::X16,
													_fullScreen);
	m_context = DisplayDeviceInterface::CreateGraphicalContext(m_window, 3, 4, gcontext);

	// Set the right projection matrix
	m_camera.updateProjection(Camera::ProjectionType::REVERSED_Z_PERSPECTIVE);
	m_camera.setScreenRatio(static_cast<float>(_windowWidth) / _windowHeight);

	// set background color
	m_backgroundColor.r = 0.0f;
	m_backgroundColor.g = 0.0f;
	m_backgroundColor.b = 0.0f;
	m_backgroundColor.a = 0.0f;

	// INPUT SYSTEM INIT
	// Set up control callbacks
	// Bind the input system to the camera
	std::function<void(const Input&)> camera_controller = [&](const Input& _input) {
		CameraControllerCallback(_input, m_camera);
	};

	// Init input system
	EventSystemInterface::StartUp();

	// Subscribe to input streams
	m_inputConsumer.subscribe(m_inputProducer, InputType::KEYBOARD_PUSH, camera_controller);
}

void SimpleApp::loop()
{
	float background[4];
	
	while (m_is_update_enabled)
	{
		// Clear screen
		background[0] = m_backgroundColor.r;
		background[1] = m_backgroundColor.g;
		background[2] = m_backgroundColor.b;
		background[3] = m_backgroundColor.a;
		DisplayDeviceInterface::ClearBuffers(background);

		onFrameStart();

		// Update controls
		m_inputProducer->update();
		m_inputConsumer.update();

		// Draw views
		for (auto& v : m_views)
			onViewUpdate(v);

		onFrameEnd();

		// Display draw result
		DisplayDeviceInterface::SwapBuffer(m_window);
	}
}

SimpleApp::~SimpleApp()
{
	// Delete views
	for (auto& v : m_views){
		v->terminate();
		delete v;
	}
	m_views.clear();

	// Delete input managers
	InputProducerFactory::DestructDefaultProducer(m_inputProducer);
	EventSystemInterface::ShutDown();

	// Delete viewport
	DisplayDeviceInterface::DestroyGraphicalContext(m_context);
	DisplayDeviceInterface::DestroyWindow(m_window);

	m_context = DisplayDeviceInterface::INVALID_HANDLE;
	m_window = DisplayDeviceInterface::INVALID_HANDLE;
}

void SimpleApp::add(View* _view)
{
	NEP_ASSERT(_view != nullptr);
	if (_view != nullptr)
	{
		m_views.push_back(_view);
		_view->bindToCamera(&m_camera);
	}
}

// Must change how views are bound to cameras
/*void SimpleApp::addViewList(View** _list, u32 _length)
{
	NEP_ASSERT(_list != nullptr && _length > 0);
	if (_list != nullptr && _length > 0)
		m_views.insert(m_views.end(), _list, _list + (_length - 1)*sizeof(*_list));
}*/

bool SimpleApp::onFrameStart()
{
	FrameData frame_data;
	frame_data.m_camera = &m_camera;
	frame_data.m_views  = &m_views;

	return m_onFrameStartCallBack(frame_data);
}

bool SimpleApp::onFrameEnd()
{
	FrameData frame_data;
	frame_data.m_camera = &m_camera;
	frame_data.m_views  = &m_views;

	return m_onFrameEndCallBack(frame_data);
}

bool SimpleApp::onViewUpdate(View* _view)
{
	bool result = m_onViewUpdateCallBack(_view);
	return result && _view->update();
}