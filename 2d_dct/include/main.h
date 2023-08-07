//
//  main.h
//  Lab2
//
//  Created by Alireza on 7/6/16.
//  Copyright © 2016 Alireza. All rights reserved.
//

#ifndef main_h
#define main_h

#include <iostream>
#include <stdio.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <math.h>

using namespace cv;

//#define WIDTH 64
//#define HEIGHT 64

#define MATMULT  //to enable matrix multiplication -- choose either BLOCK or LUT_MULT, not BOTH
#define BLOCK	 //to enable block matrix multiplication 
//#define LUT_MULT //to enable matrix multipliation with LUT 

//#define CUST_DCT //to enable student DCT -- chose either LUT or REG, not BOTH
//#define LUT 	   //to enable LUT usage inside student DCT
//#define REG	   //to revert back to original version of student DCT without LUTs

/* student DCT */
Mat student_dct(Mat input);
void initDCT(int WIDTH, int HEIGHT);
Mat mat_mult(Mat input1, Mat input2);
Mat block_mat_mult(Mat input1, Mat input2);
Mat lilian_matmult(Mat input);
#endif /* main_h */
