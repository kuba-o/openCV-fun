#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

int main( int argc, char** argv )
{
  Mat src, dst;

  /// Load image
  src = imread ("/home/kuba/Documents/openCV/red.png");


  /// Separate the image in 3 places ( B, G and R )
  vector<Mat> bgr_planes;
  split( src, bgr_planes );

  /// Establish the number of bins
  int histSize = 256;

  /// Set the ranges ( for B,G,R) )
  float range[] = { 0, 256 } ;
  const float* histRange = { range };

  Mat b_hist, g_hist, r_hist;

  /// Compute the histograms:
  calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, 1, 0 );

  cout<<r_hist.size()<<endl;
  cout<<r_hist<<endl;
  cout<<r_hist.at<float>(255)<<endl;

  imshow("test", r_hist);

  waitKey(0);

  return 0;
} 