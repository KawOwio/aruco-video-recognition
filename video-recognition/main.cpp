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
const Size chessboardDimensions = Size(9, 6);

void createArucoMarkers();
void createKnownBoardPosition(Size boardSize, float length, std::vector<Point3f>& corners);
void getChessboardCorners(std::vector<Mat> imgs, std::vector<std::vector<Point2f>>& foundCorners, bool showResults = false);

int main(int argv, char* argc)
{
	//createArucoMarkers();

	Mat frame;
	Mat drawToFrame;
	Mat distanceCoefficients;

	Mat cameraMatrix = Mat::eye(3, 3, CV_64F);

	std::vector<Mat> savedImages;
	std::vector<std::vector<Point2f>> markerCorners, rejected;

	VideoCapture vid(0);

	if (!vid.isOpened())
	{
		return -1;
	}

	namedWindow("Webcam", WINDOW_AUTOSIZE);
	int fps = 30;

	while (true)
	{
		if (!vid.read(frame))
		{
			break;
		}

		std::vector<Vec2f> foundPoints;
		bool found = false;

		found = findChessboardCorners(frame, chessboardDimensions, foundPoints, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE);
		frame.copyTo(drawToFrame);
		drawChessboardCorners(drawToFrame, chessboardDimensions, foundPoints, found);
		if (found)
		{
			imshow("Webcam", drawToFrame);
		}
		else
		{
			imshow("Webcam", frame);
		}

		if (waitKey(1000 / fps) >= 0)
		{
			break;
		}
	}

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

void createKnownBoardPosition(Size boardSize, float length, std::vector<Point3f>& corners)
{
	for (int i = 0; i < boardSize.height; i++)
	{
		for (int j = 0; j < boardSize.width; j++)
		{
			corners.push_back(Point3f(j * length, i * length, 0.0f));
		}
	}
}

void getChessboardCorners(std::vector<Mat> imgs, std::vector<std::vector<Point2f>>& foundCorners, bool showResults)
{
	for (std::vector<Mat>::iterator iter = imgs.begin(); iter != imgs.end(); iter++)
	{
		std::vector<Point2f> pointBuff;
		bool found = findChessboardCorners(*iter, Size(9,6), pointBuff, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE);

		if (found)
		{
			foundCorners.push_back(pointBuff);
		}

		if (showResults)
		{
			drawChessboardCorners(*iter, Size(9, 6), pointBuff, found);
			imshow("Looking for corners", *iter);
			waitKey(0);
		}
	}
}