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
	class mra_run : public SystemVueModelBuilder::DFModel
	{
	public:
		DECLARE_MODEL_INTERFACE(mra_run);
		mra_run();
		virtual ~mra_run();

		virtual bool Setup();
		virtual bool Initialize();
		virtual bool Run();
		virtual bool Finalize();
		
		void FreeData(double **dat, double *d, int count);
		int LinearEquations(double *data, int count, double *Answer);
		int MultipleRegression(double *data, int rows, int cols, double *Answer, double *SquarePoor);
		void Display(double *dat, double *Answer, double *SquarePoor, int rows, int cols);
		int getData(double** h_data, int* n, int* dim);


		/// define parameters
		int NUMBER_OF_ELEMENT;
		int NUMBER_OF_ATTRIBUTE;

		/// define ports
		IntCircularBuffer IN_ELEMENT;
		IntCircularBuffer IN_ATTRIBUTE;
		DoubleMatrixCircularBuffer IN_INPUTFILE;

		DoubleMatrixCircularBuffer Output;

	protected:
		DoubleMatrix in_tmp;
		DoubleMatrix out_tmp;

	};
}
