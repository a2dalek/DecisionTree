#include<bits/stdc++.h>
using namespace std;

const int numberOfAttributes = 5;
const int numberOfLabel = 2;
const string pathToTrainSet = "train.csv";
const string pathtoPrivateTestSet = "test.csv";

vector<double>* valueSetForAttribute[numberOfAttributes];

long long Rand(long long l, long long h)
{
    return l + ((long long)rand() * (RAND_MAX + 1) * (RAND_MAX + 1) * (RAND_MAX + 1) +
                (long long)rand() * (RAND_MAX + 1) * (RAND_MAX + 1) +
                (long long)rand() * (RAND_MAX + 1) +
                rand()) % (h - l + 1);
}

struct Data {
	double attributes[numberOfAttributes];
	int label;

	Data(int* s, int label_) {
		for (int i=0;i<numberOfAttributes;i++) {
			attributes[i]= s[i];
		}
		label = label_;
	}

	Data(int valueSurrived, int valPclass, int valSex, double valAge, int valSibsp,	int valParch, int valEmbakerd) {
		attributes[0] = valPclass;
		attributes[1] = valSex;
		attributes[2]=valAge;
		if (valAge==-1) attributes[2] = Rand(2,3);
		else if (valAge<=10) attributes[2] = 1;
		else if (valAge<60) attributes[2] = 2;
		else attributes[2] = 3;
		attributes[3] = valSibsp;
		attributes[4] = valParch;
		attributes[5] = valEmbakerd;
		//attributes[5] = ((valSex == 1 && valAge<=20) || (valAge>=60));
		label = valueSurrived;
	}

	void print() {
		cout<<label;
		for (int i=0;i<numberOfAttributes;i++) {
			cout<<","<<attributes[i];
		}
		cout<<endl;
	}
};


struct Node {
	int chosenAttribute;
	double chosenValue;
	int lable;
	int depth;
	Node* Left;
	Node* Right;

	Node(int _depth) {
		depth = _depth;
	}

	void setNode(int _chosenAttribute, double _chosenValue, int _lable, Node* _left, Node* _right) {
		chosenAttribute = _chosenAttribute;
		chosenValue = _chosenValue;
		lable = _lable;
		Left = _left;
		Right = _right;
	}

	void print() {
		for (int i=1;i<=depth;i++) cout<<"     ";
		cout<<"attribute "<<chosenAttribute<<", value "<<chosenValue<<endl;
		if (Left!=nullptr) Left->print();
		if (Right!=nullptr) Right->print();
	}
};

struct Tree {
	Node* root;
	Tree() {
		root = new Node(0);
	}

	void print() {
		root->print();
		cout<<endl;
	}
};

int toInt(string s) {
	if (s=="") return -1;
	int ret=0;
	for (int i=0;i<s.length();i++) ret=ret*10+s[i]-'0';
	return ret;
}

double toDouble(string s) {
	if (s=="") return -1;
	double ret=0.0;
	int i=0;
	for (i=0;i<s.length();i++) {
		if (s[i]=='.') break;
		ret=ret*10+(s[i]-'0');
	}
	i++;
	for (double tmp=1;i<s.length();i++) {
		tmp=tmp/10;
		ret+=tmp*(s[i]-'0');
	}
	return ret;
}

