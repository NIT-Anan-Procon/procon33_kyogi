#pragma once
#include "framework.h"
#include "DSP.h"


struct corrX {
    double sum_X = 0;
    double squareSum_X = 0;
};
void findXD(corrX* Xset, double X[], int size, int scale);
double testCorrelateJ01(WavFile* pwavFile);

void correlateAllJ(WavFile* pwavFile);
void correlateAllE(WavFile* pwavFile);
