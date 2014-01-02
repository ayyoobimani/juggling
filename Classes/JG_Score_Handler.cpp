#include "JG_Score_Handler.h"


JG_Score_Handler::JG_Score_Handler(void)
{

}


JG_Score_Handler::~JG_Score_Handler(void)
{
}

void JG_Score_Handler::InsertRecord(std::string name,std::string score,std::string rank)
{
	std::string tempNameScore=name+" "+score;
	const char* temprank=rank.c_str();
	CCUserDefault::sharedUserDefault()->setStringForKey(temprank,tempNameScore);
	CCUserDefault::sharedUserDefault()->flush();
}

void JG_Score_Handler::GetHighScoreTable()
{
	ScoreTableRecord record;
	//for(int i=1;i<HIGH_SCORE_RECORD_NUMBERS+1;i++)
	//{
		//std::string record=CCUserDefault::sharedUserDefault()->getStringForKey(i);
	//}

}
