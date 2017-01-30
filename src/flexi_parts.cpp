#include <windows.h>
#include <IUnityInterface.h>
#include <IUnityGraphics.h>
#include <stdio.h>

#define FP(...) extern "C" __VA_ARGS__ UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API

static void UNITY_INTERFACE_API on_graphics_device_event(UnityGfxDeviceEventType event_type);

float time = 0.f;
float delta_time = 0.f;

struct UnityPointers {
	IUnityInterfaces* interfaces = nullptr;
	IUnityGraphics* graphics = nullptr;
	UnityGfxRenderer renderer_type = kUnityGfxRendererNull;
} unity;

static void UNITY_INTERFACE_API init()
{
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
}

static void UNITY_INTERFACE_API destroy()
{
	FreeConsole();
}

FP(void) UnityPluginLoad(IUnityInterfaces* unity_interfaces)
{
    unity.interfaces = unity_interfaces;

    unity.graphics = unity.interfaces->Get<IUnityGraphics>();
    unity.graphics->RegisterDeviceEventCallback(on_graphics_device_event);

    /**
	 * Run OnGraphicsDeviceEvent(initialize) manually on plugin load
	 * to not miss the event in case the graphics device is already initialized
	 */
    on_graphics_device_event(kUnityGfxDeviceEventInitialize);

	init();
}

FP(void) UnityPluginUnload()
{
	destroy();

    unity.graphics->UnregisterDeviceEventCallback(on_graphics_device_event);
}

#if UNITY_WEBGL
typedef void	(UNITY_INTERFACE_API * PluginLoadFunc)(IUnityInterfaces* unityInterfaces);
typedef void	(UNITY_INTERFACE_API * PluginUnloadFunc)();
extern "C" void	UnityRegisterRenderingPlugin(PluginLoadFunc loadPlugin, PluginUnloadFunc unloadPlugin);

FP(void) RegisterPlugin()
{
    UnityRegisterRenderingPlugin(unity_plugin_load, unity_plugin_unload);
}
#endif

static void UNITY_INTERFACE_API on_graphics_device_event(UnityGfxDeviceEventType event_type)
{
    switch (event_type)
    {
        case kUnityGfxDeviceEventInitialize:
        {
            unity.renderer_type = unity.graphics->GetRenderer();
            //TODO: user initialization code
			if (unity.renderer_type != kUnityGfxRendererOpenGLCore) {
				printf("Prototype only works on OpenGL.\n");
				exit(-1);
			}

            break;
        }
        case kUnityGfxDeviceEventShutdown:
        {
            unity.renderer_type = kUnityGfxRendererNull;
            //TODO: user shutdown code
            break;
        }
        case kUnityGfxDeviceEventBeforeReset:
        {
            //TODO: user Direct3D 9 code
            break;
        }
        case kUnityGfxDeviceEventAfterReset:
        {
            //TODO: user Direct3D 9 code
            break;
        }
    };
}

static void UNITY_INTERFACE_API on_render_event(int eventId)
{
	//render stuffs
}

extern "C" UnityRenderingEvent UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API flx_get_render_event_func()
{
	return on_render_event;
}

extern "C" const UNITY_INTERFACE_EXPORT char* UNITY_INTERFACE_API flx_get_test_msg()
{
	return "FlexiParts message woot\n";
}

FP(void) flx_print_msg()
{
	printf("FlexiParts native message?\n");
}

FP(void) flx_set_time(float t)
{
	time = t;
}

FP(void) flx_set_delta_time(float t)
{
	delta_time = t;
}