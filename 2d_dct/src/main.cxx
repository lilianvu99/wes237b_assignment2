#include "main.h"
#include "timer.h"

#define FRAME_NUMBER 5 //set to 0 or -1 to run while loop
//#define CUST_DCT
//#define MATMULT

using namespace std;
using namespace cv;

int main(int argc, const char * argv[])
{
	unsigned int c_start;
	unsigned int opencv_c, student_c;

	cout << "WES237B lab 2" << endl;

	VideoCapture cap("input.raw");

	Mat frame, gray, dct_org, dct_student, lilian_mat, diff_img, diff_matmult, ideal_matmult;
	Mat inverse;
	char key=0;
	float mse, mat_mse;
	int fps_cnt = 0;

	int WIDTH  = 64;
	int HEIGHT = 64;

	// 1 argument on command line: WIDTH = HEIGHT = arg
	if(argc >= 2)
	{
		WIDTH = atoi(argv[1]);
		HEIGHT = WIDTH;
	}
	// 2 arguments on command line: WIDTH = arg1, HEIGHT = arg2
	if(argc >= 3)
	{
		HEIGHT = atoi(argv[2]);
	}

	initDCT(WIDTH, HEIGHT);

	float avg_perf = 0.f;
	int count = 0;

#if FRAME_NUMBER <= 0
	while(key != 'q')
#else
    for(int f = 0; f < FRAME_NUMBER; f++)
#endif
	{
		cap >> frame;
		if(frame.empty()){ break; }

		cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
		resize(gray, gray, Size(WIDTH, HEIGHT));
		gray.convertTo(gray, CV_32FC1);


		LinuxTimer t;
		float myTimer;
		#ifdef CUST_DCT
			t.start();
			printf("----------------CUSTOM STUDENT DCT----------\n");
			#ifdef REG
			printf("W/ REG...\n");
			#endif
			#ifdef LUT 
				printf("W/ LUT...\n");
			#endif
			dct(gray, dct_org);
			dct_student = student_dct(gray);
			absdiff(dct_org, dct_student, diff_img); 
			diff_img = diff_img.mul(diff_img);
			Scalar se = sum(diff_img);
			mse = se[0]/((float)HEIGHT*WIDTH);
			printf("RMSE: %.4f\n", sqrt(mse));
		
			idct(dct_student, inverse);
			inverse.convertTo(inverse, CV_8U);

			t.stop();
                	myTimer = t.getElapsed();

               	 	cout <<  "Execute time: " << (double)myTimer/1000000000.0 << endl;
		#endif
		
		#ifdef MATMULT
			printf("--------------MATRIX MULTIPLICATION---------\n");
			t.start();
			#ifdef LUT_MULT 
				printf("W/ LUT...\n");
				dct(gray, ideal_matmult);
			#elif defined BLOCK
				printf("W/ BLOCK... \n");
				dct(gray, ideal_matmult);
			#elif defined REG
				printf("W/ REGULAR...\n");
				ideal_matmult = gray*gray;
			#endif
			lilian_mat = lilian_matmult(gray);
			absdiff(ideal_matmult, lilian_mat, diff_matmult); 
			diff_matmult = diff_matmult.mul(diff_matmult);
			Scalar mat_se = sum(diff_matmult);
			mat_mse = mat_se[0]/((float)HEIGHT*WIDTH);
			printf("MATMULT RMSE: %.4f\n", sqrt(mat_mse));

			idct(lilian_mat, inverse);
			inverse.convertTo(inverse, CV_8U);

			t.stop();
			myTimer = t.getElapsed();

                	cout <<  "Execute time: " << (double)myTimer/1000000000.0 << endl;
		
		#endif
		

		gray.convertTo(gray, CV_8UC1);


#ifndef __arm__
		imshow("Original", gray);
		imshow("IDCT Output", inverse);
		moveWindow("IDCT Output", 500, 0);
		key = waitKey(30);
#endif
	}

	return 0;
}