vector<Data*>* readFromFile(string path) {
	vector<Data*>* ret = new vector<Data*>();
	string s;
	string Surrived;
	string PassengerId;
	string Pclass;
	string Name;
	string Sex;
	string Age;
	string Sibsp;
	string Parch;
	string Ticket;
	string Fare;
	string Cabin;
	string Embarked;
    ifstream inFile;
    inFile.open(path);
    if (!inFile) {
        cout << "Unable to open file";
        exit(1);
    }

    getline ( inFile, PassengerId, ',' );
	getline ( inFile, Surrived, ',' );
	getline ( inFile, Pclass, ',' );
	getline ( inFile, Name, ',' );
	getline ( inFile, Sex, ',' );
	getline ( inFile, Age, ',' );
	getline ( inFile, Sibsp, ',' );
	getline ( inFile, Parch, ',' );
	getline ( inFile, Ticket, ',' );
	getline ( inFile, Fare, ',' );
	getline ( inFile, Cabin, ',' );
	getline ( inFile, Embarked, '\n' );

    while (!inFile.eof()) {
		string tmpName;
		Name="";
		getline ( inFile, PassengerId, ',' );
		getline ( inFile, Surrived, ',' );
		getline ( inFile, Pclass, ',' );
		getline ( inFile, tmpName, ',' );
		Name += tmpName;
		Name += ", ";
		getline ( inFile, tmpName, ',' );
		Name += tmpName;
		getline ( inFile, Sex, ',' );
		getline ( inFile, Age, ',' );
		getline ( inFile, Sibsp, ',' );
		getline ( inFile, Parch, ',' );
		getline ( inFile, Ticket, ',' );
		getline ( inFile, Fare, ',' );
		getline ( inFile, Cabin, ',' );
		getline ( inFile, Embarked, '\n' );
		//inFile >> Embarked;
		int valueSurrived = toInt(Surrived);
		int valPclass = toInt(Pclass);
		int valSex = (Sex=="female");
		double valAge = toDouble(Age);
		int valSibsp = toInt(Sibsp);
		int valParch = toInt(Parch);
		int valEmbarked = (Embarked == "C");

		Data* tmp = new Data(valueSurrived, valPclass, valSex, valAge, valSibsp, valParch, valEmbarked);
		ret->push_back(tmp);
	}

	ret->pop_back();

    inFile.close();
    return ret;
}

int detectLabel(vector<Data*>* dataSet) {
	int cnt[numberOfLabel];
	memset(cnt,0,sizeof cnt);
	for (int i=0;i<dataSet->size();i++) {
		cnt[dataSet->at(i)->label]++;
	}
	int ret = 0;
	for (int i=1;i<numberOfLabel;i++) {
		if (cnt[i]>cnt[ret]) {
			ret=i;
		}
	}
	return ret;
}

double calculateGiniScore(vector<Data*>* dataSet) {
	int cnt[numberOfLabel];
	memset(cnt,0,sizeof cnt);
	for (int i=0;i<dataSet->size();i++) {
		cnt[dataSet->at(i)->label]++;
	}
	double ret = 1.0;
	for (int i=0;i<numberOfLabel;i++) {
		double tmp = (double)cnt[i] / dataSet->size();
		ret -= tmp*tmp;
	}
	return ret;
}

double calculateEntropy(vector<Data*>* dataSet) {
	int cnt[numberOfLabel];
	memset(cnt,0,sizeof cnt);
	for (int i=0;i<dataSet->size();i++) {
		cnt[dataSet->at(i)->label]++;
	}
	double ret = 1.0;
	for (int i=0;i<numberOfLabel;i++) if (cnt[i]!=0) {
		double tmp = (double)cnt[i] / dataSet->size();
		ret -= tmp*log2(tmp);
	}
	return ret;
}

double calculateAccuracy(vector<int>* predictLabels, vector<Data*>* dataSet) {
	int numTrue=0;
	for (int i=0;i<dataSet->size();i++) {
		numTrue += (predictLabels->at(i) == dataSet->at(i)->label);
	}
	return (double) numTrue/dataSet->size();
}

vector<vector<int> > constructConfusionMatrix(vector<int>* predictLabels, vector<Data*>* dataSet) {
	vector<vector<int> > ret(numberOfLabel, vector<int>(numberOfLabel,0));
	for (int i=0;i<dataSet->size();i++) {
		ret[predictLabels->at(i)][dataSet->at(i)->label]++;
	}
	return ret;
}

void calculateF1Score(vector<vector<int> > confusionMatrix) {
	for (int label=0;label<numberOfLabel;label++) {

		int precisionDenominator =0;
		for (int i=0;i<numberOfLabel;i++){
			precisionDenominator+=confusionMatrix[label][i];
		}
		double precision = (double) confusionMatrix[label][label]/precisionDenominator;

		int recallDenominator =0;
		for (int i=0;i<numberOfLabel;i++){
			recallDenominator+=confusionMatrix[i][label];
		}
		double recall = (double) confusionMatrix[label][label]/recallDenominator;

		double F1Score;
		if (confusionMatrix[label][label]==0) F1Score=0;
		else F1Score = (precision*recall*2)/(precision+recall);

		cout<<"F-1 score of label "<<label<<" is "<<F1Score<<endl;
	}
}

