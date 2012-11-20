package telly.EffectsManager;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;

import dalvik.system.DexClassLoader;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;
import android.hardware.Camera;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import telly.EffectsInterface.EffectsInterface;

public class EffectManager implements SurfaceHolder.Callback, View.OnClickListener {
    
	private static String kTAG = "EffectManager";
	private int auxSurfaceWidth;
	private int auxSurfaceHeight;
	private int auxSurfaceFormat;
	private boolean bIsSurfaceCrated = false;
    AssetManager assetMgr;
    private static EffectManager _instance = new EffectManager();
    SurfaceView mSurfaceView;
	private SurfaceHolder mSurfaceHolder;
	Camera mCameraDevice;
	private boolean bAreShadersLoaded = false;
	private Activity mActivity;
	private EffectsInterface mEFI;
    
    private EffectManager()
    {
    }
    
    @Override
	public void onClick(View v) {
		v.setVisibility(View.INVISIBLE);
	}


	public boolean IsSurfaceCreated() {
    	return bIsSurfaceCrated;
    }
	
	/** Called when the activity is first created. */
    public void Init(Activity activity) {

        Log.i(kTAG, "Init()");

        mActivity = activity;
        //InitRenderer();
        mSurfaceView = new SurfaceView(activity);
        mSurfaceHolder = mSurfaceView.getHolder();
        mSurfaceHolder.addCallback(this);
        mSurfaceView.setZOrderMediaOverlay(true);
        mSurfaceView.setOnClickListener(this);

        assetMgr = activity.getAssets();
        SetAppDirectory(activity.getPackageName(), assetMgr);        
	    
        activity.addContentView(mSurfaceView, new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));

        mEFI = SelectImplementation();
        
        //RendererSetSurface(mSurfaceHolder.getSurface());

        //try {
		//	testYUVImage = ReadSample("textures/carphone001.yuv");
		//} catch (IOException e) {
		//	e.printStackTrace();
		//}
        
    }

    private byte[] ReadSample(String name) throws IOException
    {

    	InputStream asset;
    	
		asset = assetMgr.open(name, AssetManager.ACCESS_BUFFER);

    	ByteArrayOutputStream buffer = new ByteArrayOutputStream();

    	int nRead;
    	byte[] data = new byte[16384];

		while ((nRead = asset.read(data, 0, data.length)) != -1) {
		  buffer.write(data, 0, nRead);
		}

    	buffer.flush();

    	return buffer.toByteArray();
    	
    }
    
    protected void Resume() {
        Log.i(kTAG, "Resume()");
        RendererOnResume();
    }
    
    protected void Pause() {
        Log.i(kTAG, "Pause()");
        RendererOnPause();
    }

    protected void Stop() {
        Log.i(kTAG, "Stop()");
        RendererOnStop();
    }
    
    public void Hide() {
    	if (mSurfaceView != null) {
    		mSurfaceView.setVisibility(View.INVISIBLE);
    	}
    }
   
    public void Show() {
    	if (mSurfaceView != null) {
    		mSurfaceView.setVisibility(View.VISIBLE);
    	}
    }
   
    public static EffectManager GetInstance() {
    	return _instance;
    }
    
    public void updateAuxiliarySurface(int width, int height, byte[] data) {
    	RendererUpdateAuxiliary(width, height, data);
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
    	// make sure thread is paused
    	RendererOnPause();
        RendererSetSurface(holder.getSurface());    	
        // let's get this party started again
    	LoadShaders();
        RendererOnResume();
    	Log.d(kTAG, "Surface Created");
       }

    public void surfaceDestroyed(SurfaceHolder holder) {
    	RendererOnPause();
        RendererSetSurface(null);
    }

    public void LoadEffect(String name) {
        if (!EffectManagerLoad(name)) {
        	Log.e(kTAG, "Couldn't load " + name + " for some reason");
        }    	
    }
    
    public void SelectEffectTechnique(String effectName, String techniqueName) {
    	EffectManagerSelectTechnique(effectName, techniqueName);
    }
 
    private static native void RendererOnResume();
    private static native void RendererOnPause();
    private static native void RendererOnStop();
    private static native void RendererSetSurface(Surface surface);
    private static native void EffectManagerSelectTechnique(String effectName, String techinqueName);
    private static native void RendererUpdateAuxiliary(int width, int height, byte[] auxImageData);
    private static native void SetAppDirectory(String pkgName, AssetManager assetManager);
    private static native boolean EffectManagerLoad(String effectFileName);
    private static native void ReInitEffects();

    static {
        System.loadLibrary("EffectsManager");
    }
    
    private EffectsInterface SelectImplementation() {
    	EffectsInterface efi = null;
    	
    	String defaultLibPath =  mActivity.getAssets() + "/Plugins/effectjava.jar";
    	File dexOutputDir = mActivity.getDir("dex", Context.MODE_PRIVATE);
    	DexClassLoader dcl = new DexClassLoader(defaultLibPath,
                dexOutputDir.getAbsolutePath(),
                null,
                ClassLoader.getSystemClassLoader());
    	try {
			Class<?> clazz = dcl.loadClass("EffectJavaImpl");
			efi = (EffectsInterface) clazz.newInstance();
		} catch (ClassNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (InstantiationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IllegalAccessException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    	
    	return efi;
    	
    }
    
}

