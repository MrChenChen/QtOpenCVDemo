#pragma once
#include <QtCore>
#include <QtWidgets>
#include <vector>
#include <opencv.hpp>

#include "Util.h"

using namespace cv;
using namespace std;

class MyWidget :public QWidget
{
	Q_OBJECT

public:

	MyWidget(QWidget* parent = 0) :QWidget(parent)
	{

		SetUpUI();

		InitConnection();

	}



	void SetUpUI()
	{
		setFixedSize(900, 600);

		btnTest1 = new QPushButton(" Test1 ", this);

		btnTest1->setGeometry(800, 20, 80, 30);


		src = imread("temp_image.bmp", 0);

		SetImage(src);

	}

	void SetImage(cv::Mat & _mat)
	{
		m_image = ASM::cvMatToQImage(_mat).copy();

		update();
	}

	void InitConnection()
	{
		connect(btnTest1, &QPushButton::clicked, this, [&]
		{	 

			vector<vector<Point>> point;

			findContours(src, point, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

			Rect t = boundingRect(point[0]);

			rectangle(src, t, Scalar(255), 1);

			SetImage(src);

			imshow("", src);

		});

	}


protected:


	virtual void paintEvent(QPaintEvent *) override
	{
		QPainter p(this);

		p.drawImage(0, 0, m_image);

		if (m_ismousedown)
		{
			p.setPen(QPen(QColor("#4FB92C"), 2));

			p.drawRect(m_drawrect);
		}
	}


	virtual void mousePressEvent(QMouseEvent *ev) override
	{
		m_mousedown = ev->pos();

		m_ismousedown = true;

		m_drawrect.setX(ev->pos().x());

		m_drawrect.setX(ev->pos().y());

		update();
	}


	virtual void mouseReleaseEvent(QMouseEvent *ev) override
	{
		//m_ismousedown = false;

		update();
	}


	virtual void mouseMoveEvent(QMouseEvent *ev) override
	{
		if (m_ismousedown)
		{
			m_drawrect = QRect(qMin(m_mousedown.x(), ev->pos().x()),
				qMin(m_mousedown.y(), ev->pos().y()),
				qAbs(m_mousedown.x() - ev->pos().x()),
				qAbs(m_mousedown.y() - ev->pos().y()));

			update();
		}
	}

private:

	Mat src;

	QImage m_image;

	QRect m_drawrect;

	QPoint m_mousedown;

	bool m_ismousedown = false;

	QPushButton* btnTest1 = nullptr;

};