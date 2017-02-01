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

#if (UNITY_IPHONE || UNITY_WEBGL) && !UNITY_EDITOR
	[DllImport("__Internal")]
#else
    [DllImport("libflexiparts")]
#endif
    static extern void flx_set_mvp(float[] model, float[] view, float[] projection);

    void Awake()
	{
		//Debug.Log(Marshal.PtrToStringAnsi(flx_get_test_msg()));
  //      flx_print_msg();
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
            flx_set_mvp(Mat4ToFloat16(transform.localToWorldMatrix),
                    Mat4ToFloat16(Camera.main.worldToCameraMatrix),
                    Mat4ToFloat16(Camera.main.projectionMatrix));

            // Issue a plugin event with arbitrary integer identifier.
            // The plugin can distinguish between different
            // things it needs to do based on this ID.
            // For our simple plugin, it does not matter which ID we pass here.
            GL.IssuePluginEvent(flx_get_render_event_func(), 1);
        }
    }

    float[] Mat4ToFloat16(Matrix4x4 mat)
    {
        return new float[16] {
            mat.m00, mat.m01, mat.m02, mat.m03,
            mat.m10, mat.m11, mat.m12, mat.m13,
            mat.m20, mat.m21, mat.m22, mat.m23,
            mat.m30, mat.m31, mat.m32, mat.m33
        };
    }
}
