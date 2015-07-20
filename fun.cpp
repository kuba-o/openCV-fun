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
	int redValue;
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

  Mat r_hist;

	calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, 1, 0 );

	int redValue=0;
  for (int i=0; i<256; ++i){
    redValue += r_hist.at<float>(i)*i;
  }
  //cout<<redValue<<endl; 
  string s=NumberToString(redValue);
  ++y;
  putText( img, s, Point(320+80*x, 40+80*y), 1, 1, Scalar(0, 0, 255), 1,1);
  return redValue;
}

int main() {
	VideoCapture cap(0); // open the default camera
	
	cap.set(CV_CAP_PROP_FRAME_WIDTH,1280);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT,720);

	Mat board = imread ("/home/kuba/Documents/openCV/brd2.png");
	Mat img, frame, hist;
	
	Field fields[8][8];
	Mat hists[8][8];
	
	while(1){
		cap >> frame; // get a new frame from camera

		addWeighted(frame, 1, board, 0.7, 1, img);
		for (int i=0; i<8; ++i){
			for (int j=0; j<8; ++j){
				Mat frame_roi (frame, Rect(320+80*i, 40+80*j, 80, 80));
				fields[i][j].frame_roi = frame_roi;
				fields[i][j].redValue = getRedHist(frame_roi, img, i, j);
			}
		}
		
		imshow("img", img);
		
		if(waitKey(30) >= 0)
			break;
	}
	cap.release();
	return 0;
}