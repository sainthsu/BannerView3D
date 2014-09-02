#ifndef _BANNER_RENDER_H_
#define _BANNER_RENDER_H_

#include <string.h>
#include <time.h>
#include <math.h>

#include "tapCamera.h"
#include "Banner.h"


#define PI 3.14159265

class Render
{
    public:
        Render(int width,int height);
        ~Render(void);
        bool init(int num,char ** location);
        void resize(int w, int h);
        void render(void);

    protected:
        int mWidth;
        int mHeight;
        int mBannerNum;
        int mVertexNum;

        Banner *mBanners;
        char ** mPicLocation;
        int mCurPosition;

        TapCamera *camera;
        Mat4 mat_projection_;
        Mat4 mat_view_;
        Mat4 mat_model_;

        bool mRunning;
        bool mFlinging;
    private:
        void updateViewPort(void);
};

#endif
