#pragma once
#include "support\user_default\CCUserDefault.h"
#include "cocos2d.h"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

using namespace std;

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



	
	

	void InsertRecord(string name,int score,int rank);
	vector<ScoreTableRecord>* GetHighScoreTable();

};

