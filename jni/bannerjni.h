#ifndef _BANNER_BANNERJNI_H_

#define _BANNER_BANNERJNI_H_

#include <android/log.h>
#include <jni.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#define  LOG_TAG    "BannerViewJNI"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

extern void printGLString(const char *, GLenum);

extern void checkGlError(const char*);

#ifdef __cplusplus
extern "C" {
#endif

    JNIEXPORT void JNICALL Java_org_flakor_widget_BannerRender_init(JNIEnv * env, jobject obj,jint width, jint height,jobjectArray strArray);
    JNIEXPORT void JNICALL Java_org_flakor_widget_BannerRender_resize(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT void JNICALL Java_org_flakor_widget_BannerRender_step(JNIEnv * env, jobject obj);

#ifdef __cplusplus
};
#endif

#endif
