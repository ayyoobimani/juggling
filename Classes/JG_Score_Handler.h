#pragma once
#include "support\user_default\CCUserDefault.h"
#include "cocos2d.h"

#include <vector>
#include <string>
#include <sstream>
#include <iostream>


using namespace std;
using namespace cocos2d;

#define HIGH_SCORE_RECORD_NUMBERS 5

struct ScoreTableRecord
{
	string name;
	int score;
	int rank;
};

using namespace cocos2d;
class JG_Score_Handler :
	public CCFileUtils
{
public:
	JG_Score_Handler(void);
	~JG_Score_Handler(void);

	void InsertRecord(CCString name,int score,int rank);
	vector<ScoreTableRecord>* GetHighScoreTable();
	ScoreTableRecord InitializingRecord(int rank,string nameWithScore);
	void SplitBySpace(string input,string& firstPart,int& SecondPart);

	virtual std::string getWritablePath();
    virtual bool isFileExist(const std::string& strFilePath);

};

