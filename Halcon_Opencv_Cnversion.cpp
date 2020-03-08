#  include "HalconCpp.h"  
#  include "highgui.hpp"
#  include "ctime"
#  include "vector"

using namespace cv;
using namespace std;
using namespace HalconCpp;

Mat HObject2Mat(HObject Hobj) {//Halocn Image and Opencv Image interconvert
    HTuple htCh = HTuple();
    HTuple cType;
    Mat Image;
    ConvertImageType(Hobj, &Hobj, "byte");
    CountChannels(Hobj, &htCh);
    HTuple wid;
    HTuple hgt;
    int W, H;
    if (htCh[0].I() == 1){
        HTuple ptr;
        GetImagePointer1(Hobj, &ptr, &cType, &wid, &hgt);
        W = (Hlong)wid;
        H = (Hlong)hgt;
        Image.create(H, W, CV_8UC1);
        BYTE * pdata = (BYTE *)ptr.L();
        memcpy(Image.data, pdata, W*H);
    }
    else if (htCh[0].I() == 3)
    {
        HTuple ptrR, ptrG, ptrB;
        GetImagePointer3(Hobj, &ptrR, &ptrG, &ptrB, &cType, &wid, &hgt);
        W = (Hlong)wid;
        H = (Hlong)hgt;
        Image.create(H, W, CV_8UC3);
        vector<Mat> vecM(3);
        vecM[2].create(H, W, CV_8UC1);
        vecM[1].create(H, W, CV_8UC1);
        vecM[0].create(H, W, CV_8UC1);

        BYTE * pbR = (BYTE *)ptrR.L();
        BYTE * pbG = (BYTE *)ptrG.L();
        BYTE * pbB = (BYTE *)ptrB.L();

        memcpy(vecM[2].data, pbR, W*H);
        memcpy(vecM[1].data, pbG, W*H);
        memcpy(vecM[0].data, pbB, W*H);

        merge(vecM, Image);
    }
    return Image;
}