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

		button->setGeometry((m_ButtonCount % 8) * 100 + 10, int(m_ButtonCount / 8) * 40 + 20, 80, 30);

		m_ButtonCount++;


	};

}

void MyWidget::InitConnections()
{

	AddButton("加载图片", []
	{
		Mat img = imread("1_0090_0.bmp", 0);

		imshow("LoadImage", img);

		namedWindow("LoadImage", CV_WINDOW_NORMAL);

	});

	AddButton("二值化", []
	{

		Mat img = imread("1_0090_0.bmp", 0);

		imshow("Before", img);

		threshold(img, img, 200, 255, CV_THRESH_BINARY);

		imshow("After", img);
	});


	AddButton("腐蚀", []
	{
		Mat img = imread("1_0090_0.bmp", 0);

		imshow("Before", img);

		Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));

		erode(img, img, element);

		imshow("After", img);

	});


	AddButton("模糊/滤波", []
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


	AddButton("播放视频", []
	{
		VideoCapture video;

		video.open(0/*R"(D:\Program Files\opencv\sources\samples\data\768x576.avi)"*/); //video.open(0)  打开摄像头

		while (true)
		{
			Mat frame;

			video >> frame;

			if (frame.empty()) return;

			imshow("Play Video", frame);

			waitKey(30);
		}

	});


	AddButton("创建PNG", []
	{
		Mat mat(480, 640, CV_8UC4);


		for (size_t i = 0; i < mat.rows; i++)
		{
			for (size_t j = 0; j < mat.cols; j++)
			{
				auto & rgba = mat.at<Vec4b>(i, j);

				rgba[0] = UCHAR_MAX;

				rgba[1] = uchar(float(mat.cols - j) / float(mat.cols)*UCHAR_MAX);

				rgba[2] = uchar(float(mat.rows - j) / float(mat.rows)*UCHAR_MAX);

				rgba[3] = uchar(0.5*(rgba[1] + rgba[2]));

			}
		}

		imshow("PNG", mat);

		//imwrite("D:\\1.png", mat);

	});


	AddButton("图像混合", []
	{
		Mat img = imread("1_0090_0.bmp", 0);

		imshow("1_0090_0.bmp", img);

		Mat girl = imread("ts.bmp", 0);

		imshow("ts.bmp", girl);

		Mat imageROI = img(Rect(500, 120, girl.cols, girl.rows));

		addWeighted(img(Rect(500, 120, girl.cols, girl.rows)), 0, girl, 1, 0.1, imageROI);

		imshow("After", img);
	});


	AddButton("颜色图像", []
	{
		Mat red(300, 300, CV_8UC3, Scalar(0, 0, 255));

		imshow("red", red);



		Mat gray(300, 300, CV_8UC1, Scalar(128));

		imshow("gray", gray);



		Mat rndcolor(300, 300, CV_8UC3);

		for (size_t i = 0; i < rndcolor.rows; i++)
		{
			for (size_t j = 0; j < rndcolor.cols; j++)
			{
				auto & t = rndcolor.at<Vec3b>(i, j);

				t[0] = qrand() % 255;
				t[1] = qrand() % 255;
				t[2] = qrand() % 255;
			}
		}

		imshow("rndcolor", rndcolor);



		Mat rndgray(300, 300, CV_8UC1);

		for (size_t i = 0; i < rndgray.rows; i++)
		{
			for (size_t j = 0; j < rndgray.cols; j++)
			{
				rndgray.at<uchar>(i, j) = qrand();
			}
		}

		imshow("rndgray", rndgray);



	});



}
