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
	
	CCUserDefault::sharedUserDefault()->setStringForKey(rankInString.c_str(),tempNameScore);
	CCUserDefault::sharedUserDefault()->flush();
}

vector<ScoreTableRecord>* JG_Score_Handler::GetHighScoreTable()
{
	vector<ScoreTableRecord>* highScoreVector=new vector<ScoreTableRecord>(HIGH_SCORE_RECORD_NUMBERS);
	for(int i=1;i<HIGH_SCORE_RECORD_NUMBERS+1;i++)
	{
		highScoreVector->at(i-1).name="";
		highScoreVector->at(i-1).rank=i;
		highScoreVector->at(i-1).score=0;
	}
	for(int i=1;i<HIGH_SCORE_RECORD_NUMBERS+1;i++)
	{
		const char * tempKey=CCString::createWithFormat("%i",i)->getCString();
		
		string record=CCUserDefault::sharedUserDefault()->getStringForKey(tempKey);
		if(record.empty())
			break;
		highScoreVector->at(i-1)=InitializingRecord(i,record);
	}

	return highScoreVector;	
}


ScoreTableRecord JG_Score_Handler::InitializingRecord(int rank,string nameWithScore)
{
	ScoreTableRecord recordItem;
	recordItem.rank=rank;
	
	SplitBySpace(nameWithScore,recordItem.name,recordItem.score);

	return recordItem;
		
}

void JG_Score_Handler::SplitBySpace(string input,string& firstPart,int& SecondPart)
{
	firstPart="";
	string SecondPartTemp="";
	int k=0;
	while(input[k]!=' ')
	{
		firstPart+=input[k];
		k++;
	}
	for(int p=firstPart.length()+1;p<input.length();p++)
	{
		SecondPartTemp+=input[p];
	}
	
	CCString temp=SecondPartTemp;
	SecondPart=temp.intValue();
}

std::string  JG_Score_Handler::getWritablePath()
{
	return "";
}

bool JG_Score_Handler::isFileExist(const std::string& strFilePath)
{
	return false;
}
