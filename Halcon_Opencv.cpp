
#  include "HalconCpp.h"  
#  include "HDevThread.h"
#  include "tchar.h"
#  include "core.hpp"
#  include "highgui.hpp"
#  include "ctime"
#  include "vector"
#  include "Halcon_Opencv.h"

using namespace cv;
using namespace std;
using namespace HalconCpp;

HImage img;
//HTuple hv_AcqHandle, hv_DataCodeHandle, hv_DecodedDataStrings, hv_ResultHandles, hv_Length;
//HTuple Row, Col, GenParamName, GenParamValue;
HTuple width, height;
//HObject  or_Image, ho_Image, ho_SymbolXLDs, symbolxlds, Area;
//Mat cv_Image;


void Image() {//读取图像，并输出高宽信息
    ReadImage(&img, "6.png");
    img.GetImageSize(&width, &height);
    cout << "宽:" << width << endl;
    cout << "高:" << height << endl;
}

void OpenCamera() {//打开笔记本自带的摄像头

    HString str;
    HTuple hv_AcqHandle, hv_DataCodeHandle, hv_DecodedDataStrings, hv_ResultHandles, hv_Length;
    HTuple Row, Col, GenParamName, GenParamValue;
    HObject  or_Image, ho_Image, ho_SymbolXLDs, symbolxlds, Area;
    Mat cv_Image;
    double starttime, endtime, counttime;
    char result[600] = {};

    OpenFramegrabber("DirectShow", 1, 1, 0, 0, 0, 0,//使用halcon打开自带的摄像头
        "default", 8, "rgb", -1, "false",
        "default", "[0] ", 0, -1, &hv_AcqHandle);
    CreateDataCode2dModel("QR Code", GenParamName, GenParamValue, &hv_DataCodeHandle);//建立一个二维码解码器
    SetDataCode2dParam(hv_DataCodeHandle, "default_parameters", "maximum_recognition");//将解码器设置为最强解码
    while (true) {
        GrabImageAsync(&or_Image, hv_AcqHandle, -1);//异步获取图像
        starttime = clock();
        Rgb1ToGray(or_Image, &ho_Image); // 将图像灰度化
        FindDataCode2d(ho_Image, &ho_SymbolXLDs, hv_DataCodeHandle,"stop_after_result_num", 1, &hv_ResultHandles, &hv_DecodedDataStrings);//查找图像中的二维码
        TupleLength(hv_DecodedDataStrings, &hv_Length);//返回元组的元素数。
        int a = (int(hv_Length));//转化为int类型

        if (a){//如果说解码成功，hv_DecodedDataStrings就会有内容
            str = hv_DecodedDataStrings[0].S();
            sprintf_s(result, 600, "%s", str.Text());
            SelectObj(ho_SymbolXLDs, &symbolxlds, 1);//从对象元组中选择对象。
            //GetContourXld(symbolxlds, &Row, &Col);//返回XLD轮廓的坐标。
            GenRegionContourXld(symbolxlds, &Area, "margin");//从XLD轮廓创建区域。
            OverpaintRegion(or_Image, Area, (((((HTuple(0).Append(3)).Append(255)).Append(255)).Append(255)).Append(255)), "margin");
        }
        else {
            sprintf_s(result, 600, "%s", "false");
        }

        cv_Image = HObject2Mat(or_Image);
        imshow("Halcon", cv_Image);
        endtime = clock();
        cout << "识别结果:" << result << "\n" 
             <<  "运行时间:" << (endtime-starttime) <<endl;
        if (waitKey(33) >= 0) {
            break;
        }
    }

}

int cvtest() {
    namedWindow("Example5", WINDOW_AUTOSIZE);

    VideoCapture cap;
    cap.open(0);
    //VideoCapture cap(0);这句话可以替代上面两个语句，效果是一致的。
    if (!cap.isOpened())
    {
        std::cerr << "Couldn't open capture." << std::endl;
        return -1;
    }

    Mat frame;
    while (1)
    {
        cap >> frame;
        if (frame.empty()) break;
        imshow("Example5", frame);
        if (waitKey(33) >= 0) break;
    }

    getchar();
    return 0;


}

int main() {
    OpenCamera();
    return 0;
}

