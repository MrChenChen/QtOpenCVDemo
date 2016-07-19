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
	setFixedSize(860, 600);


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

#if 0


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


	AddButton("霍夫直线", []
	{
		Mat img = imread("1_0090_0.bmp", 0);

		blur(img, img, Size(3, 3));

		threshold(img, img, 200, 255, CV_THRESH_BINARY);

		imshow("Before Canny", img);

		Mat dst;

		dst.create(img.size(), img.type());

		Canny(img, dst, 3, 9, 3);

		vector<Vec4i> lines;

		HoughLinesP(dst, lines, 1, CV_PI / 180, 50);

		cvtColor(dst, dst, CV_GRAY2BGR);

		for (size_t i = 0; i < lines.size(); i++)
		{
			auto l = lines[i];

			line(dst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255));

		}

		imshow("Result", dst);

	});


	AddButton("霍夫圆", []
	{
		Mat img = imread("vision.bmp", 0);

		GaussianBlur(img, img, Size(3, 3), 2, 2);

		threshold(img, img, 145, 255, THRESH_BINARY);

		vector<Vec3f> lines;

		HoughCircles(img, lines, HOUGH_GRADIENT, 1.5, 10, 100, 100);

		cvtColor(img, img, CV_GRAY2BGR);

		for (size_t i = 0; i < lines.size(); i++)
		{
			Point center(lines[i][0], lines[i][1]);

			int radius = lines[i][2];

			circle(img, center, radius, Scalar(0, 0, 255));
		}

		imshow("Result", img);

	});


	AddButton("均衡化", []
	{
		Mat img = imread("ts.bmp", 0);

		imshow("Before", img);

		Mat dst;

		equalizeHist(img, dst);

		imshow("After", dst);

	});


	AddButton("查找轮廓", []
	{
		Mat img = imread("vision.bmp", 0)(Rect(339, 279, 314, 315));

		imshow("Before", img);

		Mat dst = img.clone();

		//blur(img, img, Size(5, 5));

		threshold(img, img, 90, 255, THRESH_BINARY_INV);

		//imshow("Thre", img);

		vector<Mat> lines;

		findContours(img, lines, RETR_CCOMP, CHAIN_APPROX_NONE);


		vector<Mat> lines_copy;

		vector<Rect> lines_rect;

		for each (auto item in lines)
		{
			auto t_rect = boundingRect(item);

			auto area = t_rect.area();

			if (area > 50 * 50 && area < 120 * 120)
			{
				lines_copy.push_back(item);

				t_rect = Rect(t_rect.x - 5, t_rect.y - 5, t_rect.width + 8, t_rect.height + 8);

				lines_rect.push_back(t_rect);
			}
		}

		cvtColor(dst, dst, CV_GRAY2BGR);

		for each (auto item in lines_rect)
		{
			rectangle(dst, item, Scalar(0, 255, 0), 2);
		}


		imshow("After", dst);

	});


	AddButton("寻找凸包", []
	{
		Mat img(400, 600, CV_8U, Scalar(0));

		RNG & rng = theRNG();


		int count = rng.next() % 100 + 3;

		vector<Point> points;

		for (size_t i = 0; i < count; i++)
		{
			Point p;

			p.x = rng.uniform(img.cols / 4, img.cols / 4 * 3);
			p.y = rng.uniform(img.rows / 4, img.rows / 4 * 3);

			points.push_back(p);
		}

		/*Mat img(points);*/

		vector<int> hull;

		convexHull(points, hull, true);

		cvtColor(img, img, CV_GRAY2BGR);

		for each (auto item in points)
		{
			circle(img, item, 1, Scalar(0, 0, 255));
		}

		for (size_t i = 0; i < hull.size() - 1; i++)
		{
			line(img, points[hull[i]], points[hull[i + 1]], Scalar(0, 255, 0));
		}

		line(img, points[hull[hull.size() - 1]], points[hull[0]], Scalar(0, 255, 0));


		imshow(QString("%1").arg(count).toStdString(), img);


	});


	AddButton("外接矩形", []
	{
		Mat img(400, 600, CV_8U, Scalar(0));

		RNG & rng = theRNG();


		int count = rng.next() % 100 + 3;

		vector<Point> points;

		for (size_t i = 0; i < count; i++)
		{
			Point p;

			p.x = rng.uniform(img.cols / 4, img.cols / 4 * 3);
			p.y = rng.uniform(img.rows / 4, img.rows / 4 * 3);

			points.push_back(p);
		}

		cvtColor(img, img, CV_GRAY2BGR);

		for each (auto item in points)
		{
			circle(img, item, 1, Scalar(0, 0, 255));
		}

		///

		auto rect = boundingRect(points);

		rectangle(img, rect, Scalar(0, 255, 0));

		imshow(QString("%1").arg(count).toStdString(), img);

	});


	AddButton("外接旋转矩形", []
	{
		Mat img(400, 600, CV_8U, Scalar(0));

		RNG & rng = theRNG();


		int count = rng.next() % 100 + 3;

		vector<Point> points;

		for (size_t i = 0; i < count; i++)
		{
			Point p;

			p.x = rng.uniform(img.cols / 4, img.cols / 4 * 3);
			p.y = rng.uniform(img.rows / 4, img.rows / 4 * 3);

			points.push_back(p);
		}

		cvtColor(img, img, CV_GRAY2BGR);

		for each (auto item in points)
		{
			circle(img, item, 1, Scalar(0, 0, 255));
		}

		///

		Point2f rect[4];

		minAreaRect(points).points(rect);

		for (size_t i = 0; i < 4; i++)
		{
			line(img, rect[i], rect[(i + 1) % 4], Scalar(0, 255, 0), 1, LINE_AA);
		}


		imshow(QString("%1").arg(count).toStdString(), img);

	});


	AddButton("外接圆形", []
	{
		Mat img(400, 600, CV_8U, Scalar(0));

		RNG & rng = theRNG();


		int count = rng.next() % 100 + 3;

		vector<Point> points;

		for (size_t i = 0; i < count; i++)
		{
			Point p;

			p.x = rng.uniform(img.cols / 4, img.cols / 4 * 3);
			p.y = rng.uniform(img.rows / 4, img.rows / 4 * 3);

			points.push_back(p);
		}

		cvtColor(img, img, CV_GRAY2BGR);

		for each (auto item in points)
		{
			circle(img, item, 1, Scalar(0, 0, 255));
		}

		///

		Point2f center;

		float radius;

		minEnclosingCircle(points, center, radius);

		circle(img, center, radius, Scalar(0, 255, 0));

		imshow(QString("%1").arg(count).toStdString(), img);

	});


	AddButton("外接三角", []
	{
		Mat img(400, 600, CV_8U, Scalar(0));

		RNG & rng = theRNG();


		int count = rng.next() % 100 + 3;

		vector<Point> points;

		for (size_t i = 0; i < count; i++)
		{
			Point p;

			p.x = rng.uniform(img.cols / 4, img.cols / 4 * 3);
			p.y = rng.uniform(img.rows / 4, img.rows / 4 * 3);

			points.push_back(p);
		}

		cvtColor(img, img, CV_GRAY2BGR);

		for each (auto item in points)
		{
			circle(img, item, 1, Scalar(0, 0, 255));
		}

		///

		vector<Point2f> center;

		float radius;

		minEnclosingTriangle(points, center);


		for (size_t i = 0; i < center.size(); i++)
		{
			line(img, center[i], center[(i + 1) % center.size()], Scalar(0, 255, 0), 1, LINE_AA);
		}


		imshow(QString("%1").arg(count).toStdString(), img);

	});


	AddButton("拟合椭圆", []
	{
		Mat img(400, 600, CV_8U, Scalar(0));

		RNG & rng = theRNG();


		vector<Point> points;

		for (size_t i = 0; i < 6; i++)
		{
			Point p;

			p.x = rng.uniform(img.cols / 4, img.cols / 4 * 3);
			p.y = rng.uniform(img.rows / 4, img.rows / 4 * 3);

			points.push_back(p);
		}

		cvtColor(img, img, CV_GRAY2BGR);

		for each (auto item in points)
		{
			circle(img, item, 1, Scalar(0, 0, 255));
		}

		///
		Point2f rect[4];

		fitEllipse(points).points(rect);

		for (size_t i = 0; i < 4; i++)
		{
			line(img, rect[i], rect[(i + 1) % 4], Scalar(0, 255, 0), 1, LINE_AA);
		}

		imshow("", img);

	});


	AddButton("矩的计算", []
	{
		Mat img = imread("ts.bmp", 0);

		auto re = moments(img);

	});


	AddButton("轮廓面积", []
	{
		Mat img(100, 200, CV_8U, Scalar::all(0));

		img.at<uchar>(30, 60) = 255;
		img.at<uchar>(60, 60) = 255;
		img.at<uchar>(60, 90) = 255;
		img.at<uchar>(90, 30) = 255;

		vector<Point> points = { {30,60},{60,60},{60,90},{90,30} };

		qDebug() << contourArea(points);

	});



	AddButton("轮廓长度", []
	{
		Mat img(100, 200, CV_8U, Scalar::all(0));

		img.at<uchar>(30, 60) = 255;
		img.at<uchar>(60, 60) = 255;
		img.at<uchar>(60, 90) = 255;
		img.at<uchar>(90, 30) = 255;

		vector<Point> points = { {30,60},{60,60},{60,90},{90,30} };

		qDebug() << arcLength(points, true);

	});


	AddButton("直方图", []
	{
		Mat img = imread("ts.bmp", 1);

		Mat dstHist_r, dstHist_g, dstHist_b;


		int dims = 1;

		float hranges[] = { 0,255 };

		const float * ranges[] = { hranges };

		int size = 256;



		int channels = 0;

		calcHist(&img, 1, &channels, Mat(), dstHist_b, dims, &size, ranges);


		channels = 1;

		calcHist(&img, 1, &channels, Mat(), dstHist_g, dims, &size, ranges);


		channels = 2;

		calcHist(&img, 1, &channels, Mat(), dstHist_r, dims, &size, ranges);


		double max_r;

		double max_g;

		double max_b;

		minMaxLoc(dstHist_b, 0, &max_r);
		minMaxLoc(dstHist_b, 0, &max_g);
		minMaxLoc(dstHist_b, 0, &max_b);

		int max = qMax(max_b, qMax(max_g, max_r));

		Mat dst(max / 10 + 100, 255 * 6, CV_8UC3, Scalar::all(0));


		for (size_t i = 0; i < 255; i++)
		{
			dst.at<Vec3b>(dstHist_r.at<float>(i) / 10, 2 * (i + 1) - 1) = Vec3b(0, 0, 255);

			dst.at<Vec3b>(dstHist_g.at<float>(i) / 10, 2 * (i + 1) - 1 + 255 * 2) = Vec3b(0, 255, 0);

			dst.at<Vec3b>(dstHist_b.at<float>(i) / 10, 2 * (i + 1) - 1 + 255 * 4) = Vec3b(255, 0, 0);
		}

		flip(dst, dst, 0);

		cv::imshow("R G B", dst);

	});


	AddButton("直方比较", []
	{
		Mat m1 = imread("ts.bmp", 0);

		int dims = 1;

		float hranges[] = { 0,255 };

		const float * ranges[] = { hranges };

		int size = 256;

		int channels = 0;

		Mat his1;

		calcHist(&m1, 1, &channels, Mat(), his1, dims, &size, ranges);


		Mat m2 = imread("D:\\ts.bmp", 0);

		Mat his2;

		calcHist(&m2, 1, &channels, Mat(), his2, dims, &size, ranges);

		double r = compareHist(his1, his2, HistCompMethods::HISTCMP_CHISQR);

		MessageBox(0, LPCWSTR(QString("%1").arg(r).unicode()), L"", 0);

	});


