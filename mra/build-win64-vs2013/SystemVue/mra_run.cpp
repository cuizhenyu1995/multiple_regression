#pragma once
#include "mra_run.h"
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

#ifndef SV_CODE_MRA_RUN
	DEFINE_MODEL_INTERFACE(mra_run)
	{
		SET_MODEL_NAME("Regressioin_Analysis_run");
		SET_MODEL_DESCRIPTION("");
		SET_MODEL_CATEGORY("Regressioin_Analysis");
		ADD_MODEL_HEADER_FILE("mra_run.h");
		model.SetModelCodeGenName("mra_run");
		model.SetModelNamespace("SystemVueModelBuilder");

		// Add parameters


		// Add input/output ports
		DFPort IN_N = ADD_MODEL_INPUT(IN_ELEMENT);
		IN_N.SetDescription("Input the number of elements");
		DFPort IN_M = ADD_MODEL_INPUT(IN_ATTRIBUTE);
		IN_M.SetDescription("Input the number of attributes");
		DFPort IN_FILE = ADD_MODEL_INPUT(IN_INPUTFILE);
		IN_FILE.SetDescription("Input the data");


		DFPort OutputPort = ADD_MODEL_OUTPUT(Output);
		OutputPort.SetDescription("Output");

		return true;
	}
