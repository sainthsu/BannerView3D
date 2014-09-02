#include "Banner.h"

Banner::Banner():
pic(NULL),
mTextureID(0),
image(NULL),
mVBO(0),
type(UNKNOWN)
{
	data = new BannerVertex[4];
	if(!data)
		LOGI("banner data alloc error!");
	else
	{
	  data[0].uv[0] = 0.0f;
	  data[0].uv[1] = 0.0f;
	  data[1].uv[0] = 1.0f;
	  data[1].uv[1] = 0.0f;
	  data[2].uv[0] = 1.0f;
	  data[2].uv[1] = 1.0f;
	  data[3].uv[0] = 0.0f;
	  data[3].uv[1] = 1.0f;
	}

	mProgram = GLProgram::getShareProgram();
}

Banner::~Banner()
{
	free(data);
	glDeleteBuffers(1, &mVBO);

}

void Banner::setData(GLfloat vertexData[],const char* piclocation)
{
	data[0].pos[0] = vertexData[0];
	data[0].pos[1] = vertexData[1];
	data[0].pos[2] = vertexData[2];

	data[1].pos[0] = vertexData[3];
	data[1].pos[1] = vertexData[4];
	data[1].pos[2] = vertexData[5];

	data[2].pos[0] = vertexData[6];
	data[2].pos[1] = vertexData[7];
	data[2].pos[2] = vertexData[8];

	data[3].pos[0] = vertexData[9];
	data[3].pos[1] = vertexData[10];
	data[3].pos[2] = vertexData[11];

	pic = piclocation;
	char * pch;
	pch = strstr(pic,"jpg");
	if(pch == NULL)
	{
			pch = strstr(pic,"png");
			if(pch == NULL)
			{
				type=UNKNOWN;
				LOGI("unknown picture type");
			}
			else
			{
				type = PNG;
			}
		}
		else
		{
		    LOGI("picture type is jpeg!!!");
		    type = JPEG;
		}
}

bool Banner::load(void)
{
    FILE *fd;

    fd = fopen(pic, "rb");
    if (!fd){
        LOGI("open file error");
       return false;
    }

    //load texture to gpu
    bool result = false;
    if(type == JPEG)
    {
        result = loadImageJPEG(fd);
    }
    else if(type == PNG)
    {
        result = loadImagePNG(fd);
    }

    return result;
}

void Banner::draw(Mat4 *mat)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	checkGlError("glBindTexture");
    glBindBuffer( GL_ARRAY_BUFFER, mVBO);
    checkGlError("glBindBuffer");

    mProgram->use(mat);
    LOGE("BANNER INNER DRAWING!!!!");

    GLubyte index[6] = {0,1,2,0,2,3};

    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    //glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_BYTE, &index);
    checkGlError("glDrawArrays");
    glBindBuffer( GL_ARRAY_BUFFER,0);
    checkGlError("glBindBuffer");
}

#define SCALE 1

bool Banner::loadImageJPEG(FILE *fd)
{
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    JSAMPROW row_pointer[1];
    unsigned long location = 0;

    cinfo.err = jpeg_std_error(&jerr);

    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, fd);
    jpeg_read_header(&cinfo, TRUE);
    //cinfo.scale_num = 1;
    //cinfo.scale_denom = SCALE;
    jpeg_start_decompress(&cinfo);
    int width = cinfo.output_width;
    int height = cinfo.output_height;
    int depth = cinfo.num_components; //should always be 3

	LOGI("width:%d;height:%d;depth:%d",width,height,depth);
    image = (unsigned char *) malloc(width * height * depth);
    row_pointer[0] = (unsigned char *) malloc(width * depth);
    /* read one scan line at a time */
    int i;
    while( cinfo.output_scanline < cinfo.output_height )
    {
        jpeg_read_scanlines( &cinfo, row_pointer, 1 );
        for( i=0; i< (width * depth); i++)
            image[location++] = row_pointer[0][i];
    }
    fclose(fd);
    free(row_pointer[0]);

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    //gen vbo
    glGenBuffers(1, &mVBO);
	checkGlError("glGenBuffers");
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	checkGlError("glBindBuffer");
    glBufferData(GL_ARRAY_BUFFER, sizeof(struct BANNER_VERTEX)*4, data, GL_STATIC_DRAW);
	checkGlError("glBufferData");

	LOGE("mVBO:%d",mVBO);
    glActiveTexture(GL_TEXTURE0);
    checkGlError("glActiveTexture");
    glGenTextures(1, &mTextureID);
    checkGlError("glGenTextures");
    glBindTexture(GL_TEXTURE_2D, mTextureID);
    checkGlError("glBindTexture");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)image);
    checkGlError("glTexImage2D");
	LOGE("Textureid:%d",mTextureID);
    LOGE("LOADING DONE!!!!");

	return true;
}


