
#include "GLProgram.h"
static GLProgram *instance = NULL;


GLProgram::GLProgram():
mProgram(0),
mPosAttrib(-1),
mTextureCoordAttrib(-1),
mMatrixUniform(-1),
mSampleUniform(-1)
{}

GLProgram::~GLProgram()
{
	 glDeleteProgram(mProgram);
}

bool GLProgram::init()
{
    mProgram = createProgram(VERTEX_SHADER, FRAGMENT_SHADER);
	LOGE("mProgram init:%d",mProgram);

    if (!mProgram)
        return false;
    mPosAttrib = glGetAttribLocation(mProgram, "aPosition");
	LOGE("aPosition:%d",mPosAttrib);
    mTextureCoordAttrib = glGetAttribLocation(mProgram, "aTextureCoord");
	LOGE("aTextureCoord:%d",mTextureCoordAttrib);
    mMatrixUniform = glGetUniformLocation(mProgram,"uMVPMatrix");
	LOGE("uMVPMatrix:%d",mMatrixUniform);
    mSampleUniform = glGetUniformLocation(mProgram,"sTexture");
	LOGE("sTexture:%d",mSampleUniform);
    return true;
}

GLProgram* GLProgram::getShareProgram(void)
{
	if(!instance) 
    {
        instance = new GLProgram();
        if(!instance ->init())
            return NULL;
    }

    return instance;
}

GLuint GLProgram::createShader(GLenum shaderType, const char* src)
{
    GLuint shader = glCreateShader(shaderType);

    if (!shader) {
        checkGlError("glCreateShader");
        return 0;
    }
    glShaderSource(shader, 1, &src, NULL);
    checkGlError("glShaderSource");

    GLint compiled = GL_FALSE;
    glCompileShader(shader);
    checkGlError("glCompileShader");
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    checkGlError("glGetShaderiv");

    LOGI("COMPILED:%d",compiled);
    if (compiled == GL_FALSE)
	{
        GLint infoLogLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);
        LOGI("info length:%d",infoLogLen);

        char logBuf[1024];
        glGetShaderInfoLog(shader, sizeof(logBuf), NULL, (GLchar*)logBuf );
        LOGE("Could not compile %s shader:\n%s\n",
                                shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment",
                                logBuf);
        if (infoLogLen > 0) {
            GLchar* infoLog = (GLchar*)malloc(infoLogLen);
            if (infoLog) {
                glGetShaderInfoLog(shader, infoLogLen, NULL, infoLog);
                LOGE("Could not compile %s shader:\n%s\n",
                        shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment",
                        infoLog);
                free(infoLog);
            }
        }
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint GLProgram::createProgram(const char* vtxSrc, const char* fragSrc) {
    GLuint vtxShader = 0;
    GLuint fragShader = 0;
    GLuint program = 0;
    GLint linked = GL_FALSE;

    vtxShader = createShader(GL_VERTEX_SHADER, vtxSrc);
    if (!vtxShader)
        goto exit;

    fragShader = createShader(GL_FRAGMENT_SHADER, fragSrc);
    if (!fragShader)
        goto exit;

    program = glCreateProgram();
    if (!program) {
        checkGlError("glCreateProgram");
        goto exit;
    }
    glAttachShader(program, vtxShader);
    glAttachShader(program, fragShader);


	// Bind attribute locations
    // this needs to be done prior to linking
    //glBindAttribLocation( program, ATTRIB_VERTEX, "aPosition" );
    //glBindAttribLocation( program, ATTRIB_UV, "aTextureCoord" );

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        LOGE("Could not link program");
        GLint infoLogLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen) {
            GLchar* infoLog = (GLchar*)malloc(infoLogLen);
            if (infoLog) {
                glGetProgramInfoLog(program, infoLogLen, NULL, infoLog);
                LOGE("Could not link program:\n%s\n", infoLog);
                free(infoLog);
            }
        }
        glDeleteProgram(program);
        program = 0;
    }

exit:
	LOGE("program failed!!");
    glDeleteShader(vtxShader);
    glDeleteShader(fragShader);
    return program;
}

void GLProgram::use(Mat4 *mat)
{
    LOGE("START LOAD SHADER");
    glUseProgram(mProgram);

    checkGlError("glUseProgram");
    // load the position
    // 3(x , y , z)
    // (2 + 3 )* 4 (float size) = 20
    glVertexAttribPointer(mPosAttrib,
                          3, GL_FLOAT,
                                 false, 20, 0);
    checkGlError("glVertexAttribPointer");
    glEnableVertexAttribArray(mPosAttrib);
    checkGlError("glEnableVertexAttribArray");
    // load the texture coordinate
    glVertexAttribPointer(mTextureCoordAttrib, 
                                  2, GL_FLOAT,
                                  false, 20, (GLvoid*)(sizeof(GLfloat)*3));
    checkGlError("glVertexAttribPointer");
    glEnableVertexAttribArray(mTextureCoordAttrib);
    checkGlError("glEnableVertexAttribArray");
    GLfloat matrix[] = {0.0028, 0.0, 0.0, 0.0, 0.0, -0.0042, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, -0.71, 1.07, 0.0, 1.0};
    glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, matrix);
    checkGlError("glUniformMatrix4fv");
    glUniform1i(mSampleUniform,0);

    LOGE("LOAD SHADER END");

}
