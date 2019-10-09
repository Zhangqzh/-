#include"ml.hpp"

int main()
{



	NaiveBayes test("DataPrepared/train-features-50.txt", "DataPrepared/train-labels-50.txt",50, 2500);
	/*cout<<test.getFeatureMatrix()<<endl;
	cout<<test.getLableMatrix()<<endl;*/
	//cout << test.getTestFeatureMatrix("DataPrepared/test-features-5.txt") << endl;
	//cout << test.getTestLableMatrix("DataPrepared/test-labels-5.txt") << endl;
	//test.getPostProb("DataPrepared/test-features-5.txt",  6,10);
	cout<<test.getResult("DataPrepared/test-features.txt", "DataPrepared/test-labels.txt", 260, 2500)<<endl;
	//cout << test.get();

}