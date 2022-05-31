#include "framework.h"
#include "DSP.h"
#include <iostream>


#include <corecrt_math_defines.h>
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

int openFileWav(OPENFILENAME* pofn, DWORD size, LPWSTR szFile, DWORD sizeFile, LPWSTR szFileTitle, DWORD sizeFileTitle)
{
    
    // Initialize remaining fields of OPENFILENAME structure
    pofn->lStructSize = size;
    pofn->hwndOwner = NULL;
    pofn->lpstrFile = szFile;
    pofn->nMaxFile = sizeFile;
    pofn->lpstrFilter = _T("Wav File(*.wav)\0*.WAV\0All\0*.*\0");
    pofn->nFilterIndex = 1;
    pofn->lpstrFileTitle = szFileTitle;
    pofn->nMaxFileTitle = sizeFileTitle;
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

    //log window and file
    std::wstring outputSW = L"Opened file  :";
    outputSW += pofn->lpstrFile;

    std::string outputS = "";
    outputS += "\nError        :";
    outputS += strErr;
    outputS += "\nSample rate  :";
    outputS += std::to_string( myf->samplerate());
    outputS += "\nChannels     :";
    outputS += std::to_string(myf->channels());
    outputS += "\nFrames     :";
    outputS += std::to_string(int(myf->frames()));

    std::wstring outputSW2 = std::wstring(outputS.begin(), outputS.end());
    outputSW += outputSW2;

    const wchar_t* output = outputSW.c_str();
    MessageBoxW(NULL,output, L"Log", MB_OK);


    //file

    /*
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
    */



    return 1;
}

//main stuff to do ( break off from gui)
int initWavFile(WavFile** ppwavFile)
{
    if (*ppwavFile == nullptr)
    {
        *ppwavFile = new WavFile;
    }
    else {
        (*ppwavFile)->reload();
    }

    
    if ( (*ppwavFile) != nullptr)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

int fftWav(WavFile* pwavFile)
{
    pwavFile->load();

    //Do DSP here
    const int totalFrames = static_cast<int>(pwavFile->pSNDfile->frames());
    const int N = 32768*2;


    fftw_complex* in = static_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex) * N));
    fftw_complex* out = static_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex) * N));
    fftw_complex* in2 = static_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex) * N));
    //fftw_complex in[N], out[N]; /* double [2] */
    fftw_plan p, q;
    int i;



    /* prepare a cosine wave */
    /*
    for (i = 0; i < N; i++) {
        in[i][0] = cos(static_cast<long double>(3) * 2 * M_PI * i / N);
        in[i][1] = 0;
    }
    */
    /* forward Fourier transform, save the result in 'out' */


    //init audio stream buffer 
    double* buf = new double[N];
    if (buf == nullptr)
    {
        printf("Error : Out of memory.\n\n");

    };


    sf_count_t frames = N / pwavFile->pSNDfile->channels();


    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    q = fftw_plan_dft_1d(N, out, in2, FFTW_BACKWARD, FFTW_ESTIMATE);

    double* outdata = new double[frames];
    //init out wav
    //const int format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
    const int format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;
    const int channels = 1;
    const int sampleRate = 48000;

    std::wstring outputWavWstr = pwavFile->pofn->lpstrFileTitle;
    outputWavWstr += L"_rev.wav";
    const wchar_t* outfilename = outputWavWstr.c_str();

    SndfileHandle outfile(outfilename, SFM_WRITE, format, channels, sampleRate);
    if (not outfile) return -1;

    //start loop to get all bins
    int currentRead;
    int readcount = 0;;
    std::wstring currentCount;
    while ((currentRead = (int)(pwavFile->pSNDfile->readf(buf, frames)))> 0 )   //fill in audio buffer
    {
            readcount++;
            currentCount=std::to_wstring(readcount);
            
            

            //fill in input with buffer
            if (pwavFile->pSNDfile->channels() == 1)
            {
                for (i = 0; i < N; i++) {
                    //in[i][0] = buf[i] * 0.5 * (1.0 - cos(2.0 * M_PI * i / static_cast<long double>(N - 1))); //window function
                    in[i][0] = buf[i];
                    in[i][1] = 0;

                 
                    
                }

                //clear extra buffer
                for (int start = currentRead; start < N; start++)
                {
                    in[start][0] = 0;
                    in[start][1] = 0;
                }
            }
            /*
            else
            {
                for (i = 0; 2 * i < N; i++) {
                    in[i][0] = buf[2 * i - 1];
                    in[i][1] = buf[2 * i];
                }
            }
            */


            FILE* fftOut;
            std::wstring outputSW = L"./fftOut_";
            outputSW += pwavFile->pofn->lpstrFileTitle;
            outputSW += L"_bin_";
            outputSW += currentCount;
            outputSW += L".txt";

            const wchar_t* output = outputSW.c_str();
            _wfopen_s(&fftOut, output, L"w");
            if (fftOut != NULL)
            {

                fftw_execute(p);
                for (i = 0; i < N; i++)
                    fprintf(fftOut, "freq: %3d %+9.5f %+9.5f I\n", i, out[i][0], out[i][1]);


                fclose(fftOut);

            }



            FILE* fftRevOut;
            std::wstring outputRevSW = L"./fftRevOut_";
            outputRevSW += pwavFile->pofn->lpstrFileTitle;
            outputRevSW += L"_bin_";
            outputRevSW += currentCount;
            outputRevSW += L".txt";


            const wchar_t* outputRev = outputRevSW.c_str();
            _wfopen_s(&fftRevOut, outputRev, L"w");
            if (fftRevOut != NULL)
            {
                /* backward Fourier transform, save the result in 'in2' */
                fftw_execute(q);
                /* normalize */
                for (i = 0; i < N; i++) {
                    in2[i][0] *= 1. / N;
                    outdata[i] = static_cast<double>(in2[i][0]);
                    in2[i][1] *= 1. / N;
                }
                for (i = 0; i < N; i++)
                    fprintf(fftRevOut, "recover: %3d %+9.5f %+9.5f I vs. %+9.5f %+9.5f I\n",
                        i, in[i][0], in[i][1], in2[i][0], in2[i][1]);

                fclose(fftRevOut);
            }


            // write into output wav

            outfile.writef(outdata, frames);
        

    }





    


    //clean up
    fftw_destroy_plan(q);

    fftw_destroy_plan(p);
    fftw_cleanup();
    fftw_free(in);
    fftw_free(in2);
    fftw_free(out);



    


    delete[] buf;
    delete[] outdata;
    return 1;
}