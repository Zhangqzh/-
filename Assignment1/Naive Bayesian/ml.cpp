#include"ml.hpp"
NaiveBayes::NaiveBayes() {
	
}
NaiveBayes::NaiveBayes(const char* featureName,const char* lableName, int d, int v) {
	
	numTrainDocs = d;
	numTokens = v;
	trainFeatureMatrix = (this->setDataMatrix(featureName));
	trainLableMatrix = (this->setDataMatrix(lableName));
	trainMatrix = (this->getTrainMatrix(trainFeatureMatrix,numTrainDocs,numTokens));
	this->getPrioProb();
}
MatrixXd  NaiveBayes::getTrainFeatureMatrix() {
	return trainFeatureMatrix;
}
MatrixXd  NaiveBayes::getTrainLableMatrix() {
	return trainLableMatrix;
}

//得到trainMatrix，其中trainMatrix[i][j]=k表示第i封email中单词j出现了k次
MatrixXd NaiveBayes:: getTrainMatrix(MatrixXd fe, int d, int v) {
	vector<vector<int>> f(3);
	MatrixXd m(d, v);
	m.setZero();
	for (int i = 0; i < fe.cols(); i++) {
		for (int j = 0; j < fe.rows(); j++) {
			f[i].push_back(fe(j, i));
		}
	}

	for (int k = 0; k < fe.rows(); k++) {

		m(f[0][k] - 1, f[1][k] - 1) = f[2][k];
	}
	return m;
}
//得到输入文件对应的矩阵
MatrixXd NaiveBayes::setDataMatrix(const char* fileName) {
	int col = this->getFileColumns(fileName);
	int row = this->getFileRows(fileName);
	MatrixXd m(row, col);
	ifstream fileStream;
	fileStream.open(fileName, ios::in);//表示以只读的方式读取文件
	if (fileStream.fail()) {
		cout << "Fail to open!"<<fileName<<endl;
	}
	else {
		while (!fileStream.eof()) {
			for (int i = 0; i < row; i++) {
				for (int j = 0; j < col; j++) {
					fileStream >> m(i, j);
				}
			}
		}
	}
	return m;
}
//得到输入文件的行数
int NaiveBayes::getFileRows(const char* fileName) {
	ifstream fileStream;
	string temp;
	int count = 0;
	fileStream.open(fileName, ios::in);
	if (fileStream.fail()) {
		cout << "Fail to open!"<<fileName<<endl;
		return 0;
	}
	else {
		while (getline(fileStream, temp, '\n')) {
			if (temp.size() > 0) count++;
		}
		fileStream.close();
		return count;
	}

}

//得到输入文件的列数
int NaiveBayes::getFileColumns(const char* fileName) {
	ifstream fileStream;
	fileStream.open(fileName, ios::_Nocreate);
	double tmp = 0;
	int count = 0;//列数计数器
	char c;//当前位置的字符
	if (fileStream.fail()) {
		cout << "Fail to open!"<<fileName<<endl;
		return 0;
	}
	else {
		c = fileStream.peek();

		while (('\n' != c) && (!fileStream.eof())) {//指针指向的当前字符
			fileStream >> tmp;
			count++;
			c = fileStream.peek();
		}
		fileStream.close();
	}
	return count;
}

void NaiveBayes::getPrioProb() {
	vector<int> spamIndex;
	
	for (int i = 0; i < trainLableMatrix.rows(); i++) {
		if (trainLableMatrix(i, 0) == 1)
			spamIndex.push_back(i);
		
	}
	probSpam = (double)spamIndex.size() / numTrainDocs;
	probSpam = log(probSpam);
	MatrixXd v(trainMatrix.cols(), 1);
	MatrixXd eLength(trainMatrix.rows(), 1);
	MatrixXd Tcx(trainMatrix.cols(), 1);
	MatrixXd nTcx(trainMatrix.cols(), 1);
	v.setConstant(1);
	eLength.setZero();
	Tcx.setZero();
	nTcx.setZero();
	eLength = trainMatrix * v;
	double spamW = 0;
	double nonspamW = 0;
	for (int i = eLength.rows()-1; i >=0; i--) {
		if (!spamIndex.empty()) {
			if (i == spamIndex.back()) {
				spamW += eLength(i, 0);
				for (int j = 0; j < Tcx.rows(); j++) {
					Tcx(j, 0) += trainMatrix(i, j);
				}
				spamIndex.pop_back();
			}
			else {
				nonspamW += eLength(i, 0);
				for (int j = 0; j < nTcx.rows(); j++) {
					nTcx(j, 0) += trainMatrix(i, j);
				}
			}
		}
	}
	
	MatrixXd prioP(trainMatrix.cols(), 1);
	MatrixXd nprioP(trainMatrix.cols(), 1);
	prioP.setZero();
	nprioP.setZero();
	
	for (int i = 0; i < prioP.rows(); i++) {
		prioP(i, 0) = ((Tcx(i, 0) + 1) / (spamW + numTokens));
		nprioP(i,0)= ((nTcx(i, 0) + 1) / (nonspamW + numTokens));
	}
	
	this->spamPrioP = prioP;
	this->nonspamPrioP = nprioP;
}

MatrixXd NaiveBayes::getTestFeatureMatrix(const char* featureName) {
	return this->setDataMatrix(featureName);
}
MatrixXd NaiveBayes::getTestLableMatrix(const char* lableName) {
	return this->setDataMatrix(lableName);
}
MatrixXd NaiveBayes::getPostProb(const char* featureName, int d, int v) {
	MatrixXd ts = spamPrioP;
	MatrixXd tn = nonspamPrioP;
	MatrixXd test = this->getTrainMatrix(this->getTestFeatureMatrix(featureName), d, v);
	MatrixXd res(test.rows(), 1);
	MatrixXd ren(test.rows(), 1);
	MatrixXd add(test.rows(), 1);
	MatrixXd re(test.rows(), 1);
	res.setZero();
	ren.setZero();
	add.setConstant(probSpam);
	for (int i = 0; i < ts.rows(); i++) {
		ts(i, 0) = log(ts(i, 0));
		tn(i, 0) = log(tn(i, 0));
	}
	res = test * ts;
	ren = test * tn;
	//cout << re;
	res = res + add;
	add.setConstant(1 - probSpam);
	ren = ren + add;
	for (int i = 0; i < re.rows(); i++) {
		re(i, 0) = ((res(i, 0) > ren(i, 0)) ? 1 : 0);
	}
	return re;
}
double NaiveBayes::getResult(const char* featureName, const char* lableName, int d, int v) {
	MatrixXd testLable = this->getTestLableMatrix(lableName);
	//cout << testLable << endl;
	MatrixXd output = this->getPostProb(featureName, d, v);
	MatrixXd wrong(output.rows(), 1);
	for (int i = 0; i < wrong.rows(); i++) {
		wrong(i, 0) = (testLable(i,0)==output(i,0))?0:1;
	}
	return 1 - wrong.sum() / d;
}