#ifndef _BANNER_GLPROGRAM_H_
#define _BANNER_GLPROGRAM_H_

#include <malloc.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "vecmath.h"

enum SHADER_ATTRIBUTES
{
    ATTRIB_VERTEX,
    ATTRIB_UV,
};
typedef struct BANNER_VERTEX
{
    float pos[3];
    float uv[2];
} BannerVertex;

/*typedef struct _bannerData {
		//left bottom
		BannerVertex lb;
		//right bottom
		BannerVertex rb;
		//left top
		BannerVertex lt;
		//right top
		BannerVertex rt;
} BannerData;*/

static const char VERTEX_SHADER[] =
"uniform mat4 uMVPMatrix;\n"
"attribute vec3 aPosition;\n"
"attribute vec2 aTextureCoord;\n"
"varying vec2 vTextureCoord;\n"
"void main() {\n"
"  gl_Position = uMVPMatrix * vec4(aPosition,1.0);\n"
"  vTextureCoord = aTextureCoord;\n"
"}\n";

static const char FRAGMENT_SHADER[] =
"precision mediump float;\n"
"varying vec2 vTextureCoord;\n"
"uniform sampler2D sTexture;\n"
"void main() {\n"
"  gl_FragColor = texture2D(sTexture, vTextureCoord);\n"
"}\n";

class GLProgram
{
	public:
		GLProgram(void);
		~GLProgram(void);
		bool init(void);
		void use(Mat4 *mat);
		static GLProgram * getShareProgram(void);
	protected:
		GLuint mProgram;

		GLint mPosAttrib;
		GLint mTextureCoordAttrib;
		GLint mMatrixUniform;
		GLint mSampleUniform;
	private:
		GLuint createShader(GLenum shaderType,const char* src);
		GLuint createProgram(const char* vtxSrc, const char* fragSrc);
};

#endif

