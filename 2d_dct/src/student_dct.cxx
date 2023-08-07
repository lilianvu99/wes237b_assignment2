#include "main.h"


using namespace cv;
using namespace std;

Mat LUT_w;
Mat LUT_h;


// Helper function
float sf(int in){
	if (in == 0)
		return 0.70710678118; // = 1 / sqrt(2)
	return 1.;
}

// Initialize LUT
void initDCT(int WIDTH, int HEIGHT)
{
	// TODO
	//did a loop n^2
	//two sets of two for loops
	//one iterate x and i values and calculate cos value and putting into table
	//second set y and j and calculate cos value lut_w
	//scale it appropriately 
	//multiply scale while put values in

	LUT_h = Mat(HEIGHT, WIDTH, CV_32FC1);
	LUT_w = Mat(HEIGHT, WIDTH, CV_32FC1);	
	float* LUT_h_ptr = LUT_h.ptr<float>();
        float* LUT_w_ptr  = LUT_w.ptr<float>();
	float scale = 2./sqrt(HEIGHT*WIDTH);

	for (int x = 0; x < HEIGHT; x++) {
		for (int i = 0; i < WIDTH; i++) {
			LUT_h_ptr[x * WIDTH + i] = scale * sf(x) * cos(M_PI/((float)HEIGHT)*(i+1./2.)*(float)x);
		}	
	}

	for (int y = 0; y < HEIGHT; y++) {
		for (int j = 0; j < WIDTH; j++) {
			LUT_w_ptr[y * WIDTH + j] = sf(y) * cos(M_PI/((float)WIDTH)*(j+1./2.)*(float)y);
		}
	}
	
}

// Baseline: O(N^4)
Mat student_dct(Mat input)
{
	const int HEIGHT = input.rows;
	const int WIDTH  = input.cols;

	Mat result = Mat(HEIGHT, WIDTH, CV_32FC1);

	// Note: Using pointers is faster than Mat.at<float>(x,y)
	// Try to use pointers for your LUT as well
	float* result_ptr = result.ptr<float>();
	float* input_ptr  = input.ptr<float>();
	float* LUT_w_ptr = LUT_w.ptr<float>();
	float* LUT_h_ptr = LUT_h.ptr<float>();

	float scale = 2./sqrt(HEIGHT*WIDTH);
	
	for(int x = 0; x < HEIGHT; x++)
	{
		for(int y = 0; y < WIDTH; y++)
		{
			float value = 0.f;

			for(int i = 0; i < HEIGHT; i++)
			{
				for(int j = 0; j < WIDTH; j++)
				{
					#ifdef LUT
						value += input_ptr[i * WIDTH + j] * LUT_h_ptr[x * WIDTH + i] * LUT_w_ptr[y * WIDTH + j];

					#elif defined REG
						value += input_ptr[i*WIDTH + j] * cos(M_PI/((float)HEIGHT)*(i+1./2.)*(float)x)
                                                * cos(M_PI/((float)WIDTH)*(j+1./2.)*(float)y);
					#endif
					// TODO
						// --- Replace cos calculation by LUT ---
						// input_ptr*lut_h*lut_w
						//* cos(M_PI/((float)HEIGHT)*(i+1./2.)*(float)x)
						//* cos(M_PI/((float)WIDTH)*(j+1./2.)*(float)y);
				}
			}
			// TODO
			// --- Incorporate the scale in the LUT coefficients ---
			// --- and remove the line below ---
			// multiply sf(x) by every cos of table 
			// lut_h multiplied by sf(x) for approp x value
			// chose one table to multiply the scale value 
			// scale is same value getting multiplied to every value 
			// only get multiplied by scale factor once 
			// sf(x)*lut_h sf(y)*lut_w
			// scale gets handled in init dct
			#ifdef REG
				value = scale * sf(x) * sf(y) * value;
			#endif

			result_ptr[x * WIDTH + y] = value;
		}
	}

	return result;
}


// *****************
//   Hint
// *****************
//
// DCT as matrix multiplication

//make a generic matric mult and call the LUT
//matlumt(lut, gray)
//matmult(output, lut transpose)
Mat mat_mult(Mat input1, Mat input2) {
	const int height1 = input1.rows;
        const int width1 = input1.cols;

        const int height2 = input2.rows;
        const int width2 = input2.cols;
	
	Mat result = Mat(input1.rows, input2.cols, CV_32FC1);
        float* input1_ptr = input1.ptr<float>();
        float* input2_ptr = input2.ptr<float>();
        float* result_ptr = result.ptr<float>();

	for (int i = 0; i < result.rows; i++) {
                for (int j = 0; j < result.cols; j++) {
                        float value = 0.0;
                        for (int k = 0; k < width1; k++) {
                                value += input1_ptr[i*width1 + k] * input2_ptr[k*width1 + j];
                         //      cout << value << endl;
                        }
                        result_ptr[i*width1 + j] = value;
                }
        }

	return result;



}

Mat block_mat_mult(Mat input1, Mat input2) {
		
	const int height1 = input1.rows;
        const int width1 = input1.cols;

        const int height2 = input2.rows;
        const int width2 = input2.cols;
	
        Mat result = Mat(height1, width2, CV_32FC1, Scalar(0));
        float* input1_ptr = input1.ptr<float>();
        float* input2_ptr = input2.ptr<float>();
        float* result_ptr = result.ptr<float>();

                const int block_size = 64;
                int num_chunks = width1/block_size;
		int N = width1;

		for (int jj = 0; jj < (N); jj+=block_size) {
                	for (int kk = 0; kk < (N); kk+=block_size) {
                        	for (int i = 0; i < (N); i++) {
                                	for (int j = jj; j < ((jj+block_size)>N?N:(jj+block_size)); j++) {
						float value = 0.0;
                                        	for (int k = kk; k < ((kk+block_size)>N?N:(kk+block_size)); k++) {
                                                	value += input1_ptr[i*width1+k] * input2_ptr[k*width1+j];
                                        	}
						result_ptr[i*width2+j] += value;
                               	 	}
				
                        	}	

                	}
		}

		return result;

}

//5 for loops for BMM
//google BMM 
//
//function call MM inside 
//1st for loop get the block
//2nd for loop iterate thru block
//inside 2nd loop, call the MM function

Mat lilian_matmult(Mat input)
{
	const int height1 = input.rows;
        const int width1 = input.cols;

	int index_i = 0;
	int index_j = 0;

	// -- Works only for WIDTH == HEIGHT
	assert(width1 == height1);
	
	// -- Matrix multiply with OpenCV
	Mat result1, result;

	#ifdef LUT_MULT
	result1 = mat_mult(LUT_h, input); //with scale
	result = mat_mult(result1, LUT_w.t()); //without scale
	//using OpenCV
	//result = LUT_h*input*LUT_w.t();
	
	#elif defined BLOCK
	result1 = block_mat_mult(LUT_h, input);
	result = block_mat_mult(result1, LUT_w.t());	
	
	#elif defined REG
        result = mat_mult(input, input);	
	#endif

	// TODO
	// Replace the line above by your own matrix multiplication code
	// You can use a temp matrix to store the intermediate result
	
	
	
	return result;
}