void split(Node* node, vector<Data*>* dataSet, int maxDepth, int minSize) {
	if ((node->depth >= maxDepth) || (dataSet->size() <= minSize)) {
		node->setNode(-1, -1, detectLabel(dataSet), nullptr, nullptr);
		if (node->depth != 0) {
			dataSet->clear();
			delete dataSet;
		}
		return;
	}

	vector<Data*>* bestLeft = nullptr;
	vector<Data*>* bestRight = nullptr;
	int bestAttribute = -1;
	int bestValue = -1;
	double bestScore = 2;
	for (int choosingAttribute=0; choosingAttribute<numberOfAttributes; choosingAttribute++) {
		for (int j=0;j<valueSetForAttribute[choosingAttribute]->size();j++) {
			double choosingValue = valueSetForAttribute[choosingAttribute]->at(j);
		//for (int j=0;j<dataSet->size();j++) {
			//int choosingValue = dataSet->at(j)->attributes[choosingAttribute];
			vector<Data*>* Left = new vector<Data*>;
			vector<Data*>* Right = new vector<Data*>;
			for (int id=0;id<dataSet->size();id++) {
				if (dataSet->at(id)->attributes[choosingAttribute] == -1) continue;
				if (dataSet->at(id)->attributes[choosingAttribute] <= choosingValue) {
					Left->push_back(dataSet->at(id));
				} else {
					Right->push_back(dataSet->at(id));
				}
			}

			if (Left->empty() || Right->empty()) {
				delete Left;
				delete Right;
				continue;
			}

			double ratioLeft = (double)Left->size() / dataSet->size();
			double nowScore = ratioLeft*calculateGiniScore(Left) + (1-ratioLeft)*calculateGiniScore(Right);

			if (nowScore < bestScore) {
				bestScore = nowScore;
				if (bestLeft != nullptr) {
					bestLeft->clear();
					delete bestLeft;
				}
				if (bestRight != nullptr) {
					bestRight->clear();
					delete bestRight;
				}
				bestLeft = Left;
				bestRight = Right;
				bestAttribute = choosingAttribute;
				bestValue = choosingValue;
			} else {
				delete Left;
				Left=nullptr;
				delete Right;
				Right=nullptr;
			}
		}
	}

	if (bestLeft == nullptr || bestRight == nullptr) {
		node->setNode(-1, -1, detectLabel(dataSet), nullptr, nullptr);
		if (node->depth != 0) {
			dataSet->clear();
			delete dataSet;
		}
		return;
	}

	node->setNode(bestAttribute, bestValue, -1, new Node(node->depth + 1), new Node(node->depth + 1));
	if (node->depth != 0) {
		dataSet->clear();
		delete dataSet;
	}

	split(node->Left, bestLeft, maxDepth, minSize);
	split(node->Right, bestRight, maxDepth, minSize);

}

void delNode(Node* node) {
	if (node==nullptr) return;
	if (node->lable != -1) {
		delete node;
		node = nullptr;
		return;
	}
	delNode(node->Left);
	delNode(node->Right);
	delete node;
	node = nullptr;
	return;
}

void delTree(Tree* tree) {
	delNode(tree->root);
	delete tree;
	tree = nullptr;
}

Tree* buildTree(vector<Data*>* dataSet, int maxDepth, int minSize) {
	Tree* decisionTree = new Tree();
	split(decisionTree->root, dataSet, maxDepth, minSize);
	return decisionTree;
}

int doPredict(Data* currentData, Node* node) {
	if (node->lable != -1) return node->lable;
	if (currentData->attributes[node->chosenAttribute] <= node->chosenValue) {
		return doPredict(currentData, node->Left);
	}
	return doPredict(currentData, node->Right);
}

vector<int>* getPredictVector(vector<Data*>* dataSet, Tree* tree) {
	vector<int>* ret = new vector<int>;
	for (int i=0;i<dataSet->size();i++) ret->push_back(doPredict(dataSet->at(i), tree->root));
	return ret;
}