bool Banner::loadImagePNG(FILE *fp)
{
     png_structp png_ptr;
     png_infop info_ptr;
     unsigned int sig_read = 0;
     int color_type, interlace_type;

     /* Create and initialize the png_struct
     * with the desired error handler
     * functions.  If you want to use the
     * default stderr and longjump method,
     * you can supply NULL for the last
     * three parameters.  We also supply the
     * the compiler header file version, so
     * that we know if the application
     * was compiled with a compatible version
     * of the library.  REQUIRED
     */
     png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                 NULL, NULL, NULL);

     if (png_ptr == NULL) {
             fclose(fp);
             return false;
     }

     /* Allocate/initialize the memory
     * for image information.  REQUIRED. */
     info_ptr = png_create_info_struct(png_ptr);
     if (info_ptr == NULL) {
             fclose(fp);
             png_destroy_read_struct(&png_ptr, NULL, NULL);
             return false;
     }

     /* Set error handling if you are
      * using the setjmp/longjmp method
      * (this is the normal method of
      * doing things with libpng).
      * REQUIRED unless you  set up
      * your own error handlers in
      * the png_create_read_struct()
      * earlier.
     */
     if (setjmp(png_jmpbuf(png_ptr))) {
          /* Free all of the memory associated
           * with the png_ptr and info_ptr */
             png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
             fclose(fp);
             /* If we get here, we had a
              * problem reading the file */
             return false;
     }

     /* Set up the output control if
      * you are using standard C streams */
      png_init_io(png_ptr, fp);

     /* If we have already
      * read some of the signature */
      png_set_sig_bytes(png_ptr, sig_read);

     /*
      * If you have enough memory to read
      * in the entire image at once, and
      * you need to specify only
      * transforms that can be controlled
      * with one of the PNG_TRANSFORM_*
      * bits (this presently excludes
      * dithering, filling, setting
      * background, and doing gamma
      * adjustment), then you can read the
      * entire image (including pixels)
      * into the info structure with this
      * call
      *
      * PNG_TRANSFORM_STRIP_16 |
      * PNG_TRANSFORM_PACKING  forces 8 bit
      * PNG_TRANSFORM_EXPAND forces to
      *  expand a palette into RGB
      */
      png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

      int outWidth = png_get_image_width(png_ptr,info_ptr);
      int outHeight = png_get_image_height(png_ptr,info_ptr);
      /*bool outHasAlpha = false;
      switch (info_ptr->color_type) {
         case PNG_COLOR_TYPE_RGBA:
             outHasAlpha = true;
             break;
         case PNG_COLOR_TYPE_RGB:
              outHasAlpha = false;
              break;
         default:
              LOGI("Color type %d not supported!\n",info_ptr->color_type);
              png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
              fclose(fp);
              return false;
       }
       */
       unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
       image = (unsigned char*) malloc(row_bytes * outHeight);

       png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

       for (int i = 0; i < outHeight; i++){
             // note that png is ordered top to
             // bottom, but OpenGL expect it bottom to top
             // so the order or swapped
             memcpy(image+(row_bytes * (outHeight-1-i)), row_pointers[i], row_bytes);
       }

       /* Clean up after the read,
        * and free any memory allocated */
       png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

       /* Close the file */
       fclose(fp);

       //gen vbo
       glGenBuffers(1, &mVBO);
       glBindBuffer(GL_ARRAY_BUFFER, mVBO);
       GLfloat vv[]={0.0000f,
    		   0.0000f,
    		   0.0000f,
    		   0.0000f,
    		   0.0000,
    		   0.0000,
    		   256.0000,
    		   0.0000f,
    		   0.0000f,
    		   1.0000f,
    		   256.0000f,
    		   0.0000f,
    		   0.0000f,
    		   1.0000f,
    		   0.0000f,
    		   256.0000f,
    		   256.0000f,
    		   0.0000f,
    		   1.0000f,
    		   1.0000f,
       };
       glBufferData(GL_ARRAY_BUFFER, sizeof(struct BANNER_VERTEX)*4, vv, GL_STATIC_DRAW);

       glActiveTexture(GL_TEXTURE0);
       glGenTextures(1, &mTextureID);
       glBindTexture(GL_TEXTURE_2D, mTextureID);

       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
       glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, outWidth, outHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)image);
       /* That's it */
       return true;
}
