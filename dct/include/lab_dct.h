#ifndef LAB_DCT_H
#define LAB_DCT_H

#include <iostream>
#include <stdio.h>
#include <math.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

/**
 * Test our custom dot product function against Eigen's built in function
 * @param xin First vector
 * @param yin Second vector
 * @param length Length of the vectors
 * @return True/False if both methods do/do not produce equal outputs
 **/
float sf(int in);
Mat lab_dct_naive(Mat input);
Mat lab_dct_opt(Mat input);

#endif

