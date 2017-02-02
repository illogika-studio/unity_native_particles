#include "renderer_opengl.h"
#include "globals.h"

#include <windows.h>
#include <IUnityInterface.h>
#include <IUnityGraphics.h>
#include <stdio.h>
#include <stdint.h>

#define FP(...) extern "C" __VA_ARGS__ UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API

static void UNITY_INTERFACE_API on_graphics_device_event(UnityGfxDeviceEventType event_type);

Renderer* renderer = nullptr;
int particle_qty = 1000;
bool disable_flx = false;
float time = 0.f;
float delta_time = 0.f;

FILE* stream;

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

	/**
	 * Run OnGraphicsDeviceEvent(initialize) manually on plugin load
	 * to not miss the event in case the graphics device is already initialized
	 */
	on_graphics_device_event(kUnityGfxDeviceEventInitialize);

	AllocConsole();
	freopen_s(&stream, "CONOUT$", "w", stdout);

	if (!(renderer = new Renderer(static_cast<GLsizei>(particle_qty)))) {
		OUTPUT_ERROR("Problem constructing renderer. FlexiParts disabled until reload.\n");
		disable_flx = true;
	}
}

/* Destroy */
FP(void) UnityPluginUnload() {
	if (renderer) {
		delete renderer;
		renderer = nullptr;
	}

	FreeConsole();

	unity.graphics->UnregisterDeviceEventCallback(on_graphics_device_event);
}

#if UNITY_WEBGL
typedef void	(UNITY_INTERFACE_API * PluginLoadFunc)(IUnityInterfaces* unityInterfaces);
typedef void	(UNITY_INTERFACE_API * PluginUnloadFunc)();
extern "C" void	UnityRegisterRenderingPlugin(PluginLoadFunc loadPlugin, PluginUnloadFunc unloadPlugin);

FP(void) RegisterPlugin() {
	UnityRegisterRenderingPlugin(unity_plugin_load, unity_plugin_unload);
}
#endif

static void UNITY_INTERFACE_API on_graphics_device_event(UnityGfxDeviceEventType event_type) {
	if (disable_flx)
		return;

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
			//TODO: user shutdown code
		} break;
		case kUnityGfxDeviceEventBeforeReset:
		{
			//TODO: user Direct3D 9 code
		} break;
		case kUnityGfxDeviceEventAfterReset:
		{
			//TODO: user Direct3D 9 code
		} break;
	};
}

static void UNITY_INTERFACE_API on_render_event(int eventId) {
	if (disable_flx)
		return;

	renderer->render(delta_time);
}

extern "C" UnityRenderingEvent UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API flx_get_render_event_func() {
	return on_render_event;
}

extern "C" const UNITY_INTERFACE_EXPORT char* UNITY_INTERFACE_API flx_get_test_msg() {
	return "FlexiParts message woot\n";
}

FP(void) flx_print_msg() {
	OUTPUT_ERROR("FlexiParts native message?\n");
}

FP(void) flx_set_time(float t) {
	time = t;
}

FP(void) flx_set_delta_time(float t) {
	delta_time = t;
}

FP(void) flx_initialize(int32_t particle_qty) {
	if (renderer != nullptr) {
		OUTPUT_ERROR("Changing particle quantity after initialization is not yet supported.\n");
	}

}

FP(void) flx_set_mvp(GLfloat model[16], GLfloat view[16], GLfloat projection[16]) {
	if (renderer == nullptr)
		return;

	renderer->mvp(model, view, projection);
	//for (int i = 0; i < 16; ++i) {
	//	printf("%f ", model[i]);
	//} printf("\n");
	//for (int i = 0; i < 16; ++i) {
	//	printf("%f ", view[i]);
	//} printf("\n");
	//for (int i = 0; i < 16; ++i) {
	//	printf("%f ", projection[i]);
	//} printf("\n");
}

