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