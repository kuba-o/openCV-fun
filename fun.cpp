#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <iostream>
#include <string>
using namespace cv;
using namespace std;

//test2
class Field {
public:
	int x;
	int y;
	int colorValue;
	Mat frame_roi;
};

template <typename T>
string NumberToString (T Number)
{
  ostringstream ss;
  ss << Number;
  return ss.str();
}

int getRedHist(Mat src, Mat img, int x, int y){
	vector<Mat> bgr_planes;
	split( src, bgr_planes );
	int histSize = 256;

  float range[] = { 0, 256 } ;
  const float* histRange = { range };

  Mat g_hist, b_hist, r_hist;

	calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, 1, 0 );
	calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, 1, 0 );
	calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, 1, 0 );

	int colorValue=0;
  for (int i=0; i<256; ++i){
    colorValue += r_hist.at<float>(i)*i;
    colorValue += b_hist.at<float>(i)*i;
    colorValue += g_hist.at<float>(i)*i;
  }
  //cout<<redValue<<endl; 
  string s=NumberToString(colorValue);
  ++y;
  //putText( img, s, Point(320+80*x, 40+80*y), 1, 1, Scalar(0, 0, 255), 1,1);
  return colorValue;
}

int main() {
	VideoCapture cap(0); // open the default camera
	
	cap.set(CV_CAP_PROP_FRAME_WIDTH,1280);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT,720);

	Mat board = imread ("/home/kuba/Documents/openCV/brd2.png");
	Mat img, frame, hist;
	
	Field fields[8][8];
	int tempx = 0;
	int tempy = 0;
	while(1){
		cap >> frame; // get a new frame from camera

		addWeighted(frame, 1, board, 0.7, 1, img);
		for (int i=0; i<8; ++i){
			for (int j=0; j<8; ++j){
				Mat frame_roi (frame, Rect(320+80*i, 40+80*j, 80, 80));
				fields[i][j].frame_roi = frame_roi;
				fields[i][j].colorValue = getRedHist(frame_roi, img, i, j);
			}
		}
		
		for (int i=0; i<8; ++i){
			for (int j=0; j<8; ++j){
				if (fields[i][j].colorValue > fields[tempx][tempy].colorValue){
					tempx = i;
					tempy = j;
				}
			}
		}
		circle( img,Point(360+80*tempx, 80+80*tempy),15,Scalar(0, 0, 255),15,1 );
		imshow("img", img);
		
		if(waitKey(30) >= 0)
			break;
	}
	cap.release();
	return 0;
}