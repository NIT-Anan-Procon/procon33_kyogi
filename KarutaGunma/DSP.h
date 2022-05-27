#pragma once
#include "framework.h"

class WavFile;

int
convert_to_text(SndfileHandle sndf, FILE* outfile, int full_precision);

int loadWav(OPENFILENAME* pofn, SndfileHandle* psndf);

int openFileWav(OPENFILENAME* pofn, DWORD size, LPWSTR szFile, DWORD sizeFile, LPWSTR szFileTitle, DWORD sizeFileTitle);

int initWavFile(WavFile** pwavFile);

int fftWav(WavFile* pwavFile);

//helper class
class WavFile {
public:
    // common dialog box structure, setting all fields to 0 is important
    OPENFILENAME* pofn;
    TCHAR szFile[260] = { 0 };
    TCHAR szFileTitle[260] = { 0 };

    //load file into filehandle
    SndfileHandle* pSNDfile;

    bool hasLoaded;

    WavFile()
    {
        pofn = new OPENFILENAME();
        *pofn = { 0 };
        pSNDfile = new SndfileHandle();
        hasLoaded = false;

        if (openFileWav(pofn, sizeof(*pofn), szFile, sizeof(szFile),szFileTitle,sizeof(szFileTitle)) == 1)
        {
            if (load() == 1)
            {
                hasLoaded = true;
            }
        }
    }

    ~WavFile()
    {
        delete pofn;
        delete pSNDfile;
    }

    int reload()
    {

        if (openFileWav(pofn, sizeof(*pofn), szFile, sizeof(szFile), szFileTitle, sizeof(szFileTitle)) == 1)
        {
            if (load() == 1)
            {
                hasLoaded = true;
            }

            return 1;
        }
        return -1;
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
        std::wstring outputSW = L"./dataOut_";
        outputSW += pofn->lpstrFileTitle;
        outputSW += L".txt";

        const wchar_t* output = outputSW.c_str();
        _wfopen_s(&dataOut, output, L"w");
        //fopen_s(&dataOut, "./datadump.txt", "w");
        if (dataOut != NULL)
        {
            convert_to_text(*pSNDfile, dataOut, 0);

            fclose(dataOut);
        }
        return 1;
    }

    
private:
};