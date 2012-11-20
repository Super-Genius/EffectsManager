package telly.EffectsManager;

import android.app.Activity;
import android.graphics.ImageFormat;
import android.hardware.Camera;
import android.hardware.Camera.Parameters;
import android.os.*;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;

import java.io.IOException;

import telly.EffectsManager.EffectManager;

/**
 * @author Kenneth Hurley
 * @version 1.1
 */
public abstract class CameraExample extends Activity implements SurfaceHolder.Callback2, Camera.PreviewCallback, View.OnClickListener {
    private static final String TAG = CameraExample.class.getSimpleName();

    private SurfaceHolder mSurfaceHolder;
    protected Camera mCameraDevice;
    private boolean mPreviewing;
    protected EffectManager mEffectManager = EffectManager.GetInstance();
    byte[][] mPreviewBuffer = new byte[2][];
    boolean bHasLoaded = false;
    SurfaceView mVideoPreview;
	int redrawCount;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mPreviewing = false;

        setContentView(R.layout.camera_example);
                
        mEffectManager.Init(this);
        
        mVideoPreview = (SurfaceView) findViewById(R.id.camera_preview);
        //ViewGroup.LayoutParams vpParams = mVideoPreview.getLayoutParams();
        //vpParams.width = 512;
        //vpParams.height = 512;
        //mVideoPreview.setLayoutParams(vpParams);
        
        mSurfaceHolder = mVideoPreview.getHolder();
        mSurfaceHolder.addCallback(this);
        //holder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
	
        
        initPreviewDisplay();
        mVideoPreview.setOnClickListener(this);
        