#endif


	AddButton("Harris", []
	{
		Mat img = imread("1_0090_0.bmp", 0);

		threshold(img, img, 200, 255, THRESH_BINARY_INV);

		cv::imshow("Thre", img);

		Mat dst;

		cornerHarris(img, dst, 2, 3, 0.01);

		cv::normalize(dst, dst, 0, 255, NORM_MINMAX, CV_32FC1);

		//threshold(dst, dst, 0.00001, 255, THRESH_BINARY); //会呈现出明朗的线条

		convertScaleAbs(dst, dst);   //将深度转化为 CV_8U 

		cv::imshow("After", dst);

	});


	AddButton("Tomasi", []
	{
		Mat img = imread("chess.jpg", 0);

		threshold(img, img, 200, 255, THRESH_BINARY_INV);


		vector<Point2f> corners;

		goodFeaturesToTrack(img, corners, 50, 0.01, 10);


		for (size_t i = 0; i < corners.size(); i++)
		{
			qDebug() << corners[i].x;

			qDebug() << corners[i].y;

			auto str = QString("%1").arg(corners[i].x) + " , " + QString("%1").arg(corners[i].y);

			//MessageBox(0, LPCWSTR(str.unicode()), L"像素点坐标", 0);
		}


		Size winSize = Size(5, 5);

		Size zeroZone = Size(-1, -1);

		auto criteria = TermCriteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 40, 0.01);

		cornerSubPix(img, corners, winSize, zeroZone, criteria);


		qDebug() << "---------------------";

		cv::cvtColor(img, img, CV_GRAY2BGR);

		for (size_t i = 0; i < corners.size(); i++)
		{
			qDebug() << corners[i].x;

			qDebug() << corners[i].y;

			auto str = QString("%1").arg(corners[i].x) + " , " + QString("%1").arg(corners[i].y);

			//MessageBox(0, LPCWSTR(str.unicode()), L"亚像素点坐标", 0);

			cv::circle(img, corners[i], 4, Scalar(0, 0, 255), 2);

		}

		cv::imshow("Result", img);

	});


	AddButton("Chess", []
	{
		Mat img = imread("chess.jpg", 0);

		vector<Point2f> corners;

		findChessboardCorners(img, Size(5, 4), corners);

		cv::cvtColor(img, img, CV_GRAY2BGR);

		drawChessboardCorners(img, Size(5, 4), corners, true);

		cv::imshow("After", img);

	});


	AddButton("多边形测试", []
	{
		// 创建一个图形     
		const int r = 100;
		Mat src = Mat::zeros(Size(4 * r, 4 * r), CV_8UC1);

		/// 绘制一系列点创建一个轮廓:
		vector<Point2f> vert(6);

		vert[0] = Point(1.5*r, 1.34*r);
		vert[1] = Point(1 * r, 2 * r);
		vert[2] = Point(1.5*r, 2.866*r);
		vert[3] = Point(2.5*r, 2.866*r);
		vert[4] = Point(3 * r, 2 * r);
		vert[5] = Point(2.5*r, 1.34*r);

		/// 在src内部绘制
		for (int j = 0; j < 6; j++)
		{
			line(src, vert[j], vert[(j + 1) % 6], Scalar(255), 3, 8);
		}

		/// 得到轮廓
		vector<vector<Point> > contours; vector<Vec4i> hierarchy;
		Mat src_copy = src.clone();

		findContours(src_copy, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

		/// 计算到轮廓的距离
		Mat raw_dist(src.size(), CV_32FC1);

		for (int j = 0; j < src.rows; j++)
		{
			for (int i = 0; i < src.cols; i++)
			{
				raw_dist.at<float>(j, i) = pointPolygonTest(contours[0], Point2f(i, j), true);
			}
		}

		double minVal; double maxVal;
		cv::minMaxLoc(raw_dist, &minVal, &maxVal, 0, 0, Mat());
		minVal = abs(minVal); maxVal = abs(maxVal);

		/// 图形化的显示距离
		Mat drawing = Mat::zeros(src.size(), CV_8UC3);

		for (int j = 0; j < src.rows; j++)
		{
			for (int i = 0; i < src.cols; i++)
			{
				if (raw_dist.at<float>(j, i) < 0)
				{
					drawing.at<Vec3b>(j, i)[0] = 255 - (int)abs(raw_dist.at<float>(j, i)) * 255 / minVal;
				}
				else if (raw_dist.at<float>(j, i) > 0)
				{
					drawing.at<Vec3b>(j, i)[2] = 255 - (int)raw_dist.at<float>(j, i) * 255 / maxVal;
				}
				else
				{
					drawing.at<Vec3b>(j, i)[0] = 255; drawing.at<Vec3b>(j, i)[1] = 255; drawing.at<Vec3b>(j, i)[2] = 255;
				}
			}
		}

		/// 创建窗口显示结果
		char* source_window = "Source";
		namedWindow(source_window, CV_WINDOW_AUTOSIZE);
		cv::imshow(source_window, src);
		namedWindow("Distance", CV_WINDOW_AUTOSIZE);
		cv::imshow("Distance", drawing);

		waitKey(0);
		return(0);
	});

}
