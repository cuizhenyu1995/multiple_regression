#pragma once
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

namespace SystemVueModelBuilder
{
	class raf_run : public SystemVueModelBuilder::DFModel
	{
	public:
		DECLARE_MODEL_INTERFACE(raf_run);
		raf_run();
		virtual ~raf_run();

		virtual bool Setup();
		virtual bool Initialize();
		virtual bool Run();
		virtual bool Finalize();

		

		/// define parameters
		int NUMBER_OF_ELEMENT;
		int NUMBER_OF_ATTRIBUTE;
		char* INPUT_MODEL;

		/// define ports
		IntCircularBuffer IN_ELEMENT;
		IntCircularBuffer IN_ATTRIBUTE;
		DoubleMatrixCircularBuffer IN_DATA;

		DoubleMatrixCircularBuffer Output;

	protected:
		DoubleMatrix in_data;

		DoubleMatrix out_tmp;

	};
}
