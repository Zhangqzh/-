#include <iostream>  
#include<fstream>
#include <Eigen/Dense>  
#include <Eigen/Dense>
#include<string>
#include<vector>
#include<cmath>
using Eigen::MatrixXd;

using namespace std;

class NaiveBayes {
private:

	int numTrainDocs;
	int numTokens;
	double probSpam;
	MatrixXd trainFeatureMatrix;
	MatrixXd trainLableMatrix;
	MatrixXd trainMatrix;
	MatrixXd spamPrioP;
	MatrixXd nonspamPrioP;

	int getFileRows(const char* fileName);
	int getFileColumns(const char* fileName);
	MatrixXd setDataMatrix(const char* fileName);

public:
	NaiveBayes();
	NaiveBayes(const char* featureName, const char* lableName,int d, int v);

	MatrixXd getTrainFeatureMatrix();
	MatrixXd getTrainLableMatrix();
	MatrixXd getTrainMatrix(MatrixXd fe,int d,int v);

	MatrixXd getTestFeatureMatrix(const char* featureName);
	MatrixXd getTestLableMatrix(const char* lableName);
	
	void getPrioProb();
	MatrixXd getPostProb(const char* featureName, int d, int v);
	double getResult(const char* featureName, const char* lableName, int d, int v);
	
};
