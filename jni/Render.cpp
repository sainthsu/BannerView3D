#include "Render.h"

Render::Render(int width,int height)
:mBannerNum(0),
 mVertexNum(0),
 mWidth(width),
 mHeight(height),
 mCurPosition(0),
 mPicLocation(NULL),
 mBanners(NULL),
 camera(NULL),
 mRunning(false),
 mFlinging(false)
{
  mat_model_ = Mat4::Translation( 0, 0, -15.f );

  Mat4 mat = Mat4::RotationX( M_PI / 3 );
  mat_model_ = mat * mat_model_;
}

Render::~Render(void)
{
    delete [] mBanners;
    mBanners = NULL;
}
/**
 * @brief 假定向量P的坐标为（px，py，pz），向量A的坐标为（ax，ay，az）
 * 且：
 * A×P=（ay * pz- az * py, ax * pz- az * px , ax * py- ay * px）
 * A·P = ax * px + ay * py + az * pz
 * 则：
 * Px’= px * cosθ+( ay * pz- az * py)sinθ + ax (ax * px + ay * py + az * pz)(1 - cosθ)
 * Py’= py * cosθ+( ax * pz- az * px)sinθ + ay (ax * px + ay * py + az * pz)(1 - cosθ)
 * Pz’= pz * cosθ+( ax * py- ay * px)sinθ + az (ax * px + ay * py + az * pz)(1 - cosθ)
 * @param num
 * @return
 */
bool Render::init(int num,char ** location)
{
    mBannerNum = num;
    if(num <= 0)
        return false;

    if(num >= 3)
    {
       mVertexNum = num >> 1;
    }
    else if(num == 2)
    {
        mVertexNum = 6;
    }
    else if(num == 1)
    {
        mVertexNum = 4;
    }

    mBanners = new Banner[num];
    mPicLocation = location;

    double angle = (2*M_PI/num);
    float zPosition = (mWidth/2)/tan(angle/2);

    float posTmp[12];

    for(int i=0;i < mBannerNum;i++)
    {
        double curAngle = angle*i;
        float cosine = cos(curAngle);
        float sine = sin(curAngle);

        posTmp[0] = cosine*(-mWidth/2)+sine*zPosition;
        posTmp[1] = mHeight/2;
        posTmp[2] = sine*(-mWidth/2)+zPosition*(1-cosine);
        posTmp[3] = cosine*(-mWidth/2)+sine*zPosition;
        posTmp[4] = -mHeight/2;
        posTmp[5] = sine*(mWidth/2)+zPosition*(1-cosine);
        posTmp[6] = cosine*(-mWidth/2)+sine*zPosition;
        posTmp[7] = -mHeight/2;
        posTmp[8] = sine*(mWidth/2)+zPosition*(1-cosine);
        posTmp[9] = cosine*(mWidth/2)+sine*zPosition;
        posTmp[10] = mHeight/2;
        posTmp[11] = sine*(mWidth/2)+zPosition*(1-cosine);

        mBanners[i].setData(posTmp,mPicLocation[i]);
        mBanners[i].load();
    }

    //updateViewPort();

    return true;
}

void Render::resize(int width,int height)
{
	mWidth = width;
	mHeight = height;
	glViewport(0,0,mWidth,mHeight);
}

void Render::render(void)
{
      const float CAM_X = 0.f;
      const float CAM_Y = 0.f;
      const float CAM_Z = 500.f;

      mat_view_ = Mat4::LookAt( Vec3( CAM_X, CAM_Y, CAM_Z ),
              Vec3( 0.f, 0.f, 0.f ), Vec3( 0.f, 1.f, 0.f ) );

      if(camera)
      {
          camera->Update();
          mat_view_ = camera->GetTransformMatrix() * mat_view_
                  * camera->GetRotationMatrix() * mat_model_;
      }
      else
      {
          mat_view_ = mat_view_ * mat_model_;
      }

      Mat4 mat_vp = mat_projection_ * mat_view_;
	  LOGE("PREPARE TO DRAW!");


      for(int i=0;i<mBannerNum;i++)
      {
    	 LOGE("DRAWING!!!! BANNER");
         mBanners[i].draw(&mat_vp);
      }
}

void Render::updateViewPort(void)
{
    //Init Projection matrices
      int32_t viewport[4];
      glGetIntegerv( GL_VIEWPORT, viewport );
      float fAspect = (float) viewport[2] / (float) viewport[3];

      const float CAM_NEAR = 5.f;
      const float CAM_FAR = 1000.f;
      bool bRotate = false;
      mat_projection_ = Mat4::Perspective( fAspect, 1.f, CAM_NEAR, CAM_FAR );
}
