# WES-237B Assignment 2
Lilian Vu
A14491862

CHANGE SETTINGS FOR 2D DCT PROGRAM INSIDE /include/main.h
	
 	#define MATMULT  //to enable matrix multiplication -- choose either BLOCK or LUT_MULT, not BOTH
	#define BLOCK    //to enable block matrix multiplication 
	#define LUT_MULT //to enable matrix multipliation with LUT 

	#define CUST_DCT //to enable student DCT -- chose either LUT or REG, not BOTH
	#define LUT      //to enable LUT usage inside student DCT
	#define REG      //to revert back to original version of student DCT without LUTs

FILES:

	2d_dct - folder with part 3 and part 4 code
	2d_dct_BMM.zip - zip folder with settings already configured to run Block Matrix Multiplication
	2d_dct_LUT.zip - zip file with settings already configured to run LUT for DCT and LUT for Matrix Multiplication
	2d_dct_regular.zip - zip file with settings to run naive matrix multiplication and regulat student DCT before any modifications
	dct - folder with part 2 code
	lab_pynq_basics - folder with part 1 code
 
	README.md - this file lol
 
	Vu_Lilian_WES237B_Assignment2.pdf - Analysis PDF on deliverables and screenshots of output

