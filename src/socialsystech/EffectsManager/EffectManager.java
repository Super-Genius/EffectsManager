package socialsystech.EffectsManager;

import android.app.Activity;
import android.content.res.AssetManager;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;


public class EffectManager{
    
	private static String kTAG = "EffectManager";
    AssetManager mAssetMgr;
    private static EffectManager _instance = new EffectManager();
	private boolean bAreShadersLoaded = false;
	private Activity mActivity;
    
    private EffectManager()
    {
    }
    
    public static EffectManager GetInstance() {
    	return _instance;
    }
    
    private void LoadShaders() {
    	if (!bAreShadersLoaded) {
	        // for testing
	        Log.i(kTAG, "Loading Effects");
	   		// for quick testing
			LoadEffect("shaders/noir.json");    	
	        bAreShadersLoaded = true;
    	}
    	else {
    		ReInitEffects();
    	}
		
        // for testing
        Log.i(kTAG, "Selecting technique Noir1920");
        SelectEffectTechnique("Noir1920", "GLES20");        	

    }
    
    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
    	Log.d(kTAG, "Surface Changed");
    }

    public void surfaceCreated(SurfaceHolder holder) {
    	Log.d(kTAG, "Surface Created");
    	// make sure thread is paused
    	RendererOnPause();
        RendererSetSurface(holder.getSurface());    	
        // let's get this party started again
        ReInitEffects();
        RendererOnResume();
       }

    public void surfaceDestroyed(SurfaceHolder holder) {
    	Log.d(kTAG, "Surface Destroyed");
    	RendererOnPause();
        RendererSetSurface(null);
    }
     
    public static native void RendererOnResume();
    public static native void RendererOnPause();
    public static native void RendererOnStop();
    public static native void RendererSetSurface(Surface surface);
    public static native void SelectEffectTechnique(String effectName, String techinqueName);
    public static native void RendererUpdateAuxiliary(int width, int height, byte[] auxImageData);
    public static native void SetAppDirectory(String pkgName, AssetManager assetManager);
    public static native boolean LoadEffect(String effectFileName);
    public static native void ReInitEffects();

    static {
        System.loadLibrary("EffectsManager");
    }
 }

