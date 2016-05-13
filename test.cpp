#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include "CImg.h"
#include "RANSAC.hpp"
#include "DataType.hpp"

using namespace std;
using namespace cimg_library;
using namespace ransac;


const int TOTALPOINTS = 100;
const int inlierCnt = 50;
const int outlierCnt = TOTALPOINTS - inlierCnt;



void testFitting2Dline() {
	CImg<unsigned char> testImg1(500, 500, 1, 3);
	testImg1.fill(255);
	//CImgDisplay displayTestImg1(testImg1, "Fitting 2D line");
	vector<myDot> points(TOTALPOINTS);
	srand(time(0));

	float a = rand() % 10 / 4.0f;
	float b = rand() % 10;
	cout <<"a = " << a << " b = " << b << endl;
	
	printf("groundtruth: (%f, %f)\n", a, b);
	// 点线颜色
	int color[3] = { 0, 255, 255 };

	int imgWidth = testImg1._width;
	int imgHeight = testImg1._height;
	//inliers
	for (int i = 0, x = 10; i < inlierCnt, x < 10 + inlierCnt * 4; i++, x += 4)
	{
		points[i].x = x;
		points[i].y = x * a + b + rand() % 100 / 20.0;
		points[i].y = points[i].y < imgHeight ? points[i].y : imgHeight;
		testImg1.draw_circle(points[i].x, points[i].y, testImg1.width() / 200, color);
	}

	
	//outliers
	for (int i = inlierCnt; i < TOTALPOINTS; i++)
	{
		points[i].x = rand() % imgWidth;
		points[i].y = rand() % imgHeight;
		testImg1.draw_circle(points[i].x, points[i].y, testImg1.width() / 200, color);
	}
	//testImg1.display("Fitting 2D line");



	bool exists = false;
	Line<myDot>* fit;

	RANSAC<myDot, Line<myDot>> ransac;
	/*RANSAC<Circle, Point2d, CircleFromPoints, DistancePointFromCircle, CircleError> r(3, points, 100, 20, 40,
		CircleFromPoints(),
		DistancePointFromCircle(),
		CircleError());*/
	ransac.setData(points);
	ransac.setMinEstimateNum(2);
	ransac.setIterateTimes(1000);
	ransac.setFitWellNum(TOTALPOINTS/5);
	//ransac.setProbabilityForInlier(0.95);
	//ransac.setFitWellNum(10);
	ransac.setThreshold(3);
	fit = ransac.findBestFit();
	assert(fit != NULL);
	//if (exists) {
	cout << endl << endl << "Final K : " << fit->K << " B : " << fit->B << endl;
		int colorLine[3] = { 255, 0, 0 };
		double y1 = (fit->K) * 1 + (fit->B);
		double y2 = (fit->K) * 200 + (fit->B);
		testImg1.draw_line(1, y1, 200, y2, colorLine);
		//circle(img, fit.c, std::round(fit.r), Scalar(255, 0, 0), 1);
	//}
	testImg1.display("Fitting 2D line");

	/*
	点击鼠标获取坐标，并在该坐标上画一个园。
	*/
	//int dataNum;
	//cout << "Please input the number of data points: ";
	//cin >> dataNum;
	//displayTestImg1.wait();
	//int i = 0;
	//// 点线颜色
	//int color[3] = { 0, 255, 255 };
	//// 控制点数组
	//vector<myDot*> vecDotA, vecDotB;
	//while (i < dataNum) {
	//	displayTestImg1.wait();
	//	if (displayTestImg1.button() & displayTestImg1.mouse_x() >= 0 && displayTestImg1.mouse_y() >= 0)
	//	{
	//		i++;
	//		myDot* click = new myDot(displayTestImg1.mouse_x(), displayTestImg1.mouse_y()); // vecDotA的size恰好是新一个点的编号
	//		testImg1.draw_circle(click->x, click->y, testImg1.width() / 90, color);
	//		testImg1.display(displayTestImg1);
	//		vecDotA.push_back(click);
	//	}
	//}
}


int main() {
	testFitting2Dline();
	system("pause");

	return 0;
}
