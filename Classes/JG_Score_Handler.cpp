#include "JG_Score_Handler.h"


JG_Score_Handler::JG_Score_Handler(void)
{

}


JG_Score_Handler::~JG_Score_Handler(void)
{
}

void JG_Score_Handler::InsertRecord(CCString name,int score,int rank)
{
	//string tempNameScore=name+' '+to_string(score);
	//CCString* tempNameScore=CCString::createWithFormat("%s %i",name.getCString(),score);
	//const char* temprank=CCString::createWithFormat("%i",rank)->getCString();
	CCUserDefault::sharedUserDefault()->setIntegerForKey("high",121212);
	CCUserDefault::sharedUserDefault()->flush();
}

int JG_Score_Handler::GetHighScoreTable()
{
	//ScoreTableRecord recordItem;
	//vector<ScoreTableRecord>* highScoreVector=new vector<ScoreTableRecord>(HIGH_SCORE_RECORD_NUMBERS);
	//for(int i=1;i<HIGH_SCORE_RECORD_NUMBERS+1;i++)
	//{
	//	const char * tempKey=CCString::createWithFormat("%i",i)->getCString();
	//	
	//	string record=CCUserDefault::sharedUserDefault()->getStringForKey("1");
	//	//CCLOG("the string record is %s",record);
	//	if(record.empty())
	//		break;
		//recordItem.rank=i;
		////stringstream nameBYscore(record);
		////getline(nameBYscore,recordItem.name,' ');
		////string tempScore;
		////getline(nameBYscore,tempScore,' ');
		////recordItem.score=stoi(tempScore);

		//recordItem.name=record;
		//recordItem.score=1000;


		////vector initializing
		//highScoreVector->push_back(recordItem);
		
	//}
	//return highScoreVector;
	int high= CCUserDefault::sharedUserDefault()->getIntegerForKey("high");
	return high;
}
