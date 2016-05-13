#ifndef RANSAC_HPP
#define RANSAC_HPP

#include <random>
#include <set>
#include <cmath>
#include <iostream>
#include <assert.h>
#include "DataType.hpp"


using std::set;
using std::cout;
using std::endl;

/*
Given:
    data - a set of observed data points
    model - a model that can be fitted to data points
    n - the minimum number of data values required to fit the model
    k - the maximum number of iterations allowed in the algorithm
    t - a threshold value for determining when a data point fits a model
    d - the number of close data values required to assert that a model fits well to data
Return:
    bestfit - model parameters which best fit the data (or nil if no good model is found)

iterations = 0
bestfit = nil
besterr = something really large
while (iterations < k) {
    maybeinliers = n randomly selected values from data
    maybemodel = model parameters fitted to maybeinliers
    alsoinliers = empty set
    for every point in data not in maybeinliers {
        if point fits maybemodel with an error smaller than t
             add point to alsoinliers
    }
    if the number of elements in alsoinliers is > d {
        % this implies that we may have found a good model
        % now test how good it is
        bettermodel = model parameters fitted to all points in maybeinliers and alsoinliers
        thiserr = a measure of how well model fits these points
        if thiserr < besterr {
            bestfit = bettermodel
            besterr = thiserr
        }
    }
    increment iterations
}
return bestfit
*/

namespace ransac {
//template<class DataType, class Model, class Threshold, class fitError, class modelError>

template<typename DataType, typename Model>
class RANSAC {
private:
	vector<DataType> data;  // a set of observed data points
	Model* model;  // a model that can be fitted to data points
	int minEstimateNum;  // the minimum number of data values required to fit the model
	int	iterateTimes;  // the maximum number of iterations allowed in the algorithm
	double threshold;  // a threshold value for determining when a data point fits a model (the distance from the model)
	int	fitWellNum;  // the number of close data values required to assert that a model fits well to data
	//FitModelFromData fitModelFromData;
	/*Model (*fitModelFromData)(vector<DataType> &a, bool &b);*/
	double desiredProbability;  // Probability for drawing an outlier free subset.
	double probabilityForInlier; // number of inliers in data / number of points in data
	//Threshold fitError(maybeModel, data[index]);
public:
	/*RANSAC(vector<DataType> &_data, Model &_model, Threshold _threshold, int _minEstimateNum, double _desiredProbability,
		double _probabilityForInlier, FitModelFromData &_fitModelFromData) :
		data(_data), model(_model), threshold(_threshold), minEstimateNum(_minEstimateNum), desiredProbability(_desiredProbability),
		probabilityForInlier(_probabilityForInlier), fitModelFromData(_fitModelFromData)
		{ }*/
	//void compute();
	/*void setModel(Model* _model) {
		model = _model;
	}*/
	RANSAC() {
		data.clear();
		model = NULL;
		minEstimateNum = iterateTimes = fitWellNum = desiredProbability = probabilityForInlier = 0;
	}
	~RANSAC() {
		data.clear();
		model = NULL;
	}
	void initPara() {

	}
	void setData(vector<DataType> &_data) {
		data = _data;
	}
	void setThreshold(double _threshold) {
		threshold = _threshold;
	}
	void setMinEstimateNum(int _minEstimateNum) {
		if (_minEstimateNum <= 0) {
			cout << "Invalid minimal estimate number!" << endl;
		}
		minEstimateNum = _minEstimateNum;
	}
	void setProbabilityForInlier(double _probabilityForInlier) {
		if (p <= 0 || p > 1) {
			cout << "Invalid probability for inlier!" << endl;
		}
		probabilityForInlier = _probabilityForInlier
	}
	/*void setFitModelFromData(Model (*_fitModelFromData)(vector<DataType> &a, bool b)) {
		fitModelFromData = _fitModelFromData;
	}*/
	void setFitWellNum(int _fitWellNum) {
		fitWellNum = _fitWellNum;
	}
	void setIterateTimes(int _iterateTimes) {
		iterateTimes = _iterateTimes;
	}
	void calculateIterationTimes() {
		if (iterateTimes == 0) {
			iterateTimes = std::log2(1 - desiredProbability) / std::log2(1 - pow(probabilityForInlier, minEstimateNum));
		}
	}
	//void set

