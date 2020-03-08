
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


void Image() {//��ȡͼ�񣬲�����߿���Ϣ
    ReadImage(&img, "6.png");
    img.GetImageSize(&width, &height);
    cout << "��:" << width << endl;
    cout << "��:" << height << endl;
}

void OpenCamera() {//�򿪱ʼǱ��Դ�������ͷ

    HString str;
    HTuple hv_AcqHandle, hv_DataCodeHandle, hv_DecodedDataStrings, hv_ResultHandles, hv_Length;
    HTuple Row, Col, GenParamName, GenParamValue;
    HObject  or_Image, ho_Image, ho_SymbolXLDs, symbolxlds, Area;
    Mat cv_Image;
    double starttime, endtime, counttime;
    char result[600] = {};

    OpenFramegrabber("DirectShow", 1, 1, 0, 0, 0, 0,//ʹ��halcon���Դ�������ͷ
        "default", 8, "rgb", -1, "false",
        "default", "[0] ", 0, -1, &hv_AcqHandle);
    CreateDataCode2dModel("QR Code", GenParamName, GenParamValue, &hv_DataCodeHandle);//����һ����ά�������
    SetDataCode2dParam(hv_DataCodeHandle, "default_parameters", "maximum_recognition");//������������Ϊ��ǿ����
    while (true) {
        GrabImageAsync(&or_Image, hv_AcqHandle, -1);//�첽��ȡͼ��
        starttime = clock();
        Rgb1ToGray(or_Image, &ho_Image); // ��ͼ��ҶȻ�
        FindDataCode2d(ho_Image, &ho_SymbolXLDs, hv_DataCodeHandle,"stop_after_result_num", 1, &hv_ResultHandles, &hv_DecodedDataStrings);//����ͼ���еĶ�ά��
        TupleLength(hv_DecodedDataStrings, &hv_Length);//����Ԫ���Ԫ������
        int a = (int(hv_Length));//ת��Ϊint����

        if (a){//���˵����ɹ���hv_DecodedDataStrings�ͻ�������
            str = hv_DecodedDataStrings[0].S();
            sprintf_s(result, 600, "%s", str.Text());
            SelectObj(ho_SymbolXLDs, &symbolxlds, 1);//�Ӷ���Ԫ����ѡ�����
            //GetContourXld(symbolxlds, &Row, &Col);//����XLD���������ꡣ
            GenRegionContourXld(symbolxlds, &Area, "margin");//��XLD������������
            OverpaintRegion(or_Image, Area, (((((HTuple(0).Append(3)).Append(255)).Append(255)).Append(255)).Append(255)), "margin");
        }
        else {
            sprintf_s(result, 600, "%s", "false");
        }

        cv_Image = HObject2Mat(or_Image);
        imshow("Halcon", cv_Image);
        endtime = clock();
        cout << "ʶ����:" << result << "\n" 
             <<  "����ʱ��:" << (endtime-starttime) <<endl;
        if (waitKey(33) >= 0) {
            break;
        }
    }

}

int cvtest() {
    namedWindow("Example5", WINDOW_AUTOSIZE);

    VideoCapture cap;
    cap.open(0);
    //VideoCapture cap(0);��仰�����������������䣬Ч����һ�µġ�
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

