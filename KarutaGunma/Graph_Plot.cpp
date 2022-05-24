#include "pbPlots.hpp"
#include "supportLib.hpp"

int main() {
	//creating an image
	RGBABitmapImageReference* imageRef = CreateRGBABitmapImageReference();

	vector<double> x{ -2, -1, 0, 1, 2 };
	vector<double> y{ 2, -1, -2, -1, 2 };

	DrawScatterPlot(imageRef, 600, 400, x, y);

	vector<double>* pngData = ConvertToPNG(imageRef -> image);
	WriteToFile(pngData, "plot.png");
	DeleteImage(imageRef->image);

	return 0;
}