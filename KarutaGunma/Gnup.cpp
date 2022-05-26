#ifndef GNUPLOT_H
#define GNUPLOT_H
#include <iostream>
#include <string>
using namespace std;

class gnuplot {
public:
    gnuplot();
    ~gnuplot();
    void operator() (const string& command);
protected:
    FILE *gnuplotpipe;
};

gnuplot::gnuplot() {
    gnuplotpipe = popen("gnuplot -persist", "w");
    if (!gnuplotpipe)
        cerr << ("Gnuplot not found!");
}
gnuplot::~gnuplot() {
    fprintf(gnuplotpipe, "exit\n");
    pclose(gnuplotpipe);
}
void gnuplot::operator() (const string& command) {
    fprintf(gnuplotpipe, "%s\n", command.c_str());
    fflush(gnuplotpipe);
}
#endif

/*other method
int main() {
    int x = 0;
    int y = 0;
    FILE* fp = NULL;
    FILE* gnupipe = NULL;
    char* fnuCommands[] = { "set title \"Demo\"", "plot 'data.tmp'" };

    fp = fopen("data.tmp", "w");
    gnupipe = _popen("gnuplot -persistent", "w");

    for (int i = 0; i < 11; i++) {
        fprintf(fp, "%d %d\n", x, y);

        x++;
        y++;
    }

    for (int i = 0; i < 2; i++) {
        fprintf(gnupipe, "%s\n", GnuCommands);
    }

    return 0;
}
*/