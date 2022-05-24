#pragma once
#include "framework.h"

class WavFile;

int
convert_to_text(SndfileHandle sndf, FILE* outfile, int full_precision);

int loadWav(OPENFILENAME* pofn, SndfileHandle* psndf);

int openFileWav(OPENFILENAME* pofn, DWORD size, LPWSTR szFile, DWORD sizeFile);

int initWavFile(WavFile** pwavFile);

int fftWav(WavFile* pwavFile);

//helper class
class WavFile {
public:
    // common dialog box structure, setting all fields to 0 is important
    OPENFILENAME* pofn;
    TCHAR szFile[260] = { 0 };

    //load file into filehandle
    SndfileHandle* pSNDfile;

    WavFile()
    {
        pofn = new OPENFILENAME();
        *pofn = { 0 };
        pSNDfile = new SndfileHandle();

        openFileWav(pofn, sizeof(*pofn), szFile, sizeof(szFile));
        load();
    }

    ~WavFile()
    {
        delete pofn;
        delete pSNDfile;
    }

    int reload()
    {
        openFileWav(pofn, sizeof(*pofn), szFile, sizeof(szFile));
        load();
        return 1;
    }

    int load()
    {
        if (loadWav(pofn, pSNDfile) != -1) {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    int dump()
    {
        if (load() == -1)
        {
            return -1;
        }
        FILE* dataOut;
        fopen_s(&dataOut, "./dataOut.txt", "w");
        if (dataOut != NULL)
        {
            convert_to_text(*pSNDfile, dataOut, 0);

            fclose(dataOut);
        }
        return 1;
    }

    
private:
};