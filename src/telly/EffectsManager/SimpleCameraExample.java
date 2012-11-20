package telly.EffectsManager;

import android.hardware.Camera;

/**
 * @author cristian
 * @version 1.0
 */
public class SimpleCameraExample extends CameraExample {
    @Override
    public void onPreviewFrame(byte[] bytes, Camera camera) {
    	if (bytes != null) {
	    	Camera.Size camSize = camera.getParameters().getPreviewSize();
	    	mEffectManager.updateAuxiliarySurface(camSize.width, camSize.height, bytes);
			// add it back into the queue.  this is double buffered
			mCameraDevice.addCallbackBuffer(bytes);
    	}
        //System.out.println("Got a yuv with" + bytes.length + " bytes");
    }
}
