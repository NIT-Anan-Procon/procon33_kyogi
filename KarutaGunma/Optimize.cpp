#include "framework.h"
#include "DSP.h"
#include "CustomCorrelate.h"
#include <thread>

void loadAllYJ(std::vector<double*>* Ys, std::vector<int>* Yframe)
{

    //manipulate string
    std::string fileName;
    //iterate filename
    for (int i = 1; i < 45; i++)
    {
        fileName = "J";
        if (i < 10)
        {
            fileName += "0";
            fileName += std::to_string(i);
            fileName += ".wav";
        }
        else
        {

            fileName += std::to_string(i);
            fileName += ".wav";
        }

        //load yomi data
        SndfileHandle wavJ01 = SndfileHandle(fileName);
        int targetN = wavJ01.frames();

        Ys->push_back(new double[targetN]);

        wavJ01.readf(( * Ys)[i-1], targetN);
        Yframe->push_back(wavJ01.frames());
    }

}

void loadAllYE(std::vector<double*>* Ys, std::vector<int>* Yframe)
{

    //manipulate string
    std::string fileName;
    //iterate filename
    for (int i = 1; i < 45; i++)
    {
        fileName = "E";
        if (i < 10)
        {
            fileName += "0";
            fileName += std::to_string(i);
            fileName += ".wav";
        }
        else
        {

            fileName += std::to_string(i);
            fileName += ".wav";
        }

        //load yomi data
        SndfileHandle wavJ01 = SndfileHandle(fileName);
        int targetN = wavJ01.frames();

        Ys->push_back(new double[targetN]);

        wavJ01.readf((*Ys)[i - 1], targetN);
        Yframe->push_back(wavJ01.frames());
    }

}

void clearAllY(std::vector<double*>* Ys)
{
    for (double* buf : *Ys)
    {
        delete[] buf;
    }
}

void coeffCalc(double* pr, double* Y, double X[],const corrX &preX, int mondaiSize ,int offset , int scale ,int size)
{
    double sum_Y = 0, sum_XY = 0;
    double squareSum_Y = 0;

    double sum_X = preX.sum_X;
    double squareSum_X = preX.squareSum_X;

    for (int i = 0; scale * i < mondaiSize; i++)
    {
        double currentY = Y[scale * i + offset];
        // sum of elements of array Y.
        sum_Y = sum_Y + currentY;


        // sum of square of array elements.
        squareSum_Y = squareSum_Y + currentY*currentY;
        // sum of X[i] * Y[i].
        sum_XY = sum_XY + X[scale * i] * currentY;

    }

    // use formula for calculating correlation coefficient.
    double corr = ((double)size * sum_XY - (double)sum_X * sum_Y)
        / sqrt(((double)size * squareSum_X - (double)sum_X * sum_X)
            * ((double)size * squareSum_Y - (double)sum_Y * sum_Y));

    //apply changes
    *pr = corr;
}



//correlato to file num
void correlateOne(double** pr, int** pframe, int file, std::vector<double*>& Ys , double X[], const corrX& preX, int yomiSize, int mondaiSize, int scale)
{
    //load Y into an vector
    double highest_r = -3;
    double current_r = -2;
    int frame = -1;

    //sliding offset
    for (int offset = 0; offset < yomiSize - mondaiSize + 1; offset++)
    {
        //get correlate coefficient + frame
        coeffCalc(&current_r, Ys[file], X, preX,mondaiSize, offset, scale, mondaiSize / scale);
        
        if (highest_r < current_r)
        {
            highest_r = current_r;
            frame = offset;
        }
    }
    //apply changes to r and frame
    (*pr)[file] = highest_r;
    (*pframe)[file] = frame;
}

//correlate mondai against all yomi and output into a file
void correlateAllJOpt(WavFile* pwavFile,int scale)
{
    //init Y
    std::string output = "";


    std::vector<double*> Ys;
    std::vector<int> Yframe;
    loadAllYJ(&Ys,&Yframe);

    //init X
    pwavFile->load();
    const int mondaiN = pwavFile->pSNDfile->frames();
    double* X = new double[mondaiN];

    pwavFile->pSNDfile->readf(X, mondaiN);

    //precal X
    corrX preX;
    findXD(&preX, X, mondaiN, scale);

   //get r and frame
    double* r = new double[44];
    int* frame = new int[44];
   // testCorrelate(&r, &frame, 0);
    std::vector<std::thread> threads;

    for (int file = 0; file < 44; file++) {
        threads.push_back(std::thread(correlateOne, &r, &frame, file,std::ref(Ys),X,std::ref(preX), Yframe[file], mondaiN, scale));
    }

    for (auto& th : threads) {
        th.join();
    }
    //output result to a file
    std::string outputS = "";

    for (int file = 0; file < 44; file++) {
        FILE* outfile;
        std::wstring outname = L"";
        outname += pwavFile->pofn->lpstrFileTitle;
        outname += L"J.txt";

        const wchar_t* outputTitle = outname.c_str();
        _wfopen_s(&outfile, outputTitle, L"a");
        if (outfile != NULL)
        {
            fprintf(outfile, "%d / %lf \n",
                frame[file], r[file]);

            fclose(outfile);
        }
    }
    



    //clean up
    delete[] X;
    delete[] r;
    delete[] frame;
    clearAllY(&Ys);
}


//correlate mondai against all yomi and output into a file
void correlateAllEOpt(WavFile* pwavFile, int scale)
{
    //init Y
    std::string output = "";


    std::vector<double*> Ys;
    std::vector<int> Yframe;
    loadAllYE(&Ys, &Yframe);

    //init X
    pwavFile->load();
    const int mondaiN = pwavFile->pSNDfile->frames();
    double* X = new double[mondaiN];

    pwavFile->pSNDfile->readf(X, mondaiN);

    //precal X
    corrX preX;
    findXD(&preX, X, mondaiN, scale);

    //get r and frame
    double* r = new double[44];
    int* frame = new int[44];
    // testCorrelate(&r, &frame, 0);
    std::vector<std::thread> threads;

    for (int file = 0; file < 44; file++) {
        threads.push_back(std::thread(correlateOne, &r, &frame, file, std::ref(Ys), X, std::ref(preX), Yframe[file], mondaiN, scale));
    }

    for (auto& th : threads) {
        th.join();
    }
    //output result to a file
    std::string outputS = "";

    for (int file = 0; file < 44; file++) {
        FILE* outfile;
        std::wstring outname = L"";
        outname += pwavFile->pofn->lpstrFileTitle;
        outname += L"E.txt";

        const wchar_t* outputTitle = outname.c_str();
        _wfopen_s(&outfile, outputTitle, L"a");
        if (outfile != NULL)
        {
            fprintf(outfile, "%d / %lf \n",
                frame[file], r[file]);

            fclose(outfile);
        }
    }




    //clean up
    delete[] X;
    delete[] r;
    delete[] frame;
    clearAllY(&Ys);
}

