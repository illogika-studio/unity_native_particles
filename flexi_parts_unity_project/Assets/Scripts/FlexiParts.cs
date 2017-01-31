using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;

public class FlexiParts : MonoBehaviour {
#if (UNITY_IPHONE || UNITY_WEBGL) && !UNITY_EDITOR
	[DllImport("__Internal")]
#else
	[DllImport("libflexiparts")]
#endif
	static extern System.IntPtr flx_get_test_msg();

#if (UNITY_IPHONE || UNITY_WEBGL) && !UNITY_EDITOR
	[DllImport("__Internal")]
#else
	[DllImport("libflexiparts")]
#endif
	static extern void flx_print_msg();

#if (UNITY_IPHONE || UNITY_WEBGL) && !UNITY_EDITOR
	[DllImport("__Internal")]
#else
    [DllImport("libflexiparts")]
#endif
    static extern System.IntPtr flx_get_render_event_func();

#if (UNITY_IPHONE || UNITY_WEBGL) && !UNITY_EDITOR
	[DllImport("__Internal")]
#else
    [DllImport("libflexiparts")]
#endif
    static extern void flx_set_time(float t);

#if (UNITY_IPHONE || UNITY_WEBGL) && !UNITY_EDITOR
	[DllImport("__Internal")]
#else
    [DllImport("libflexiparts")]
#endif
    static extern void flx_set_delta_time(float t);

#if (UNITY_IPHONE || UNITY_WEBGL) && !UNITY_EDITOR
	[DllImport("__Internal")]
#else
    [DllImport("libflexiparts")]
#endif
    static extern void flx_initialize(System.Int32 particle_qty);

    void Awake()
	{
		Debug.Log(Marshal.PtrToStringAnsi(flx_get_test_msg()));
        flx_print_msg();
        flx_initialize(1);
	}

    IEnumerator Start()
    {
        yield return StartCoroutine(CallPluginAtEndOfFrames());
    }

    void Update()
    {
    }

    IEnumerator CallPluginAtEndOfFrames()
    {
        while (true) {
            yield return new WaitForEndOfFrame();

            flx_set_time(Time.timeSinceLevelLoad);
            flx_set_delta_time(Time.deltaTime);

            // Issue a plugin event with arbitrary integer identifier.
            // The plugin can distinguish between different
            // things it needs to do based on this ID.
            // For our simple plugin, it does not matter which ID we pass here.
            GL.IssuePluginEvent(flx_get_render_event_func(), 1);
        }
    }
}
