#pragma once
#include "JG_Score_Handler.h"
#include <vector>
#include <string>

class JG_Test_ScoreHandler
{
public:


	
	char GetRandomString();


	void TestHighScoreTable();
	void TestInitilizingRecord();
	void TestInsertingRecord();

	void RunTestFunctions();

	JG_Test_ScoreHandler(void);
	~JG_Test_ScoreHandler(void);
};

