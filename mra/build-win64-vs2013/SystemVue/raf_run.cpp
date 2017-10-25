#pragma once
#include "raf_run.h"
#include "SystemVue/ModelBuilder.h"
#include "SystemVue/CircularBuffer.h"
#include "SystemVue/Matrix.h"
#include "SystemVue/MatrixCircularBuffer.h"
#include "SystemVue/DFParam.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

using namespace std;

namespace SystemVueModelBuilder {

#ifndef SV_CODE_RAF_RUN
	DEFINE_MODEL_INTERFACE(raf_run)
	{
		SET_MODEL_NAME("RA_F_run");
		SET_MODEL_DESCRIPTION("");
		SET_MODEL_CATEGORY("Regressioin_Analysis");
		ADD_MODEL_HEADER_FILE("raf_run.h");
		model.SetModelCodeGenName("raf_run");
		model.SetModelNamespace("SystemVueModelBuilder");

		// Add parameters
		SystemVueModelBuilder::DFParam param = NULL;
		param = ADD_MODEL_PARAM(INPUT_MODEL);
		param.SetParamAsFile();

		// Add input/output ports
		DFPort IN_N = ADD_MODEL_INPUT(IN_ELEMENT);
		IN_N.SetDescription("Input the number of elements");
		DFPort IN_M = ADD_MODEL_INPUT(IN_ATTRIBUTE);
		IN_M.SetDescription("Input the number of attributes");
		DFPort IN_MFILE = ADD_MODEL_INPUT(IN_DATA);
		IN_MFILE.SetDescription("Input the model");
		


		DFPort OutputPort = ADD_MODEL_OUTPUT(Output);
		OutputPort.SetDescription("Output");

		return true;
	}
#endif

	raf_run::raf_run()
	{
	}

	raf_run::~raf_run()
	{
	}

	bool raf_run::Setup()
	{
		IN_ELEMENT.SetRate(1);
		IN_ATTRIBUTE.SetRate(1);
		IN_DATA.SetRate(1);
		Output.SetRate(1);

		return true;
	};

	bool raf_run::Initialize()
	{
		return true;
	}
	
	bool raf_run::Run()
	{
		int n=IN_ELEMENT[0],dim=IN_ATTRIBUTE[0];
		
		double* Answer;
		Answer = (double*)malloc(sizeof(double) * n);
		FILE* read = fopen(INPUT_MODEL, "r");
		int cnt = 0;
		double red;
		while (fscanf(read,"%lf",&red) != EOF) ++cnt;
		double* Model;
		Model = (double*)malloc(sizeof(double) * cnt);
		fclose(read);
		read = fopen(INPUT_MODEL, "r");
		for (int i = 0; i < cnt; i++) fscanf(read, "%lf",Model+i);
		fclose(read);

		in_data = IN_DATA[0];

		out_tmp.Resize(n, 1);
		out_tmp.Zero();
		for (int i = 0; i < n; i++)
		{
			double tmp = *Model;
			for (int j = 1; j <= dim; j++)
			{
				tmp += in_data(i,j-1)* *(Model+j);
			}
			out_tmp(i, 0) = tmp;
		}
		Output[0] = out_tmp;
		
		free(Answer);
		free(Model);
		
		
		return true;
	}

	bool raf_run::Finalize()
	{
		return true;
	}

}