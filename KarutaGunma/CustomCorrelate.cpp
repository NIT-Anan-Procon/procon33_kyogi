// Program to find correlation coefficient
#include "framework.h"
#include "DSP.h"

struct corrX {
    double sum_X = 0;
    double squareSum_X = 0;
};

// function that returns correlation coefficient.
double correlationCoefficient(double X[], double Y[], int n)
{

    double sum_X = 0, sum_Y = 0, sum_XY = 0;
    double squareSum_X = 0, squareSum_Y = 0;

    for (int i = 0; i < n; i++)
    {
        // sum of elements of array X.
        sum_X = sum_X + X[i];

        // sum of elements of array Y.
        sum_Y = sum_Y + Y[i];

        // sum of X[i] * Y[i].
        sum_XY = sum_XY + X[i] * Y[i];

        // sum of square of array elements.
        squareSum_X = squareSum_X + X[i] * X[i];
        squareSum_Y = squareSum_Y + Y[i] * Y[i];
    }

    // use formula for calculating correlation coefficient.
    double corr = ((double)n * sum_XY - (double)sum_X * sum_Y)
        / sqrt(((double)n * squareSum_X - (double)sum_X * sum_X)
            * ((double)n * squareSum_Y - (double)sum_Y * sum_Y));

    return corr;
}

// function that returns correlation coefficient with predetermined X
double correlationCoefficientX(double sum_X, double squareSum_X, double X[], double Y[], int n)
{

    double sum_Y = 0, sum_XY = 0;
    double squareSum_Y = 0;

    for (int i = 0; i < n; i++)
    {
        // sum of elements of array Y.
        sum_Y = sum_Y + Y[i];

        // sum of X[i] * Y[i].
        sum_XY = sum_XY + X[i] * Y[i];

        // sum of square of array elements.
        squareSum_Y = squareSum_Y + Y[i] * Y[i];
    }

    // use formula for calculating correlation coefficient.
    double corr = ((double)n * sum_XY - (double)sum_X * sum_Y)
        / sqrt(((double)n * squareSum_X - (double)sum_X * sum_X)
            * ((double)n * squareSum_Y - (double)sum_Y * sum_Y));

    return corr;
}

// function that returns correlation coefficient with predetermined X
double correlationCoefficientX(double sum_X, double squareSum_X, double X[], std::vector<double>* Y, int n,int offset)
{

    double sum_Y = 0, sum_XY = 0;
    double squareSum_Y = 0;

    for (int i = 0; i < n; i++)
    {
        // sum of elements of array Y.
        sum_Y = sum_Y + (*Y)[i + offset];


        // sum of square of array elements.
        squareSum_Y = squareSum_Y + (*Y)[i + offset] * (*Y)[i + offset];
        // sum of X[i] * Y[i].
        sum_XY = sum_XY + X[i] * (*Y)[i+offset];

    }

    // use formula for calculating correlation coefficient.
    double corr = ((double)n * sum_XY - (double)sum_X * sum_Y)
        / sqrt(((double)n * squareSum_X - (double)sum_X * sum_X)
            * ((double)n * squareSum_Y - (double)sum_Y * sum_Y));

    return corr;
}

double correlationCoefficientXD(double sum_X, double squareSum_X, double X[], std::vector<double>* Y, int n, int offset,int scale,int size )
{

    double sum_Y = 0, sum_XY = 0;
    double squareSum_Y = 0;

    for (int i = 0; scale*i < n; i++)
    {
        // sum of elements of array Y.
        sum_Y = sum_Y + (*Y)[scale * i + offset];


        // sum of square of array elements.
        squareSum_Y = squareSum_Y + (*Y)[scale * i + offset] * (*Y)[scale * i + offset];
        // sum of X[i] * Y[i].
        sum_XY = sum_XY + X[scale * i] * (*Y)[scale * i + offset];

    }

    // use formula for calculating correlation coefficient.
    double corr = ((double)size * sum_XY - (double)sum_X * sum_Y)
        / sqrt(((double)size * squareSum_X - (double)sum_X * sum_X)
            * ((double)size * squareSum_Y - (double)sum_Y * sum_Y));

    return corr;
}

void findX(corrX* Xset, double X[], int n)
{
    for (int i = 0; i < n; i++)
    {
        // sum of elements of array Y.
        Xset->sum_X = Xset->sum_X + X[i];
        Xset->squareSum_X = Xset->squareSum_X + X[i] * X[i];
    }
}

void findXD(corrX* Xset, double X[], int n, int scale)
{
    for (int i = 0; scale * i < n; i++)
    {
        // sum of elements of array Y.
        Xset->sum_X = Xset->sum_X + X[scale * i];
        Xset->squareSum_X = Xset->squareSum_X + X[scale * i] * X[scale * i];
    }
}

void Yselect(double* Y[], std::vector<double>* dataZ, int n, int offset)
{
    for (int j = offset; j < offset + n; j++)
    {
        (*Y)[j - offset] = dataZ->at(j);
    }
}

