#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/aruco.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

using namespace cv;
using namespace aruco;

const float calibrationSquareDimension = 0.023f; //meteres
const float arucoSquareDimension = 0.1016f;

void createArucoMarkers();

int main(int argv, char* argc)
{
	/*Mat frame;
	VideoCapture vid(0);

	if (!vid.isOpened())
	{
		return -1;
	}

	namedWindow("Webcam", WINDOW_AUTOSIZE);
	int fps = (int)vid.get(CAP_PROP_FPS);

	while (true)
	{
		vid.read(frame);
		imshow("webcam", frame);

		if (waitKey(1000 / fps) >= 0)
		{
			break;
		}
	}*/
	createArucoMarkers();

	return 1;
}

void createArucoMarkers()
{
	Mat output;
	Ptr<Dictionary> markerDictionary = getPredefinedDictionary(PREDEFINED_DICTIONARY_NAME::DICT_4X4_50);

	for (int i = 0; i < 50; i++)
	{
		drawMarker(markerDictionary, i, 500, output, 1);
		std::ostringstream convert;
		std::string imageName = "4x4Marker_";
		convert << imageName << i << ".jpg";
		imwrite(convert.str(), output);
	}
}