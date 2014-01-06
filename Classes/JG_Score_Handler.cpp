#include "JG_Score_Handler.h"


JG_Score_Handler::JG_Score_Handler(void)
{

}


JG_Score_Handler::~JG_Score_Handler(void)
{
}

void JG_Score_Handler::InsertRecord(CCString name,int score,int rank)
{
	std::string scoreInString=CCString::createWithFormat("%i",score)->getCString();
	std::string rankInString=CCString::createWithFormat("%i",rank)->getCString();
	string tempNameScore=std::string(name.getCString())+std::string(" ")+scoreInString;
	//CCString* tempNameScore=CCString::createWithFormat("%s %i",name.getCString(),score);
	//const char* temprank=CCString::createWithFormat("%i",rank)->getCString();
	CCUserDefault::sharedUserDefault()->setStringForKey(rankInString.c_str(),tempNameScore);
	CCUserDefault::sharedUserDefault()->flush();
}

vector<ScoreTableRecord>* JG_Score_Handler::GetHighScoreTable()
{
	ScoreTableRecord recordItem;
	vector<ScoreTableRecord>* highScoreVector=new vector<ScoreTableRecord>(HIGH_SCORE_RECORD_NUMBERS);
	for(int i=1;i<HIGH_SCORE_RECORD_NUMBERS+1;i++)
	{
		const char * tempKey=CCString::createWithFormat("%i",i)->getCString();
		
		string record=CCUserDefault::sharedUserDefault()->getStringForKey(tempKey);
	//	//CCLOG("the string record is %s",record);
		if(record.empty())
			break;
		recordItem.rank=i;
		

		//TODO:function
		recordItem.name="";
		string scoretempString="";
		int k=0;
		while(record[k]!=' ')
		{
			recordItem.name+=record[k];
			k++;
		}
		
		for(int p=recordItem.name.length()+1;p<record.length();p++)
		{
			scoretempString+=record[p];
		}
		//CCLOG("the name is %s",recordItem.name.c_str());
		//CCLOG("the score is %s",scoretempString.c_str());
		////stringstream nameBYscore(record);
		////getline(nameBYscore,recordItem.name,' ');
		////string tempScore;
		////getline(nameBYscore,tempScore,' ');
		////recordItem.score=stoi(tempScore);

		
		
		CCString temp=scoretempString;
		recordItem.score=temp.intValue();

		////vector initializing
		//CCLOG("the score is %i ***********************************",recordItem.score);
		
		//CCString tempName=recordItem.name;
		
	
		highScoreVector->at(i-1)=recordItem;
		
		
	}
	//return highScoreVector;
	//int high= CCUserDefault::sharedUserDefault()->getIntegerForKey("high");
	
	return highScoreVector;
	
}