#endif

	mra_run::mra_run()
	{
	}

	mra_run::~mra_run()
	{
	}

	bool mra_run::Setup()
	{
		IN_ELEMENT.SetRate(1);
		IN_ATTRIBUTE.SetRate(1);
		IN_INPUTFILE.SetRate(1);
		Output.SetRate(1);

		return true;
	};

	bool mra_run::Initialize()
	{
		return true;
	}

	bool mra_run::Run()
	{
		int n;
		int dim;
		double* h_data;
		getData(&h_data, &n, &dim);

		double* Answer;
		double* SquarePoor;
		Answer = (double*)malloc(sizeof(double)*dim);
		SquarePoor = (double*)malloc(sizeof(double)*4);

		if (MultipleRegression((double*)h_data, n, dim, Answer, SquarePoor) == 0)
			Display((double*)h_data, Answer, SquarePoor, n, dim);

		free(Answer);
		free(SquarePoor);
		free(h_data);
		return true;
	}


	bool mra_run::Finalize()
	{
		return true;
	}

	void mra_run::Display(double *dat, double *Answer, double *SquarePoor, int rows, int cols)
	{
		FILE *out = fopen("mra_out.txt","w");
		FILE *model = fopen("mra_model.txt","w");
		double v, *p;
		int i, j;
		out_tmp.Resize(cols, 1);
		for (int i = 0; i < cols; i++)
		{
			fprintf(model, "%f\n",Answer[i]);
			out_tmp(i, 0) = Answer[i];
		}
		fclose(model);
		Output[0] = out_tmp;
		fprintf(out,"回归方程式:    Y = %.5lf", Answer[0]);
		for (i = 1; i < cols; i++)
			fprintf(out, " + %.5lf*X%d", Answer[i], i);
		fprintf(out, " \n");
		fprintf(out, "回归显著性检验: \n");
		fprintf(out, "回归平方和：%12.4lf  回归方差：%12.4lf \n", SquarePoor[0], SquarePoor[2]);
		fprintf(out, "剩余平方和：%12.4lf  剩余方差：%12.4lf \n", SquarePoor[1], SquarePoor[3]);
		fprintf(out, "离差平方和：%12.4lf  标准误差：%12.4lf \n", SquarePoor[0] + SquarePoor[1], sqrt(SquarePoor[3]));
		fprintf(out, "F   检  验：%12.4lf  相关系数：%12.4lf \n", SquarePoor[2] / SquarePoor[3],
			sqrt(SquarePoor[0] / (SquarePoor[0] + SquarePoor[1])));
		fprintf(out, "剩余分析: \n");
		fprintf(out, "      观察值      估计值      剩余值    剩余平方 \n");
		for (i = 0, p = dat; i < rows; i++, p++)
		{
			v = Answer[0];
			for (j = 1; j < cols; j++, p++)
				v += *p * Answer[j];
			fprintf(out, "%12.2lf%12.2lf%12.2lf%12.2lf \n", *p, v, *p - v, (*p - v) * (*p - v));
		}
		fclose(out);

	}

	int mra_run::LinearEquations(double *data, int count, double *Answer)
	{
		int j, m, n;
		double tmp, **dat, *d = data;
		dat = (double**)malloc(count * sizeof(double*));
		for (m = 0; m < count; m++, d += (count + 1))
		{
			dat[m] = (double*)malloc((count + 1) * sizeof(double));
			memcpy(dat[m], d, (count + 1) * sizeof(double));
		}
		d = (double*)malloc((count + 1) * sizeof(double));
		for (m = 0; m < count - 1; m++)
		{
			// 如果主对角线元素为0，行交换
			for (n = m + 1; n < count && dat[m][m] == 0.0; n++)
			{
				if (dat[n][m] != 0.0)
				{
					memcpy(d, dat[m], (count + 1) * sizeof(double));
					memcpy(dat[m], dat[n], (count + 1) * sizeof(double));
					memcpy(dat[n], d, (count + 1) * sizeof(double));
				}
			}
			// 行交换后，主对角线元素仍然为0，无解，返回-1
			if (dat[m][m] == 0.0)
			{
				FreeData(dat, d, count);
				return -1;
			}
			// 消元
			for (n = m + 1; n < count; n++)
			{
				tmp = dat[n][m] / dat[m][m];
				for (j = m; j <= count; j++)
					dat[n][j] -= tmp * dat[m][j];
			}
		}
		for (j = 0; j < count; j++)
			d[j] = 0.0;
		// 求得count - 1的元
		Answer[count - 1] = dat[count - 1][count] / dat[count - 1][count - 1];
		// 逐行代入求各元
		for (m = count - 2; m >= 0; m--)
		{
			for (j = count - 1; j > m; j--)
				d[m] += Answer[j] * dat[m][j];
			Answer[m] = (dat[m][count] - d[m]) / dat[m][m];
		}
		FreeData(dat, d, count);
		return 0;
	}

	void mra_run::FreeData(double **dat, double *d, int count)
	{
		int i, j;
		free(d);
		for (i = 0; i < count; i++)
			free(dat[i]);
		free(dat);
	}

	int mra_run::MultipleRegression(double *data, int rows, int cols, double *Answer, double *SquarePoor)
	{
		int m, n, i, count = cols - 1;
		double *dat, *p, a, b;
		if (data == 0 || Answer == 0 || rows < 2 || cols < 2)
			return -1;
		dat = (double*)malloc(cols * (cols + 1) * sizeof(double));
		dat[0] = (double)rows;
		for (n = 0; n < count; n++)                     // n = 0 to cols - 2
		{
			a = b = 0.0;
			for (p = data + n, m = 0; m < rows; m++, p += cols)
			{
				a += *p;
				b += (*p * *p);
			}
			dat[n + 1] = a;                              // dat[0, n+1] = Sum(Xn)
			dat[(n + 1) * (cols + 1)] = a;               // dat[n+1, 0] = Sum(Xn)
			dat[(n + 1) * (cols + 1) + n + 1] = b;       // dat[n+1,n+1] = Sum(Xn * Xn)
			for (i = n + 1; i < count; i++)             // i = n+1 to cols - 2
			{
				for (a = 0.0, p = data, m = 0; m < rows; m++, p += cols)
					a += (p[n] * p[i]);
				dat[(n + 1) * (cols + 1) + i + 1] = a;   // dat[n+1, i+1] = Sum(Xn * Xi)
				dat[(i + 1) * (cols + 1) + n + 1] = a;   // dat[i+1, n+1] = Sum(Xn * Xi)
			}
		}
		for (b = 0.0, m = 0, p = data + n; m < rows; m++, p += cols)
			b += *p;
		dat[cols] = b;                                   // dat[0, cols] = Sum(Y)
		for (n = 0; n < count; n++)
		{
			for (a = 0.0, p = data, m = 0; m < rows; m++, p += cols)
				a += (p[n] * p[count]);
			dat[(n + 1) * (cols + 1) + cols] = a;        // dat[n+1, cols] = Sum(Xn * Y)
		}
		n = LinearEquations(dat, cols, Answer);          // 计算方程式
		// 方差分析
		if (n == 0 && SquarePoor)
		{
			b = b / rows;                                // b = Y的平均值
			SquarePoor[0] = SquarePoor[1] = 0.0;
			p = data;
			for (m = 0; m < rows; m++, p++)
			{
				for (i = 1, a = Answer[0]; i < cols; i++, p++)
					a += (*p * Answer[i]);               // a = Ym的估计值
				SquarePoor[0] += ((a - b) * (a - b));    // U(回归平方和)
				SquarePoor[1] += ((*p - a) * (*p - a));  // Q(剩余平方和)(*p = Ym)
			}
			SquarePoor[2] = SquarePoor[0] / count;       // 回归方差

			if (rows - cols > 0.0)
				SquarePoor[3] = SquarePoor[1] / (rows - cols); // 剩余方差
			else
				SquarePoor[3] = 0.0;

		}
		free(dat);
		return n;
	}
	int mra_run::getData(double** h_data,
		int* n,
		int* dim)
	{
		int i, j;


		*n = IN_ELEMENT[0];
		*dim = IN_ATTRIBUTE[0];

		in_tmp = IN_INPUTFILE[0];


		*h_data = (double*)malloc(sizeof(double) * (*n) * (*dim));

		for (int i = 0; i < *n * *dim; i++)
		{
			*(*h_data + i) = in_tmp(i, 0);
		}


		return 0;
	}

}