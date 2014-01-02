#pragma once
#include "support\user_default\CCUserDefault.h"
#include "cocos2d.h"
#include <string>


#define HIGH_SCORE_RECORD_NUMBERS 5

struct ScoreTableRecord
{
	std::string name;
	std::string score;
	std::string rank;
};

using namespace cocos2d;
class JG_Score_Handler :
	public CCFileUtils
{
public:
	JG_Score_Handler(void);
	~JG_Score_Handler(void);

	

	void InsertRecord(std::string name,std::string score,std::string rank);
	void GetHighScoreTable();

};