Tree* buildBestTree(vector<Data*>* trainSet, vector<Data*>* validSet) {
	Tree* bestTree = nullptr;
	double bestAccuracy = 0.0;
	for (int maxDepth=1; maxDepth<=10;maxDepth++) {
		for (int minSize=1; minSize<=10;minSize++) {

			Tree* currentTree = buildTree(trainSet, maxDepth, minSize);

			vector<int>* predictLabel = getPredictVector(validSet, currentTree);

			double accuracy = calculateAccuracy(predictLabel, validSet);
			if (bestAccuracy < accuracy) {
				bestAccuracy = accuracy;
				if (bestTree != nullptr) {
					delTree(bestTree);
				}
				bestTree = currentTree;
			} else {
				delTree(currentTree);
			}

			predictLabel->clear();
			delete predictLabel;
			predictLabel=nullptr;
		}
	}
	return bestTree;
}

void writeToFile(string path, vector<int>* predict) {
	ofstream outData;
	outData.open(path);
	outData<<"PassengerId"<<","<<"Survived"<<endl;
	for (int i=892;i<892+predict->size();i++) {
		outData<<i<<","<<predict->at(i-892)<<endl;
	}
}

void randomForrest(int ntree, vector<Data*>* trainSet, vector<Data*>* validSet, vector<Data*>* testSet, vector<Data*>* privateSet) {
	vector<vector<int> > predictLabelsForTestSet(testSet->size(), vector<int>(numberOfLabel, 0));
	vector<vector<int> > predictLabelsForValidSet(trainSet->size(), vector<int>(numberOfLabel, 0));
	vector<vector<int> > predictLabelsForPrivateSet(privateSet->size(), vector<int>(numberOfLabel, 0));

	vector<Data*> *vectorLabel[numberOfLabel];
	for (int i=0;i<numberOfLabel;i++) vectorLabel[i] = new vector<Data*>;
	for (int i=0;i<trainSet->size();i++) {
		vectorLabel[trainSet->at(i)->label]->push_back(trainSet->at(i));
	}

	vector<int> attributelId;
	for (int i=0;i<numberOfAttributes;i++) attributelId.push_back(i);

	for (int i=0;i<ntree;i++) {
		random_shuffle(attributelId.begin(), attributelId.end());
		int nattribute = Rand(numberOfAttributes-3,numberOfAttributes);
		vector<Data*>* currentDataSet = new vector<Data*>;
		for (int label=0;label<numberOfLabel;label++) {
			int num;
			if (label==0) num=550; else num=450;
			for (int j=0;j<num;j++) {
				int dataId = Rand(0, vectorLabel[label]->size()-1);
				int labelValue[numberOfLabel];
				memset(labelValue,-1,sizeof labelValue);
				for (int k=0;k<nattribute;k++) labelValue[attributelId[k]] = vectorLabel[label]->at(dataId)->attributes[attributelId[k]];
				Data* tmp = new Data(labelValue, label);
				currentDataSet->push_back(tmp);
			}
		}

		Tree* currentTree = buildBestTree(currentDataSet, validSet);

		vector<int>* currentPredictForTestSet = getPredictVector(testSet, currentTree);
		vector<int>* currentPredictForValidSet = getPredictVector(validSet, currentTree);
		vector<int>* currentPredictForPrivateSet = getPredictVector(privateSet, currentTree);

		for (int i=0;i<currentPredictForTestSet->size();i++) {
			predictLabelsForTestSet[i][currentPredictForTestSet->at(i)]++;
		}

		for (int i=0;i<currentPredictForValidSet->size();i++) {
			predictLabelsForValidSet[i][currentPredictForValidSet->at(i)]++;
		}

		for (int i=0;i<currentPredictForPrivateSet->size();i++) {
			predictLabelsForPrivateSet[i][currentPredictForPrivateSet->at(i)]++;
		}

		currentPredictForTestSet->clear();
		currentPredictForValidSet->clear();
		currentPredictForPrivateSet->clear();
		currentPredictForTestSet=nullptr;
		currentPredictForValidSet=nullptr;
		currentPredictForPrivateSet=nullptr;


		for (int j=currentDataSet->size()-1;j>=0;j--) {
			delete currentDataSet->at(j);
			currentDataSet->at(j) = nullptr;
			currentDataSet->pop_back();
		}
		delete currentDataSet;
		currentDataSet = nullptr;

		delTree(currentTree);
	}

	cout<<"Result for valid set:"<<endl;
	vector<int>* lastPredictForValidSet = new vector<int>;
	for (int i=0;i<validSet->size();i++) {
		int bestLabel = 0;
		for (int j=1;j<numberOfLabel;j++) if (predictLabelsForValidSet[i][bestLabel] < predictLabelsForValidSet[i][j]) {
			bestLabel = j;
		}

		lastPredictForValidSet->push_back(bestLabel);
	}

	double accuracy = calculateAccuracy(lastPredictForValidSet, validSet);
	cout<<"Accuracy is "<<accuracy<<endl;

	vector<vector<int> > confusionMatrix = constructConfusionMatrix(lastPredictForValidSet, validSet);
	calculateF1Score(confusionMatrix);

	vector<int>* lastPredictForTestSet = new vector<int>;
	for (int i=0;i<testSet->size();i++) {
		int bestLabel = 0;
		for (int j=1;j<numberOfLabel;j++) if (predictLabelsForTestSet[i][bestLabel] < predictLabelsForTestSet[i][j]) {
			bestLabel = j;
		}

		lastPredictForTestSet->push_back(bestLabel);
	}

	cout<<endl<<"Result for test set:"<<endl;
	accuracy = calculateAccuracy(lastPredictForTestSet, testSet);
	cout<<"Accuracy is "<<accuracy<<endl;

	confusionMatrix = constructConfusionMatrix(lastPredictForTestSet, testSet);
	calculateF1Score(confusionMatrix);

	vector<int>* lastPredictForPrivateSet = new vector<int>;
	for (int i=0;i<privateSet->size();i++) {
		int bestLabel = 0;
		for (int j=1;j<numberOfLabel;j++) if (predictLabelsForPrivateSet[i][bestLabel] < predictLabelsForPrivateSet[i][j]) {
			bestLabel = j;
		}

		lastPredictForPrivateSet->push_back(bestLabel);
	}

	writeToFile("predict.csv", lastPredictForPrivateSet);
}

