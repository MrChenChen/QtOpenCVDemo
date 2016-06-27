#pragma once
#include <QtCore>
#include <QtWidgets>

#include <opencv.hpp>

#include "Util.h"

using namespace cv;


class MyWidget :public QWidget
{

public:

	MyWidget(QWidget* parent = 0) :QWidget(parent)
	{

		SetUpUI();

	}

	virtual ~MyWidget()
	{
		 
	};

	void SetUpUI()
	{
		setFixedSize(900, 600);

		QPushButton* btn = new QPushButton(" Test ", this);

		btn->setGeometry(800, 20, 80, 30);


		src = imread(R"(C:\Users\Taylor Swift\Desktop\ÖáÏò\ÖáÏò_50807_x86_test_st2_int\to220_saveall_0120\visualtech\bin\Release\temp_image.bmp)", 0);

		SetImage(src);

	}

	void SetImage(cv::Mat & _mat)
	{
		m_image = ASM::cvMatToQImage(_mat);

		update();
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

};