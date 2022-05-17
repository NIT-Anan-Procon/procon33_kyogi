#pragma once
#include "framework.h"

int
convert_to_text(SndfileHandle sndf, FILE* outfile, int full_precision);

int loadWav(SndfileHandle* psndf);

int openFileWav(OPENFILENAME* pofn);