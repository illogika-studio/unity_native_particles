#include "renderer_opengl.h"
#include "particle_component.h"
#include "globals.h"

#include <windows.h>
#include <IUnityInterface.h>
#include <IUnityGraphics.h>
#include <cstdint>

#define FP(...) extern "C" __VA_ARGS__ UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API

static void UNITY_INTERFACE_API on_graphics_device_event(UnityGfxDeviceEventType event_type);

Renderer renderer;
FILE* stream;
int particle_qty = 2'000'000;
float _delta_time = 0.f;
bool disable_flx = false;

struct UnityPointers {
	IUnityInterfaces* interfaces = nullptr;
	IUnityGraphics* graphics = nullptr;
	UnityGfxRenderer renderer_type = kUnityGfxRendererNull;
} unity;

/* Init */
FP(void) UnityPluginLoad(IUnityInterfaces* unity_interfaces) {
	unity.interfaces = unity_interfaces;

	unity.graphics = unity.interfaces->Get<IUnityGraphics>();
	unity.graphics->RegisterDeviceEventCallback(on_graphics_device_event);

	AllocConsole();
	SetConsoleTitle("Unity Native Particles");
	freopen_s(&stream, "CONOUT$", "w", stdout);

	ParticleComponent::init(particle_qty);

	/**
	 * Run OnGraphicsDeviceEvent(initialize) manually on plugin load
	 * to not miss the event in case the graphics device is already initialized
	 */
	on_graphics_device_event(kUnityGfxDeviceEventInitialize);
}

/* Destroy */
FP(void) UnityPluginUnload() {
	FreeConsole();

	unity.graphics->UnregisterDeviceEventCallback(on_graphics_device_event);
}

#if UNITY_WEBGL // UNTESTED
typedef void	(UNITY_INTERFACE_API * PluginLoadFunc)(IUnityInterfaces* unityInterfaces);
typedef void	(UNITY_INTERFACE_API * PluginUnloadFunc)();
extern "C" void	UnityRegisterRenderingPlugin(PluginLoadFunc loadPlugin, PluginUnloadFunc unloadPlugin);

FP(void) RegisterPlugin() {
	UnityRegisterRenderingPlugin(unity_plugin_load, unity_plugin_unload);
}
#endif

static void UNITY_INTERFACE_API on_graphics_device_event(UnityGfxDeviceEventType event_type) {
	switch (event_type) {
		case kUnityGfxDeviceEventInitialize:
		{
			unity.renderer_type = unity.graphics->GetRenderer();

			if (unity.renderer_type != kUnityGfxRendererOpenGLCore) {
				OUTPUT_ERROR("Prototype only works on OpenGL. FlexiParts disabled until reload.\n");
				disable_flx = true;
				return;
			}

		} break;
		case kUnityGfxDeviceEventShutdown:
		{
			unity.renderer_type = kUnityGfxRendererNull;

			ParticleComponent::destroy();

		} break;
		case kUnityGfxDeviceEventBeforeReset:
		{
			OUTPUT_MSG("Before Reset")
		} break;
		case kUnityGfxDeviceEventAfterReset:
		{
			OUTPUT_MSG("AfterReset")
		} break;
	};
}

static void UNITY_INTERFACE_API flx_render(int eventId) {
	if (disable_flx)
		return;

	renderer.render(_delta_time);
}

extern "C" UnityRenderingEvent UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API flx_get_render_event_func() {
	return flx_render;
}

FP(void) flx_update(float delta_time) {
	if (disable_flx)
		return;

	_delta_time = delta_time;
	ParticleComponent::update(delta_time);
};

FP(void) flx_init(int32_t particle_qty) {
	OUTPUT_ERROR("Changing particle quantity after initialization is not yet supported.\n");

}

FP(void) flx_set_mvp(GLfloat model[16], GLfloat view[16], GLfloat projection[16]) {
	renderer.mvp(model, view, projection);
}

