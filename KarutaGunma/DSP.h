#pragma once
#include "framework.h"

int
convert_to_text(SndfileHandle sndf, FILE* outfile, int full_precision);

int loadWav(OPENFILENAME* pofn, SndfileHandle* psndf);

int openFileWav(OPENFILENAME* pofn, DWORD size, LPWSTR szFile, DWORD sizeFile);