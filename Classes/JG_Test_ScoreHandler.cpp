#include "JG_Test_ScoreHandler.h"


JG_Test_ScoreHandler::JG_Test_ScoreHandler(void)
{
}


JG_Test_ScoreHandler::~JG_Test_ScoreHandler(void)
{
}

static const char alphanum[] =
		"!@#$%^&*"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

void JG_Test_ScoreHandler::TestHighScoreTable()
{
	//table format is like vector<ScoreTableRecord>* 
	JG_Score_Handler* scoreHandler;
	vector<ScoreTableRecord>* highScoreTestVector=new vector<ScoreTableRecord>(HIGH_SCORE_RECORD_NUMBERS);
	highScoreTestVector=scoreHandler->GetHighScoreTable();
	//Testing High Score 
	for(int i=0;i<HIGH_SCORE_RECORD_NUMBERS;i++)
	{
		if(highScoreTestVector->at(i).rank>HIGH_SCORE_RECORD_NUMBERS || highScoreTestVector->at(i).rank<1)
			exit(-1);
	}
	//Testing Score
	for(int i=0;i<HIGH_SCORE_RECORD_NUMBERS;i++)
	{
		if(highScoreTestVector->at(i).score<0)
			exit(-1);
	}
	exit(0);
}

char JG_Test_ScoreHandler::GetRandomString()
{
	int stringLength = sizeof(alphanum) - 1;
	return alphanum[rand() % stringLength];
}

void JG_Test_ScoreHandler::TestInitilizingRecord()
{
	JG_Score_Handler* scoreHandler;
	ScoreTableRecord testingRecord;

	int randomRank=6;

	string randomString=std::to_string(GetRandomString());
	scoreHandler->InitializingRecord(randomRank,randomString);
	TestHighScoreTable();
}
void JG_Test_ScoreHandler::TestInsertingRecord()
{
	JG_Score_Handler* scoreHandler;

	string randomString=std::to_string(GetRandomString());
	int randomScore=CCRANDOM_0_1()*10;
	int randomRank=CCRANDOM_0_1()*10;
	CCString tmp;
	tmp.m_sString=randomString;
	scoreHandler->InsertRecord(randomString,randomScore,randomRank);
	TestHighScoreTable();
}

void JG_Test_ScoreHandler::RunTestFunctions()
{
	//TestHighScoreTable();
	TestInitilizingRecord();
	TestInsertingRecord();

}