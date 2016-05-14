#ifndef DATATYPE_HPP
#define DATATYPE_HPP

#include <vector>
#include <string>

using std::vector;
using std::string;

/*
  my2DDot is the basic abstraction of point in 2D space.
*/
class my2DDot {
public:
	/*
		Its two member variable x and y represents it position (x,y) in 2D space.
	*/
	float x, y;
	my2DDot(float a, float b) {
		x = a;
		y = b;
	}
	my2DDot(const my2DDot &other) {
		x = other.x;
		y = other.y;
	}
	my2DDot() {
		x = y = 0;
	}
	float dotProduct(const my2DDot& v) const	{
		return x * v.x + y * v.y;
	}
	my2DDot operator - (my2DDot &a) {
		return my2DDot(x - a.x, y - a.y);
	}
	bool operator== (my2DDot &a) {
		if (this->x == a.x && this->y == a.y)
			return true;
		else
			return false;
	}
};

/*
	Use template to implement generalization.
	my2DLine is the class that represent a line in 2D space.
	@para "Point" represent the basic element type of a 2D line -- point.
*/
template<typename Point>
class my2DLine {
public:
	/*
	Use formula "y = Kx + B" to represent a line in 2D space.
	*/
	double K, B;
	/*
	The bool variable verticalToX indicates the situation that the line is vertical to X-axis.
	The formula "y = Kx + B" can not be applied to this situation.
	*/
	bool verticalToX;
	void operator = (my2DLine &other) {
		K = other.K;
		B = other.B;
	}
	my2DLine() : K(0), B(0), verticalToX(false) {}
	/*
		This construction function use the point dataset and use them to construct a line in 2D space.
		@para points is the dataset that are used to fit the model.
		@para fitType is the command that are used to control the fitting method.
			  There are two types in this demo: "leastSquareFit" and "directlyFitModel".
			  "leastSquareFit" is to fit the model using least square method.
			  "directlyFitModel" is to fit the model using minimal number of data.
			  	Such as two points for a line and three points for a circle.
		@return The fitted line object.
	*/
	my2DLine(vector<Point> points, const char* fitType) {
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
			// Fitting a line need exactly two points.
			assert(points.size() == 2);
			// See whether the line is vertical to X-axis.
			if (abs(points[0].x - points[1].x) <= 0.00001) { 
				verticalToX = true;
			}
			else {
				K = (points[0].y - points[1].y) / (points[0].x - points[1].x);
				B = points[0].y - K * points[0].x;
			}
		}
		
	}

	/*
		modelError calculate the accumulated error from current "inliers"
		  and return the accumulated error. It is used to test how good a model(line) is.
		@para points are the "inliers" which are used to calculate
		   their accumulated error/distance from the fitting model.
		@return The accumulated error from all the "inliers" under this model.
	*/
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
	/*
		disFromModel calculate the distance/error of a point from current model(line).
		@para p is the point to be measured.
		@return the error/distance between current model/line to the point.
	*/
	double disFromModel(Point &p) {
		return abs((p.x * K + B) - p.y);
	}
};


/*
	Use template to implement generalization.
	my2DCircle is the class that represent a circle in 2D space.
	@para "Point" represent the basic element type of a 2D circle -- point.
*/

template <typename Point>
class my2DCircle {
public:
	Point center;
	double radius;

	my2DCircle() { }

	my2DCircle(const Point &c, double r) : center(c), radius(r) { }

	my2DCircle(vector<Point> points, const char* fitType) {
		int Size = points.size();
		assert(Size >= 3);
		double X1 = 0;
		double Y1 = 0;
		double X2 = 0;
		double Y2 = 0;
		double X3 = 0;
		double Y3 = 0;
		double X1Y1 = 0;
		double X1Y2 = 0;
		double X2Y1 = 0;
		for (int i = 0; i < Size; i++) {
			X1 += points[i].x;//使用对象数组
			Y1 += points[i].y;
			X2 += points[i].x * points[i].x;
			Y2 += points[i].y * points[i].y;
			X3 += points[i].x * points[i].x * points[i].x;
			Y3 += points[i].y * points[i].y * points[i].y;
			X1Y1 += points[i].x * points[i].y;
			X1Y2 += points[i].x * points[i].y * points[i].y;
			X2Y1 += points[i].x * points[i].x * points[i].y;
		}
		double C, D, E, G, H, N;
		double a, b, c;
		N = Size;
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

		//cout << "x: " << center.x << " y: " << center.y << " radius: " << radius << endl;
	}

	double modelError(vector<Point> &points) {
		double accumulateError = 0;
		int Size = points.size();
		assert(Size > 0);
		double distanceFromCenter;
		for (int i = 0; i < Size; i++) {
			distanceFromCenter = (center.x - points[i].x) * (center.x - points[i].x) + (center.y - points[i].y) * (center.y - points[i].y);
			accumulateError += sqrt(abs(distanceFromCenter - radius*radius));
		}
		return accumulateError;
	}

	double disFromModel(Point &p) {
		double distanceFromCenter = (center.x - p.x) * (center.x - p.x) + (center.y - p.y) * (center.y - p.y);
		double diff = sqrt(abs(distanceFromCenter - radius*radius));
		return diff;
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
