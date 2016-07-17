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
	setFixedSize(870, 600);


	AddButton = [&](QString caption, std::function<void()> _fun)
	{
		auto button = new QPushButton(caption, this);

		connect(button, &QPushButton::clicked, this, _fun);

		button->setGeometry((m_ButtonCount % 8) * 100 + 40, int(m_ButtonCount / 8) * 50 + 20, 80, 30);

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


		//dst = img1[x]*alpha + img2[x]*beta + gama       //输出数组深度是 CV_32S 时不适合 addWeigted
		//addWeighted(img(Rect(500, 120, girl.cols, girl.rows)), 0, girl, 1, 0.1, imageROI);

		girl.copyTo(imageROI);


		imshow("After", img);


	});


	AddButton("颜色图像", []
	{
		Mat red(300, 300, CV_8UC3, Scalar(0, 0, 255));

		imshow("red", red);



		Mat gray(300, 300, CV_8UC1, Scalar(128));

		imshow("gray", gray);



		Mat rndcolor(300, 300, CV_8UC3);

		/*for (size_t i = 0; i < rndcolor.rows; i++)
		{
			for (size_t j = 0; j < rndcolor.cols; j++)
			{
				auto & t = rndcolor.at<Vec3b>(i, j);

				t[0] = qrand() % 255;
				t[1] = qrand() % 255;
				t[2] = qrand() % 255;
			}
		}*/

		randu(rndcolor, Scalar(0, 0, 0), Scalar(255, 255, 255));


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


	AddButton("通道分离", []
	{
		Mat src = imread("ts.bmp", 1);

		imshow("Source", src);

		vector<Mat> channels;

		split(src, channels);

		/*int len = src.cols*src.rows * 3;

		Mat c1;
		Mat c2;
		Mat c3;

		uchar* ptr = src.data;

		for (size_t i = 0; i < len; i += 3)
		{
			c1.push_back<uchar>(ptr[i]);
			c2.push_back<uchar>(ptr[i + 1]);
			c3.push_back<uchar>(ptr[i + 2]);
		}

		channels.push_back(c1);
		channels.push_back(c2);
		channels.push_back(c3);*/


		imshow("B", channels[0]);

		imshow("G", channels[1]);

		imshow("R", channels[2]);

	});


	AddButton("通道和并", []
	{
		Mat src = imread("ts.bmp", 1);

		vector<Mat> channels;

		split(src, channels);

		imshow("B", channels[0]);

		imshow("G", channels[1]);

		imshow("R", channels[2]);

		Mat dst;


		//merge(channels, dst);


		Mat temp = channels[0];

		cvtColor(temp, temp, COLOR_GRAY2BGR);

		uchar* ptr = temp.data;

		int len = channels[0].cols*channels[1].rows * 3;

		int j = 0;

		for (size_t i = 0; i < len; i += 3)
		{
			ptr[i] = channels[0].data[j];
			ptr[i + 1] = channels[1].data[j];
			ptr[i + 2] = channels[2].data[j];
			j++;
		}



		imshow("After Merge", temp);

	});


	AddButton("亮度", []
	{
		Mat src = imread("ts.bmp", 1);

		imshow("Before", src);

		for (size_t i = 0; i < src.rows; i++)
		{
			for (size_t j = 0; j < src.cols; j++)
			{
				src.at<Vec3b>(i, j)[0] = qMax(qMin(src.at<Vec3b>(i, j)[0] - 30, 255), 0);
				src.at<Vec3b>(i, j)[1] = qMax(qMin(src.at<Vec3b>(i, j)[1] - 30, 255), 0);
				src.at<Vec3b>(i, j)[2] = qMax(qMin(src.at<Vec3b>(i, j)[2] - 30, 255), 0);
			}
		}

		imshow("After", src);

	});


	AddButton("对比度", []
	{
		Mat src = imread("ts.bmp", 1);

		imshow("Before", src);

		for (size_t i = 0; i < src.rows; i++)
		{
			for (size_t j = 0; j < src.cols; j++)
			{
				src.at<Vec3b>(i, j)[0] = qMax(qMin(uchar(src.at<Vec3b>(i, j)[0] * 2), (uchar)255), (uchar)0);
				src.at<Vec3b>(i, j)[1] = qMax(qMin(uchar(src.at<Vec3b>(i, j)[1] * 2), (uchar)255), (uchar)0);
				src.at<Vec3b>(i, j)[2] = qMax(qMin(uchar(src.at<Vec3b>(i, j)[2] * 2), (uchar)255), (uchar)0);
			}
		}

		imshow("After", src);

	});


	AddButton("扩充图像", []
	{
		Mat src = imread("ts.bmp", 1);

		imshow("Before", src);

		Mat dst;

		copyMakeBorder(src, dst, 10, 30, 20, 40, BorderTypes::BORDER_CONSTANT, Scalar(0, 0, 255));

		imshow("After", dst);
	});


	AddButton("傅里叶", []
	{
		Mat src = imread("ts.bmp", 0);

		imshow("Source", src);

		//扩大尺寸
		int m = getOptimalDFTSize(src.rows);

		int n = getOptimalDFTSize(src.cols);

		Mat padd;

		copyMakeBorder(src, padd, 0, m - src.rows, 0, n - src.cols, BORDER_CONSTANT, Scalar::all(0));

		//为结果分配空间 两个Mat数组
		Mat planes[] = { Mat_<float>(padd), Mat::zeros(padd.size(),CV_32F) };

		Mat complexI;

		merge(planes, 2, complexI);

		//进行傅里叶变换
		dft(complexI, complexI);

		//将傅里叶变换的结果分解通道
		split(complexI, planes);

		//将复数转化为振幅
		magnitude(planes[0], planes[0], planes[1]);

		Mat magnitudeImage = planes[0];

		//进行对数尺度缩放

		magnitudeImage += Scalar::all(1);

		log(magnitudeImage, magnitudeImage);

		//重新分布图像象限
		magnitudeImage = magnitudeImage(Rect(0, 0, magnitudeImage.cols & -2, magnitudeImage.rows & -2));

		int cx = magnitudeImage.cols / 2;

		int cy = magnitudeImage.rows / 2;

		Mat q0(magnitudeImage, Rect(0, 0, cx, cy)); //左上

		Mat q1(magnitudeImage, Rect(cx, 0, cx, cy)); //右上

		Mat q2(magnitudeImage, Rect(0, cy, cx, cy)); //左下

		Mat q3(magnitudeImage, Rect(cx, cy, cx, cy)); //右下

		Mat temp; //交换左上 和 右下

		q0.copyTo(temp);

		q3.copyTo(q0);

		q1.copyTo(temp);  //交换右上 和 左下

		q2.copyTo(q1);

		temp.copyTo(q2);

		//归一化

		normalize(magnitudeImage, magnitudeImage, 0, 1, NORM_MINMAX);

		imshow("After DFT", magnitudeImage);

	});


	AddButton("Make XML", []
	{
		FileStorage file("D:\\v.xml", FileStorage::WRITE);

		std::vector<Point> vec;

		vec.push_back(Point(1, 2));
		vec.push_back(Point(3, 4));
		vec.push_back(Point(5, 6));


		//Mat img = imread("ts.bmp");

		file << "ts" << 1 << "hah" << "[" << vec << "]";




		file.release();
	});


	AddButton("Read XML", []
	{
		FileStorage file("D:\\v.xml", FileStorage::READ);

		Mat img;

		file["ts"] >> img;

		imshow("ts", img);

	});


	AddButton("方框滤波", []
	{
		Mat img = imread("1_0090_0.bmp", 0);

		imshow("Before", img);

		boxFilter(img, img, -1, Size(5, 5), Point(-1, -1));

		imshow("After", img);

		threshold(img, img, 200, 255, CV_THRESH_BINARY);

		imshow("Thresold Blur", img);

	});


	AddButton("均值滤波", []
	{
		Mat img = imread("1_0090_0.bmp", 0);

		imshow("Before", img);

		Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));

		blur(img, img, Size(7, 7));

		imshow("After", img);

		threshold(img, img, 200, 255, CV_THRESH_BINARY);

		imshow("Thresold Blur", img);

	});



	AddButton("中值滤波", []
	{
		Mat img = imread("ts.bmp", 0);

		imshow("Before", img);


		medianBlur(img, img, 7);

		imshow("After", img);

		threshold(img, img, 200, 255, CV_THRESH_BINARY);

		imshow("Thresold Blur", img);

	});


	AddButton("双边滤波", []
	{
		Mat img = imread("ts.bmp", 0);

		imshow("Before", img);

		Mat out;

		bilateralFilter(img, out, 9, 9 * 2, 9 / 2);

		imshow("After", out);

		threshold(out, out, 200, 255, CV_THRESH_BINARY);

		imshow("Thresold Blur", out);

	});


	AddButton("腐蚀", []
	{
		Mat img = imread("1_0090_0.bmp", 0);

		imshow("Before", img);

		Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));

		erode(img, img, element);

		imshow("After", img);

		threshold(img, img, 200, 255, CV_THRESH_BINARY);

		imshow("Thresold erode", img);
	});


	AddButton("膨胀", []
	{
		Mat img = imread("1_0090_0.bmp", 0);

		imshow("Before", img);

		Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));

		dilate(img, img, element);

		imshow("After", img);

		threshold(img, img, 200, 255, CV_THRESH_BINARY);

		imshow("Thresold dilate", img);

	});


	AddButton("开运算", []
	{
		Mat img = imread("1_0090_0.bmp", 0);

		imshow("Before", img);

		Mat temp;

		Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));

		dilate(img, temp, element);

		erode(temp, temp, element);

		imshow("After", temp);

	});


	AddButton("闭运算", []
	{
		Mat img = imread("1_0090_0.bmp", 0);

		imshow("Before", img);

		Mat temp;

		Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));

		erode(img, temp, element);

		dilate(temp, temp, element);

		imshow("After", temp);

	});


	AddButton("形态学梯度", []
	{
		Mat img = imread("1_0090_0.bmp", 0);

		imshow("Before", img);

		Mat element = getStructuringElement(MORPH_RECT, Size(25, 25));

		morphologyEx(img, img, MORPH_GRADIENT, element);

		imshow("After", img);

	});


	AddButton("顶帽", []
	{
		Mat img = imread("1_0090_0.bmp", 0);

		imshow("Before", img);

		Mat element = getStructuringElement(MORPH_RECT, Size(25, 25));

		morphologyEx(img, img, MORPH_TOPHAT, element);

		imshow("After", img);

	});


	AddButton("黑帽", []
	{
		Mat img = imread("1_0090_0.bmp", 0);

		imshow("Before", img);

		Mat element = getStructuringElement(MORPH_RECT, Size(25, 25));

		morphologyEx(img, img, MORPH_BLACKHAT, element);

		imshow("After", img);

	});


	AddButton("FloodFill", []
	{
		Mat img = imread("1_0090_0.bmp", 1);

		Mat mask(img.rows + 2, img.cols + 2, CV_8U, Scalar::all(0));


		floodFill(img, mask, Point(300, 100), Scalar(0, 0, 255), 0, 0, 0);

		imshow("After", img);
	});


	AddButton("缩放", []
	{
		Mat img = imread("ts.bmp", 1);

		imshow("Before", img);

		Mat dst;

		double scale = 2;

		//cv::resize(img, dst, Size(round(img.cols*scale), round(img.rows*scale)));

		cv::resize(img, dst, Size(), 2, 2, INTER_CUBIC);

		imshow("After", dst);

	});


	AddButton("自动阈值", []
	{
		Mat img = imread("1_0090_0.bmp", 0);

		adaptiveThreshold(img, img, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 3, 5);

		imshow("After", img);
	});


	AddButton("Canny", []
	{

		Mat img = imread("1_0090_0.bmp", 0);

		blur(img, img, Size(10, 10));

		threshold(img, img, 200, 255, CV_THRESH_BINARY);

		imshow("Before Canny", img);

		Mat des;

		des.create(img.size(), img.type());

		Canny(img, des, 3, 9, 3);

		imshow("After Canny", des);

	});


	AddButton("Sobel", []
	{

		Mat img = imread("1_0090_0.bmp", 0);

		blur(img, img, Size(10, 10));

		threshold(img, img, 200, 255, CV_THRESH_BINARY);

		imshow("Before Canny", img);

		Mat des;

		des.create(img.size(), img.type());

		Mat img_x, img_y, abs_img_x, abs_img_y, dst;

		Sobel(img, img_x, img.depth(), 1, 0);

		convertScaleAbs(img_x, abs_img_x);

		Sobel(img, img_y, img.depth(), 0, 1);

		convertScaleAbs(img_x, abs_img_x);

		addWeighted(img_x, 0.5, img_y, 0.5, 0, dst);

		imshow("After Sobel", dst);

	});


	AddButton("Laplacian", []
	{
		Mat img = imread("ts.bmp", 0);

		imshow("Before", img);

		Mat dst;

		Laplacian(img, dst, img.depth());

		imshow("After Laplacian", dst);

		imshow("After Laplacian", img - dst);

	});


	AddButton("霍夫变换", []
	{
		Mat img = imread("1_0090_0.bmp", 0);

		blur(img, img, Size(3, 3));

		threshold(img, img, 200, 255, CV_THRESH_BINARY);

		imshow("Before Canny", img);

		Mat dst;

		dst.create(img.size(), img.type());

		Canny(img, dst, 3, 9, 3);

		vector<Vec2f> lines;

		HoughLines(dst, lines, 1, CV_PI / 180, 150);

 


	});

}
