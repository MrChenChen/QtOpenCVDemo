#pragma once
#include <QtCore>
#include <QtWidgets>

#include <functional>

#include <opencv.hpp>

#include "Util.h"

#include "Stopwatch.h"

#include <vector>

using namespace cv;

using namespace std;

class MyWidget :public QWidget
{

public:

	MyWidget(QWidget* parent = 0);


	void SetUpUI();


	void InitConnections();



public:

	Mat src;

private:

	QPushButton* btnLoadImg;

	std::function<void(QString, std::function<void()>)> AddButton;

	int m_ButtonCount = 0;

};