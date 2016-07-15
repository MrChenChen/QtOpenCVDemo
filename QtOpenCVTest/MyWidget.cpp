#if _MSC_VER >= 1600 
#pragma execution_character_set("utf-8") 
#endif

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "MyWidget.h"



MyWidget::MyWidget(QWidget* parent) :QWidget(parent)
{

	SetUpUI();

	InitConnections();

}


void MyWidget::SetUpUI()
{
	setFixedSize(900, 600);


	AddButton = [&](QString caption, std::function<void()> _fun)
	{
		auto button = new QPushButton(caption, this);

		connect(button, &QPushButton::clicked, this, _fun);

		button->setGeometry((m_ButtonCount % 8) * 100 + 10, int(m_ButtonCount == 0 ? 0 : log10(m_ButtonCount)) * 30 + 20, 80, 30);

		m_ButtonCount++;



	};

}

void MyWidget::InitConnections()
{

	AddButton("����ͼƬ", []
	{
		Mat img = imread("1_0090_0.bmp", 0);

		imshow("LoadImage", img);

	});

	AddButton("��ֵ��", []
	{

		Mat img = imread("1_0090_0.bmp", 0);

		imshow("Before", img);

		threshold(img, img, 200, 255, CV_THRESH_BINARY);

		imshow("After", img);
	});


	AddButton("��ʴ", []
	{
		Mat img = imread("1_0090_0.bmp", 0);

		imshow("Before", img);

		Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));

		erode(img, img, element);

		imshow("After", img);

	});


	AddButton("ģ��/�˲�", []
	{
		Mat img = imread("1_0090_0.bmp", 0);

		Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));

		blur(img, img, Size(7, 7));

		imshow("After", img);

		threshold(img, img, 200, 255, CV_THRESH_BINARY);

		imshow("Thresold Blur", img);

	});


	AddButton("Canny", []
	{

		Mat img = imread("1_0090_0.bmp", 0);

		threshold(img, img, 200, 255, CV_THRESH_BINARY);

		imshow("Before Canny", img);

		Mat des;

		des.create(img.size(), img.type());

		Canny(img, des, 3, 9, 3);

		imshow("After Canny", des);


	});


	AddButton("������Ƶ", [] 
	{
		VideoCapture video;

		video.open(0/*R"(D:\Program Files\opencv\sources\samples\data\768x576.avi)"*/); //video.open(0)  ������ͷ

		while (true)
		{
			Mat frame;

			video >> frame;

			if (frame.empty()) return;

			imshow("Play Video", frame);

			waitKey(30);
		}

	});


}