	/*
	
	*/
	Model* findBestFit() {
		assert(minEstimateNum <= data.size());
		/*if (minEstimateNum > data.size()) {
			
			cout << "Not enough points given!" << endl;
		}*/
		Model* bestFit = NULL; // 注意默认构造函数
		double bestError = 9999999.0f;
		Model maybeModel;
		vector<DataType> maybeInliers;
		int iteration = 0;
		//double w;  // w = number of inliers in data / number of points in data
		calculateIterationTimes();
		// @test
		cout << "iterateTimes = " << iterateTimes << endl;
		while (iteration < iterateTimes) {

			// 生成不相同的随机下标
			set<int> maybeInlierIndices;
			int dataSize = data.size();
			while (maybeInlierIndices.size() < minEstimateNum) {
				int index = std::rand() % dataSize;
				//cout << "index = " << index << endl;
				maybeInlierIndices.insert(index);
			}

			vector<DataType> maybeInliers;
			set<int>::iterator it = maybeInlierIndices.begin();
			for (; it != maybeInlierIndices.end(); it++) {
				maybeInliers.push_back(data[*it]);
			}

			//bool modelExists = true;
			Model* maybeModel = new Model(maybeInliers, "directlyFitModel");

			/*if (!modelExists) {
				cout << "Not enough points to fit the model" << endl;
				return model;
			}*/

			vector<DataType> alsoInliers;
			for (int index = 0; index < data.size(); index++) {
				if (maybeInlierIndices.find(index) == maybeInlierIndices.end()) { // 不在maybeinliers
					if (maybeModel->disFromModel(data[index]) < threshold) {
						alsoInliers.push_back(data[index]);
					}
				}
			}
			//cout << "alsoInliers :" <<alsoInliers.size() << endl;

			vector<DataType> &consensusSet = maybeInliers;
			if (alsoInliers.size() > fitWellNum) {
				for (int i = 0; i < alsoInliers.size(); i++) {
					consensusSet.push_back(alsoInliers[i]);
				}
				Model *betterModel = new Model(consensusSet, "leastSquareFit");

				double error = betterModel->modelError(consensusSet);

				if (error < bestError) {
					bestFit = betterModel;
					bestError = error;
					cout << "K : " << bestFit->K << " B : " << bestFit->B << endl;
					//inliers = maybeInliers;
				}
			}
			
			iteration++;
		}
		return bestFit;
	}


	Model &modelFromData() {

	}
	
};


//template<class Model, class Point, class ModelFromPoints, class DistancePointFromModel, class ModelError>
//class RANSAC {
//public:
//	RANSAC(int n, vector<Point> points, int iterations, double error, int minimum, ModelFromPoints mfp,
//		DistancePointFromModel dpm, ModelError me) :
//		N(n), data(points), k(iterations), t(error), d(minimum), modelFromPoints(mfp),
//		distancePointModel(dpm), modelError(me) {	}
//
//	Model model;
//	vector<Point> data;
//	int k;
//	double t;
//	int d;
//	int N;
//	ModelFromPoints modelFromPoints;
//	DistancePointFromModel distancePointModel;
//	ModelError modelError;
//
//	vector<Point> inliers;
//
//	Model findBestFit() {
//
//		if (N > data.size()) {
//			std::cout << "Not enough points given." << std::endl;
//		}
//
//		int i = 0;
//		Model bestFit;
//		double bestError = std::numeric_limits<double>::max();
//
//		while (i < k) {
//			// 提前生成随机数
//			std::set<int> indices;
//			while (indices.size() < N) {
//				int index = std::rand() % data.size();
//				indices.insert(index);
//			}
//			std::vector<Point> maybeInliers;
//			for (int u : indices) {
//				maybeInliers.push_back(data[u]);
//			}
//			bool exists = true;
//			Model M = modelFromPoints(maybeInliers.begin(), maybeInliers.end(), exists);
//
//			if (!exists) {
//				std::cout << "Not enough points to fit the model";
//				return model;
//			}
//
//			std::vector<Point> alsoInliers;
//			for (int index = 0; index < data.size(); index++) {
//				if (indices.find(index) == indices.end()) {
//					if (distancePointModel(M, data[index]) < t) {
//						alsoInliers.push_back(data[index]);
//					}
//				}
//			}
//
//			if (alsoInliers.size() > d) {
//				for (Point p : alsoInliers) {
//					maybeInliers.push_back(p);
//				}
//
//				Model betterModel = modelFromPoints(maybeInliers.begin(), maybeInliers.end(), exists);
//				if (!exists) {
//					std::cout << "Not enough points to fit the model";
//					return model;
//				}
//
//				double error = modelError(M, maybeInliers.begin(), maybeInliers.end());
//
//				if (error < bestError) {
//					bestFit = betterModel;
//					bestError = error;
//					inliers = maybeInliers;
//				}
//			}
//
//			i++;
//		}
//
//		return bestFit;
//
//	}
//
//	const vector<Point> &getInliers() {
//		return inliers;
//	}
//};
}

#endif
