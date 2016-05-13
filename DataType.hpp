#ifndef DATATYPE_HPP
#define DATATYPE_HPP

#include <vector>
#include <string>

using std::vector;
using std::string;

class myDot {
public:
	float x;
	float y;
	myDot(float a, float b) {
		x = a;
		y = b;
	}
	myDot(const myDot &other) {
		x = other.x;
		y = other.y;
	}
	myDot() {
		x = y = 0;
	}
	/*float dotProduct(const myDot& v) const	{
		return x * v.x + y * v.y;
	}
	myDot operator - (myDot &a) {
		return myDot(x - a.x, y - a.y);
	}
	bool operator== (myDot &a) {
		if (this->x == a.x && this->y == a.y)
			return true;
		else
			return false;
	}*/
};
template<typename Point>
class Line {
public:
	/*
	y = Kx + B;
	*/
	double K, B;
	bool verticalToX;
	void operator = (Line &other) {
		K = other.K;
		B = other.B;
	}
	Line() : K(0), B(0), verticalToX(false) {}
	Line(vector<Point> points, const char* fitType) {
		assert(points.size() >= 2);
		if (fitType == "leastSquareFit") {
			double t1 = 0, t2 = 0, t3 = 0, t4 = 0;
			int Size = points.size();
			for (int i = 0; i < Size; ++i)
			{
				t1 += points[i].x * points[i].x;
				t2 += points[i].x;
				t3 += points[i].x * points[i].y;
				t4 += points[i].y;
			}
			if (abs(t1 * Size - t2 * t2) <= 0.00001) {
				verticalToX = true;
			}
			else {
				K = (t3 * Size - t2 * t4) / (t1 * Size - t2 * t2);
				B = (t1 * t4 - t2 * t3) / (t1 * Size - t2 * t2);
			}
		}
		else if (fitType == "directlyFitModel") {
			assert(points.size() == 2);
			if (abs(points[0].x - points[1].x) <= 0.00001) { // Vertical to X-axis
				verticalToX = true;
			}
			else {
				K = (points[0].y - points[1].y) / (points[0].x - points[1].x);
				B = points[0].y - K * points[0].x;
			}
		}
		
	}
	double modelError(vector<Point> &points) {
		double accumulateError = 0;
		int Size = points.size();
		assert(Size > 0);
		double modelY;
		for (int i = 0; i < Size; i++) {
			modelY = K*(points[i].x) + B;
			accumulateError += abs(modelY - points[i].y);
		}
		return accumulateError;
	}
	double disFromModel(Point p) {
		return abs((p.x * K + B) - p.y);
	}
};


template <typename Point>
class Circle {
public:
	myDot center;
	double radius;

	Circle() { }

	Circle(const myDot &c, double r) : center(c), radius(r) { }

	Circle(vector<Point> points, const char* fitType) {
		assert(points.size() >= 3);
		double X1 = 0;
		double Y1 = 0;
		double X2 = 0;
		double Y2 = 0;
		double X3 = 0;
		double Y3 = 0;
		double X1Y1 = 0;
		double X1Y2 = 0;
		double X2Y1 = 0;
		for (i = 0; i < m_nNum; i++) {
			X1 = X1 + points[i].x;//使用对象数组
			Y1 = Y1 + points[i].y;
			X2 = X2 + points[i].x*points[i].x;
			Y2 = Y2 + points[i].y*points[i].y;
			X3 = X3 + points[i].x*points[i].x*points[i].x;
			Y3 = Y3 + points[i].y*points[i].y*points[i].y;
			X1Y1 = X1Y1 + points[i].x*points[i].y;
			X1Y2 = X1Y2 + points[i].x*points[i].y*points[i].y;
			X2Y1 = X2Y1 + points[i].x*points[i].x*points[i].y;
		}
		double C, D, E, G, H, N;
		double a, b, c;
		N = m_nNum;
		C = N*X2 - X1*X1;
		D = N*X1Y1 - X1*Y1;
		E = N*X3 + N*X1Y2 - (X2 + Y2)*X1;
		G = N*Y2 - Y1*Y1;
		H = N*X2Y1 + N*Y3 - (X2 + Y2)*Y1;
		a = (H*D - E*G) / (C*G - D*D);
		b = (H*C - E*D) / (D*D - G*C);
		c = -(a*X1 + b*Y1 + X2 + Y2) / N;
		double A, B, R;
		A = a / (-2);
		B = b / (-2);
		R = sqrt(a*a + b*b - 4 * c) / 2;
		center.x = A;
		center.y = B;
		radius = R;
		



		/*long N = std::distance(begin, end);
		if (N < 3) {
			exists = false;
		}

		Point M = std::accumulate(begin, end, Point2d(0.0, 0.0));
		M = (1.0 / N) * M;
		Point2d A(M);

		Point2d p;
		double Suu = 0, Suv = 0, Svv = 0, Suuu = 0, Suvv = 0, Svvv = 0, Svuu = 0;
		int i = 0;
		for (Iterator it = begin; it != end; it++) {
			p = *it - M;
			double uu = p.x * p.x, vv = p.y * p.y;
			Suu += uu;
			Svv += vv;
			Suv += p.x * p.y;
			Suuu += uu * p.x;
			Svvv += vv * p.y;
			Suvv += p.x * vv;
			Svuu += p.y * uu;
		}

		double alpha = 0.5 * (Suuu + Suvv), beta = 0.5 * (Svvv + Svuu),
			delta = Suu * Svv - Suv * Suv,
			deltaX = alpha * Svv - beta * Suv, deltaY = beta * Suu - alpha * Suv;

		if (delta == 0) {
			exists = false;
			return;
		}

		Point2d u(deltaX / delta, deltaY / delta);
		r = std::sqrt((u.x * u.x) + (u.y * u.y) + ((Suu + Svv) / N));
		c = u + M;

		exists = true;*/
	}

	/*template<class Iterator>
	double error(Iterator begin, Iterator end) {
		double error = 0;
		for (Iterator it = begin; it != end; it++) {
			error += std::pow(cv::norm(c - *it) - r, 2);
		}

		return error;
	}

	double distance(Point2d p) {
		return std::pow(cv::norm(c - p) - r, 2);
	}*/
};


#endif
