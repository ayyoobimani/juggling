#include "JG_Score_Handler.h"


JG_Score_Handler::JG_Score_Handler(void)
{

}


JG_Score_Handler::~JG_Score_Handler(void)
{
}

void JG_Score_Handler::InsertRecord(string name,int score,int rank)
{
	string tempNameScore=name+' '+to_string(score);
	const char* temprank=(to_string(rank)).c_str();
	CCUserDefault::sharedUserDefault()->setStringForKey(temprank,tempNameScore);
	CCUserDefault::sharedUserDefault()->flush();
}

vector<ScoreTableRecord>* JG_Score_Handler::GetHighScoreTable()
{
	ScoreTableRecord recordItem;
	vector<ScoreTableRecord>* highScoreVector=new vector<ScoreTableRecord>(HIGH_SCORE_RECORD_NUMBERS);
	for(int i=1;i<HIGH_SCORE_RECORD_NUMBERS+1;i++)
	{
		const char * tempKey=to_string(i).c_str();
		string record=CCUserDefault::sharedUserDefault()->getStringForKey(tempKey);
		CCLOG("the string record is %s",record);
		if(record.empty())
			break;
		recordItem.rank=i;
		stringstream nameBYscore(record);
		getline(nameBYscore,recordItem.name,' ');
		string tempScore;
		getline(nameBYscore,tempScore,' ');
		recordItem.score=stoi(tempScore);

		//vector initializing
		highScoreVector->push_back(recordItem);
		
	}
	return highScoreVector;

}
