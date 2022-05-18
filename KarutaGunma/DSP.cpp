#include "framework.h"
#include <iostream>

#define	BLOCK_SIZE 4096

#ifdef DBL_DECIMAL_DIG
	#define OP_DBL_Digs (DBL_DECIMAL_DIG)
#else

	#ifdef DECIMAL_DIG
		#define OP_DBL_Digs (DECIMAL_DIG)
	#else
		#define OP_DBL_Digs (DBL_DIG + 3)
	#endif
#endif

int
convert_to_text(SndfileHandle sndf,FILE* outfile, int full_precision)
{
	float* buf;
	sf_count_t frames;
	int k, m, readcount;
	int channels = sndf.channels();

	buf = new float[BLOCK_SIZE];
	if (buf == nullptr)
	{
		printf("Error : Out of memory.\n\n");
		return 1;
	};

	frames = BLOCK_SIZE / channels;


	//while ((readcount = (int)sf_readf_float(infile, buf, frames)) > 0)
	while ((readcount = (int)sndf.readf(buf, frames)) > 0)
	{
		for (k = 0; k < readcount; k++)
		{
			for (m = 0; m < channels; m++)
				if (full_precision)
					fprintf(outfile, " %.*e", OP_DBL_Digs - 1, buf[k * channels + m]);
				else
					fprintf(outfile, " % 12.10f", buf[k * channels + m]);
			fprintf(outfile, "\n");
		};
	};

	delete[] buf;

	return 0;
} /* convert_to_text */

int openFileWav(OPENFILENAME* pofn, DWORD size, LPWSTR szFile, DWORD sizeFile )
{
    
    // Initialize remaining fields of OPENFILENAME structure
    pofn->lStructSize = size;
    pofn->hwndOwner = NULL;
    pofn->lpstrFile = szFile;
    pofn->nMaxFile = sizeFile;
    pofn->lpstrFilter = _T("Wav File(*.wav)\0*.WAV\0All\0*.*\0");
    pofn->nFilterIndex = 1;
    pofn->lpstrFileTitle = NULL;
    pofn->nMaxFileTitle = 0;
    pofn->lpstrInitialDir = NULL;
    pofn->Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (!GetOpenFileName(pofn))
    {
        std::ofstream logFile;
        logFile.open("log.txt", std::ios::app);
        logFile << "Error Opening File\n\n";
        logFile.close();
        return -1;
    }
    return 1;


    // use ofn.lpstrFile here


    //convert lpstrFile to string if needed
    /*
    std::wstring arr_w(ofn.lpstrFile);
    std::string fileName(arr_w.begin(), arr_w.end());
    */
}

//FUNCTION: Load Wav file and dump info, data
int loadWav(OPENFILENAME* pofn, SndfileHandle* myf)
{
   

    *myf = SndfileHandle(pofn->lpstrFile);

    //convert char to string to wstring to wchar LOL
    std::string strErr = std::string(myf->strError());
    std::wstring wstrErr = std::wstring(strErr.begin(), strErr.end());
    const wchar_t* wErr = wstrErr.c_str();

    //log

    std::locale::global(std::locale(""));
    std::wofstream myfile;
    myfile.open("Wcharexample.txt", std::ios::app);
    myfile << "\n\n";
    myfile << "Opened file  :" << pofn->lpstrFile << '\n';
    myfile << "Error        :" << wErr << '\n';
    myfile << "Sample rate  :" << myf->samplerate() << '\n';
    myfile << "Channels     :" << myf->channels() << '\n';
    myfile << "Frames       :" << int(myf->frames()) << '\n';
    myfile.close();

    //dump
    FILE* dataOut;
    fopen_s(&dataOut, "./dataOut.txt", "w");
    if (dataOut != NULL)
    {
        convert_to_text(*myf, dataOut, 0);

        fclose(dataOut);
    }

    return 1;
}

//main stuff to do ( break off from gui)
int DSPMAIN()
{
    // common dialog box structure, setting all fields to 0 is important
    OPENFILENAME ofn = { 0 };
    TCHAR szFile[260] = { 0 };

    //load file into filehandle
    SndfileHandle SNDfile;
    openFileWav(&ofn, sizeof(ofn), szFile, sizeof(szFile));
    loadWav(&ofn, &SNDfile);

    //Do DSP here


    return 1;
}