double highestCorr(double sum_X, double squareSum_X, double X[], std::vector<double>* dataZ, int max_n, int n, int* frame,int scale)
{
    double highest_r = -3;
    double r  =-2;
    for (int i = 0; i < max_n-n+1; i++)
    {

        //Yselect(&Y, dataZ, n, i);
        r = correlationCoefficientXD(sum_X, squareSum_X, X, dataZ, n,i,scale,n/scale);

        if (r > highest_r)
        {
            highest_r = r;
            *frame = i;
        }
        if (r > 0.7)
        {

            //show which frame
            std::string capttarget = std::to_string(i);
            capttarget += ", ";
            capttarget += std::to_string(r);
            MessageBox(NULL, std::wstring(capttarget.begin(), capttarget.end()).c_str(), L"target frames", MB_OK);
        }
    }


    return highest_r;
}

// Driver function
double correlate(double X[], std::vector<double>* dataZ,int n,int* frame, int scale)
{

    //Find the size of array.
    int max_n = dataZ->size();

    //Function call to correlationCoefficient.
    corrX Xset1;
    findXD(&Xset1, X, n,scale);

    double r = highestCorr(Xset1.sum_X, Xset1.squareSum_X, X, dataZ, max_n, n,frame,scale);
    

    return r;
}


//correlate one yomi data
double testCorrelate(WavFile* pwavFile,std::string fileName, int* foundFrame)
{
    pwavFile->load();
    const int N1 = pwavFile->pSNDfile->frames();

    double* X = new double[N1];
    std::vector<double> dataZ;
   

    //load yomi data
    SndfileHandle wavJ01 =  SndfileHandle(fileName);
    int targetN = wavJ01.frames();
    double* buf = new double[targetN];
    std::string capttarget = std::to_string(targetN);
    //MessageBox(NULL, std::wstring(capttarget.begin(), capttarget.end()).c_str(), L"target(bunkatsu data) frames", MB_OK);

    wavJ01.readf(buf, targetN);

    //insert to dataZ
    dataZ.insert(dataZ.end(), &buf[0], &buf[targetN]);

    //Find the size of array.
    int n = N1;
    int max_n = dataZ.size();

    //load mondai into X
    int frames = N1 / pwavFile->pSNDfile->channels();

    pwavFile->pSNDfile->readf(X, frames);

    //downsample
    int scale = 100;

    double r = correlate(X, &dataZ, n,foundFrame,scale);

    std::string outputS = "";

    outputS += std::to_string(*foundFrame);

    outputS += "/ ";
    outputS += std::to_string(r);


    delete[] X;
    delete[] buf;

    return r;
    
}



//correlate test against a problem
double testCorrelateJ01(WavFile* pwavFile)
{
    int foundFrame = -1;
    pwavFile->load();
    const int N1 = pwavFile->pSNDfile->frames();

    double* X = new double[N1];
    std::vector<double> dataZ;


    //load J01
    SndfileHandle wavJ01 = SndfileHandle("J05.wav");
    int targetN = wavJ01.frames();
    double* buf = new double[targetN];
    std::string capttarget = std::to_string(targetN);
    MessageBox(NULL, std::wstring(capttarget.begin(), capttarget.end()).c_str(), L"target frames", MB_OK);

    wavJ01.readf(buf, targetN);

    //insert to dataZ
    dataZ.insert(dataZ.end(), &buf[0], &buf[targetN]);

    //Find the size of array.
    int n = N1;
    int max_n = dataZ.size();

    //load J01 into X
    int frames = N1 / pwavFile->pSNDfile->channels();

    pwavFile->pSNDfile->readf(X, frames);

    //downsample
    int scale = 200;

    double r = correlate(X, &dataZ, n, &foundFrame,scale);

    std::string outputS = "";

    outputS += std::to_string(foundFrame);

    outputS += "/ ";
    outputS += std::to_string(r);

    std::wstring outputSW = std::wstring(outputS.begin(), outputS.end());

    const wchar_t* output = outputSW.c_str();
    MessageBoxW(NULL, output, L"frame/highest r", MB_OK);

    delete[] X;


    return r;

}

//correlate mondai against all yomi and output into a file
void correlateAllJ(WavFile* pwavFile)
{
    //manipulate string
    std::string fileName;
    std::string output = "";

    double r = -3;
    int frame = -1;

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
        //get r
        r = testCorrelate(pwavFile, fileName, &frame);

        //print out in file
        output = std::to_string(frame);

        output += "/ ";
        output += std::to_string(r);
        output += "\n";

        FILE* outfile;
        std::wstring outname = L"";
        outname += pwavFile->pofn->lpstrFileTitle;
        outname += L"J.txt";

        const wchar_t* outputTitle = outname.c_str();
        _wfopen_s(&outfile, outputTitle, L"a");
        if (outfile != NULL)
        {
            fprintf(outfile, "%d / %lf \n",
                frame, r);

            fclose(outfile);
        }
    }
}

void correlateAllE(WavFile* pwavFile)
{
    {
        //manipulate string
        std::string fileName;
        std::string output = "";

        double r = -3;
        int frame = -1;

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
            //get r
            r = testCorrelate(pwavFile, fileName, &frame);

            //print out in file
            output = std::to_string(frame);

            output += "/ ";
            output += std::to_string(r);
            output += "\n";

            FILE* outfile;
            std::wstring outname = L"";
            outname += pwavFile->pofn->lpstrFileTitle;
            outname += L"E.txt";

            const wchar_t* outputTitle = outname.c_str();
            _wfopen_s(&outfile, outputTitle, L"a");
            if (outfile != NULL)
            {
                fprintf(outfile, "%d / %lf \n",
                    frame, r);

                fclose(outfile);
            }
        }
    }
}