vector<Data*>* spilitTrainSet(vector<Data*>* trainSet) {
	vector<Data*>* validSet = new vector<Data*>;
	vector<Data*>* vectorLabel[numberOfLabel];

	for (int i=0;i<numberOfLabel;i++) vectorLabel[i] = new vector<Data*>;
	for (int i=0;i<trainSet->size();i++) {
		vectorLabel[trainSet->at(i)->label]->push_back(trainSet->at(i));
	}
	trainSet->clear();

	for (int label=0;label<numberOfLabel;label++) {
		for (int i=0;i<vectorLabel[label]->size();i++) {
			if (i<=vectorLabel[label]->size()/5*4 - 1) {
				trainSet->push_back(vectorLabel[label]->at(i));
			} else {
				validSet->push_back(vectorLabel[label]->at(i));
			}
		}
	}

	return validSet;
}

void constructValueSetForAttribute(vector<Data*>* dataSet) {
	for (int i=0;i<numberOfAttributes;i++) {
		set<double> s;
		for (int j=0;j<dataSet->size();j++) {
			s.insert(dataSet->at(j)->attributes[i]);
		}
		valueSetForAttribute[i] = new vector<double>(s.begin(),s.end());
	}
}

int main() {
	srand(time(NULL));
	vector<Data*>* trainSet = readFromFile(pathToTrainSet);
	random_shuffle(trainSet->begin(), trainSet->end());
	constructValueSetForAttribute(trainSet);
	vector<Data*>* testSet = spilitTrainSet(trainSet);
	vector<Data*>* validSet = spilitTrainSet(trainSet);
	vector<Data*>* privateSet = readFromFile(pathtoPrivateTestSet);
	//vector<Data*>* testSet = readFromFile(pathToTestSet);
	//Tree* decisionTree = buildBestTree(trainSet, testSet);
	randomForrest(50, trainSet, validSet, testSet, privateSet);
}