        //this.setVisible(true);
        EnableEffects(false);
        //mVideoPreview.setVisibility(View.VISIBLE);
        
    }

    @Override
    public void surfaceCreated(SurfaceHolder surfaceHolder) {
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        // Make sure we have a surface in the holder before proceeding.
        if (holder.getSurface() == null) {
            Log.d(TAG, "holder.getSurface() == null");
            return;
        }

        Log.d(TAG, String.format("Surface changed, width %d, height %d", width, height));

		mVideoPreview.setWillNotDraw(false);

        mSurfaceHolder = holder;
        if (mCameraDevice != null)
        {
        	try {
				mCameraDevice.setPreviewDisplay(holder);
			} catch (IOException e) {
				e.printStackTrace();
			}
        }
        restartPreview();
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder surfaceHolder) {
        mSurfaceHolder = null;
    }

    
    @Override
	public void surfaceRedrawNeeded(SurfaceHolder holder) {
    	
    	redrawCount++;
    	if (redrawCount == 1000) {
        	Log.d(TAG, "In surfaceRedrawNeeded" );
        	redrawCount = 0;
    	}
		
	}

	private void AllocateBuffers(int format, int width, int height) {
    	int buffSize;
    	int bpp = ImageFormat.getBitsPerPixel(format);
    	
    	buffSize = (width * height * bpp) / 8;
    	
    	mPreviewBuffer[0] = new byte[buffSize];
    	mPreviewBuffer[1] = new byte[buffSize];
    	
    }
    
    private void initPreviewDisplay() {
    	Log.d(TAG, "initPreviewDisplay");
        try {
            if (mCameraDevice == null) {
                // If the activity is paused and resumed, camera device has been
                // released and we need to open the camera.
                mCameraDevice = CameraHolder.instance().open(0);
            }
       	
            Parameters camParams = mCameraDevice.getParameters();
                        
            Camera.Size camSize = camParams.getPreviewSize();
            AllocateBuffers(camParams.getPreviewFormat(), camSize.width, camSize.height);
            
            mCameraDevice.addCallbackBuffer(mPreviewBuffer[0]);
            mCameraDevice.addCallbackBuffer(mPreviewBuffer[1]);            
            mCameraDevice.setPreviewCallbackWithBuffer(this);
            // set the preview callback listener
            //mCameraDevice.setPreviewCallback(this);
            
            Log.d(TAG, "Done setting preview callback");
            //setVisible(false);

        } catch (Throwable ex) {
            closeCamera();
            throw new RuntimeException("initPreviewDisplay failed", ex);
        }
    }

    @Override
	public void onClick(View v) {
        EnableEffects(true);
		//mEffectManager.Show();
	}

	private void startPreview() {
        Log.v(TAG, "startPreview");

        if (mPreviewing) {
        	mEffectManager.Pause();
            mCameraDevice.stopPreview();
            mPreviewing = false;
        }
       
        setCameraDisplayOrientation(this, 0, mCameraDevice);
        
        try {
            Log.d(TAG, "Starting Camera Start Preview");
            mCameraDevice.startPreview();
            mPreviewing = true;
        } catch (Throwable ex) {
            closeCamera();
            throw new RuntimeException("startPreview failed", ex);
        }
    }


    private void closeCamera() {
        Log.v(TAG, "closeCamera");
        if (mCameraDevice == null) {
            Log.d(TAG, "already stopped.");
            return;
        }
        // If we don't lock the camera, release() will fail.
        mCameraDevice.lock();
        CameraHolder.instance().release();
        mCameraDevice = null;
        mPreviewing = false;
    }

    private static void setCameraDisplayOrientation(Activity activity,
                                                    int cameraId, Camera camera) {
        // See android.hardware.Camera.setCameraDisplayOrientation for
        // documentation.
        CameraInfo info = new CameraInfo();
        CameraInfo.getCameraInfo(cameraId, info);
        int degrees = getDisplayRotation(activity);
        int result = (info.orientation - degrees + 360) % 360;
        camera.setDisplayOrientation(result);
    }

    private static int getDisplayRotation(Activity activity) {
        int rotation = activity.getWindowManager().getDefaultDisplay()
                .getRotation();
        switch (rotation) {
            case Surface.ROTATION_0:
                return 0;
            case Surface.ROTATION_90:
                return 90;
            case Surface.ROTATION_180:
                return 180;
            case Surface.ROTATION_270:
                return 270;
        }
        return 0;
    }

    private boolean restartPreview() {
        try {
            startPreview();
        } catch (RuntimeException e) {
            return false;
        }
        return true;
    }



    @Override
    protected void onDestroy() {
        super.onDestroy();
        mEffectManager.Stop();
        mCameraDevice.stopPreview();
    }
    
    
    @Override
    protected void onPause() {
    	super.onPause();
		Log.d(TAG, "OnPause");
	    mEffectManager.Pause();       	
    }
    
	@Override
	protected void onResume() {
		super.onResume();
		Log.d(TAG, "OnResume");
		mEffectManager.Resume();   	       
	}

	public void EnableEffects(boolean on) {
		if (on) {
			//mVideoPreview.setWillNotDraw(true);
			//mVideoPreview.setVisibility(View.INVISIBLE);
			mEffectManager.Show();
		}
		else
		{
			mEffectManager.Hide();
			//mVideoPreview.setWillNotDraw(false);
		}
	}


	/**
     * This is used to fake the Camera.CameraInfo class introduced in Gingerbread.
     *
     * @author cristian
     * @version 1.0
     */
    public static class CameraInfo {

        public int facing;
        public int orientation;

        /**
         * Returns the information about a particular camera. If getNumberOfCameras() returns N,
         * the valid id is 0 to N-1.
         *
         * @param cameraId the camera id
         * @param info     the object to put the information into
         */
        public static void getCameraInfo(int cameraId, CameraInfo info) {
            Camera.CameraInfo newerCameraInfo = new Camera.CameraInfo();
            Camera.getCameraInfo(cameraId, newerCameraInfo);
            info.facing = newerCameraInfo.facing;
            info.orientation = newerCameraInfo.orientation;
        }

    }

    /**
     * The class is used to hold an {@code android.hardware.Camera} instance.
     *
     * <p>The {@code open()} and {@code release()} calls are similar to the ones
     * in {@code android.hardware.Camera}. The difference is if {@code keep()} is
     * called before {@code release()}, CameraHolder will try to hold the {@code
     * android.hardware.Camera} instance for a while, so if {@code open()} is
     * called soon after, we can avoid the cost of {@code open()} in {@code
     * android.hardware.Camera}.
     *
     * <p>This is used in switching between {@code Camera} and {@code VideoCamera}
     * activities.
     */
    public static class CameraHolder {
        private static final String TAG = "CameraHolder";
        private Camera mCameraDevice;
        private long mKeepBeforeTime = 0;  // Keep the Camera before this time.
        private final Handler mHandler;
        private int mUsers = 0;  // number of open() - number of release()
        private int mCameraId = -1;

        // We store the camera parameters when we actually open the device,
        // so we can restore them in the subsequent open() requests by the user.
        // This prevents the parameters set by the Camera activity used by
        // the VideoCamera activity inadvertently.
        private Camera.Parameters mParameters;

        // Use a singleton.
        private static CameraHolder sHolder;
        public static synchronized CameraHolder instance() {
            if (sHolder == null) {
                sHolder = new CameraHolder();
            }
            return sHolder;
        }

        private static final int RELEASE_CAMERA = 1;
        private class MyHandler extends Handler {
            MyHandler(Looper looper) {
                super(looper);
            }

            @Override
            public void handleMessage(Message msg) {
                switch(msg.what) {
                    case RELEASE_CAMERA:
                        synchronized (CameraHolder.this) {
                            // In 'CameraHolder.open', the 'RELEASE_CAMERA' message
                            // will be removed if it is found in the queue. However,
                            // there is a chance that this message has been handled
                            // before being removed. So, we need to add a check
                            // here:
                            if (CameraHolder.this.mUsers == 0) releaseCamera();
                        }
                        break;
                }
            }
        }

        private CameraHolder() {
            HandlerThread ht = new HandlerThread("CameraHolder");
            ht.start();
            mHandler = new MyHandler(ht.getLooper());
            int mNumberOfCameras = Camera.getNumberOfCameras();
            CameraInfo[] mInfo = new CameraInfo[mNumberOfCameras];
            for (int i = 0; i < mNumberOfCameras; i++) {
                mInfo[i] = new CameraInfo();
                CameraInfo.getCameraInfo(i, mInfo[i]);
            }
        }

        public synchronized Camera open(int cameraId) {
            if (mCameraDevice != null && mCameraId != cameraId) {
                mCameraDevice.release();
                mCameraDevice = null;
                mCameraId = -1;
            }
            if (mCameraDevice == null) {
                try {
                    Log.v(TAG, "open camera " + cameraId);
                    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.GINGERBREAD) {
                        mCameraDevice = Camera.open(cameraId);
                    } else {
                        mCameraDevice = Camera.open();
                    }
                    mCameraId = cameraId;
                } catch (RuntimeException e) {
                    Log.e(TAG, "fail to connect Camera", e);
                    throw new RuntimeException(e);
                }
                mParameters = mCameraDevice.getParameters();
            } else {
                try {
                    mCameraDevice.reconnect();
                } catch (IOException e) {
                    Log.e(TAG, "reconnect failed.");
                    throw new RuntimeException(e);
                }
                mCameraDevice.setParameters(mParameters);
            }
            ++mUsers;
            mHandler.removeMessages(RELEASE_CAMERA);
            mKeepBeforeTime = 0;
            return mCameraDevice;
        }

        public synchronized void release() {
            --mUsers;
            mCameraDevice.stopPreview();
            releaseCamera();
        }

        private synchronized void releaseCamera() {
            long now = System.currentTimeMillis();
            if (now < mKeepBeforeTime) {
                mHandler.sendEmptyMessageDelayed(RELEASE_CAMERA,
                        mKeepBeforeTime - now);
                return;
            }
            if (mCameraDevice != null) {
                mCameraDevice.release();
            }
            mCameraDevice = null;
            mCameraId = -1;
        }
        
    }
}

