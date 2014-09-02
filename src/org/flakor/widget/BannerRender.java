package org.flakor.widget;

import android.opengl.GLSurfaceView;
import android.os.Environment;
import android.util.Log;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by saint(saint@aliyun.com) on 4/28/14.
 */
public class BannerRender implements GLSurfaceView.Renderer
{
    private static String TAG = "3D Banner Render";
    private int width,height;
    
    static {
        System.loadLibrary("bannerviewJNI");
    }

    public BannerRender()
    {
    }
    
    public void setView(int width,int height)
    {
    	this.width = width;
    	this.height = height;
    }
    
    @Override
    public void onSurfaceCreated(GL10 glUnused, EGLConfig config) {
    	
    	String baseDir;
    	boolean hasSDCard = Environment.getExternalStorageState()
				.equals(Environment.MEDIA_MOUNTED);
    	if(hasSDCard)
    	{
    		baseDir = Environment.getExternalStorageDirectory().getAbsolutePath();
    		String tmp = baseDir+"/testimage/2.png";
    		String[] pic = {
    				tmp
    		};
    		Log.d(TAG, "width:"+width+";height:"+height);
    		init(width,height,pic);
    	}
    	//init(width,height);
    }

    @Override
    public void onSurfaceChanged(GL10 glUnused, int width, int height) {
        resize(width, height);
    }

    @Override
    public void onDrawFrame(GL10 glUnused) {
        step();
    }

    public static native void init(int width,int height,String pic[]);
    public static native void resize(int width, int height);
    public static native void step();
}
