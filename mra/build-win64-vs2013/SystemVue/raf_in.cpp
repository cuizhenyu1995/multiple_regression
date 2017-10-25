#include "raf_in.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

namespace SystemVueModelBuilder {

#ifndef SV_CODE_RAF_IN
	DEFINE_MODEL_INTERFACE(raf_in)
	{
		SET_MODEL_NAME("RA_F_in");
		SET_MODEL_DESCRIPTION("");
		SET_MODEL_CATEGORY("Regressioin_Analysis");
		ADD_MODEL_HEADER_FILE("raf_in.h");
		model.SetModelCodeGenName("raf_in");
		model.SetModelNamespace("SystemVueModelBuilder");

		// Add parameters
		SystemVueModelBuilder::DFParam param = NULL;
		param = ADD_MODEL_PARAM(NUMBER_OF_ELEMENT);
		param.SetDefaultValue(0);
		param = ADD_MODEL_PARAM(NUMBER_OF_ATTRIBUTE);
		param.SetDefaultValue(0);
		param = ADD_MODEL_PARAM(INPUT_DATA);
		param.SetParamAsFile();

		// Add input/output ports
		DFPort OUT_N = ADD_MODEL_OUTPUT(OUT_ELEMENT);
		OUT_N.SetDescription("Output the number of elements");
		DFPort OUT_M = ADD_MODEL_OUTPUT(OUT_ATTRIBUTE);
		OUT_M.SetDescription("Output the number of attributes");
		DFPort OUT_DFILE = ADD_MODEL_OUTPUT(OUT_DATA);
		OUT_DFILE.SetDescription("Output the model");

		return true;
	}
#endif

	raf_in::raf_in()
	{
	}

	raf_in::~raf_in()
	{
	}

	bool raf_in::Setup()
	{
		//setRate��������Ե���ÿһ���˿ڣ�����˿ں�����˿ڶ�Ҫ�������ã������õĻ�Ĭ��Ϊ1
		//�˿ڿ�����Ϊ��һ��Buffer, ���ε�����˿ڻ´�����ε�����˿ڵ����ݣ��ÿ���Ϊֵ��������˵�����
		OUT_ELEMENT.SetRate(1);
		OUT_ATTRIBUTE.SetRate(1);
		OUT_DATA.SetRate(1);

		return true;
	};

	bool raf_in::Initialize()
	{
		return true;
	}

	//Run�������ռ�Ŀ�ľ��ǲ������ݣ�����ֵ����Ӧ�Ķ˿�
	bool raf_in::Run()
	{
		OUT_ELEMENT[0] = NUMBER_OF_ELEMENT;
		OUT_ATTRIBUTE[0] = NUMBER_OF_ATTRIBUTE;

		OUT_INPUT.Resize(NUMBER_OF_ELEMENT, NUMBER_OF_ATTRIBUTE);
		OUT_INPUT.Zero();
		FILE* ind = fopen(INPUT_DATA, "r");
		for (int i = 0; i < NUMBER_OF_ELEMENT; i++)
		{
			for (int j = 0; j < NUMBER_OF_ATTRIBUTE; j++)
			{
				fscanf(ind, "%lf", &OUT_INPUT(i,j));
			}
		}
		OUT_DATA[0] = OUT_INPUT;
		fclose(ind);

		return true;
	}


	bool raf_in::Finalize() {

		return true;
	}

}