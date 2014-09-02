#include "bannerjni.h"
#include "Render.h"

static Render *g_renderer = NULL;

void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error
            = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}

JNIEXPORT void JNICALL Java_org_flakor_widget_BannerRender_init(JNIEnv * env, jobject obj,jint width, jint height,jobjectArray strArray)
{
    if (g_renderer) {
            delete g_renderer;
            g_renderer = NULL;
        }

    jstring jstr;
    jsize len = env->GetArrayLength(strArray);
    char **pstr = (char **) malloc(len*sizeof(char *));

    int i=0;

    for (i=0 ; i<len; i++)
    {
            jstr = (jstring)env->GetObjectArrayElement(strArray, i);
            pstr[i] = (char *)env->GetStringUTFChars(jstr, 0);
            LOGI("image:%s",pstr[i]);
    }

    //printGlString("Version", GL_VERSION);
    //printGlString("Vendor", GL_VENDOR);
    //printGlString("Renderer", GL_RENDERER);
    //printGlString("Extensions", GL_EXTENSIONS);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

   const char* versionStr = (const char*)glGetString(GL_VERSION);
   LOGI("OpenGL ES version:%s",versionStr);
   g_renderer = new Render((int)width,(int)height);
   g_renderer->init(i,pstr);

}

JNIEXPORT void JNICALL Java_org_flakor_widget_BannerRender_resize(JNIEnv * env, jobject obj,  jint width, jint height)
{
    if (g_renderer) {
        LOGE("RESIZE----");
            g_renderer->resize(width, height);
        }
}

JNIEXPORT void JNICALL Java_org_flakor_widget_BannerRender_step(JNIEnv * env, jobject obj)
{
    if (g_renderer) {
        LOGE("RENDER----");
       g_renderer->render();
    }
}






