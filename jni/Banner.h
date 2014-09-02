#ifndef _BANNER_BANNER_H_
#define _BANNER_BANNER_H_

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <jpeglib.h>
#include <png.h>
#include <zlib.h>

#include "GLProgram.h"

/*
 * <setjmp.h> is used for the optional error recovery mechanism shown in
 * the second part of the example.
 */

#include <setjmp.h>

#define VERTEX_PER_BANNER 12
#define UVNUM_PER_BANNER 8

typedef enum {
		UNKNOWN,
		JPEG,
		PNG
}ImageType;

#define DATALENGTH(_data_) (sizeof(_data_))

class Banner
{
		protected:
				BannerVertex *data;
				const char *pic;
				ImageType type;
				unsigned char *image;

				GLProgram *mProgram;
				GLuint mVBO;
				GLuint mTextureID;
		public:
				Banner();
				~Banner();
				bool load(void);
				void setData(GLfloat data[], const char *pic);
				void draw(Mat4 *mat);
				GLint getTextureId(void);
		private:
				void checkImageType(void);
				bool loadImageJPEG(FILE *fd);
				bool loadImagePNG(FILE *fd);
};

#endif
