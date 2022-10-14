#include "framework.h"
#include "DSP.h"

void precalcYJ(std::vector<double>** Y, std::vector<double>* Ysqr, std::vector<double>* Ysum,int scale)
{
	//precalculate all Y jp ver
    //manipulate string
    std::string fileName;
    std::string output = "";

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

        //load yomi data
        SndfileHandle wavJ = SndfileHandle(fileName);
        int targetN = wavJ.frames();
        double* buf = new double[targetN];

        wavJ.readf(buf, targetN);

        //insert to dataZ
        (* Y)[i-1].insert((*Y)[i - 1].end(), &buf[0], &buf[targetN]);

        //precalc sum and sum of sqr
       

        delete[] buf;
    }
}