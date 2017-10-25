#pragma once
#include "SystemVue/ModelBuilder.h"
#include "SystemVue/CircularBuffer.h"
#include "SystemVue/Matrix.h"
#include "SystemVue/MatrixCircularBuffer.h"
#include "SystemVue/DFParam.h"
#include <iostream>


namespace SystemVueModelBuilder
{

	class raf_in : public SystemVueModelBuilder::DFModel
	{
	public:
		DECLARE_MODEL_INTERFACE(raf_in);
		raf_in();
		virtual ~raf_in();

		virtual bool Setup();
		virtual bool Initialize();
		virtual bool Run();
		virtual bool Finalize();



		/// define useless parameters, just for illustrating how to add parameters 
		//字符串类型，只能用char*， 而不能用string类型

		int NUMBER_OF_ELEMENT;
		int NUMBER_OF_ATTRIBUTE;
		char* INPUT_DATA;

		/// define input and output ports
		IntCircularBuffer OUT_ELEMENT;
		IntCircularBuffer OUT_ATTRIBUTE;
		DoubleMatrixCircularBuffer OUT_DATA;

	protected:
		DoubleMatrix OUT_INPUT;
	};
}

