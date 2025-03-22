#include <stdio.h>
#include <stdlib.h>
#include <emmintrin.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

int main(int argc, char** argv) {

    if (argc != 2) {
        printf("Usage: %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    //CV_LOAD_IMAGE_COLOR = 1 forces the resultant IplImage to be colour.
    //CV_LOAD_IMAGE_GRAYSCALE = 0 forces a greyscale IplImage.
    //CV_LOAD_IMAGE_UNCHANGED = -1
    IplImage* Img1 = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
    IplImage* Img2 = cvCreateImage(cvSize(Img1->width, Img1->height), IPL_DEPTH_8U, 4);
    // Always check if the program can find a file
    if (!Img1) {
        printf("Error: fichero %s no leido\n", argv[1]);
        return EXIT_FAILURE;
    }
     
    // a visualization window is created with title 'image'
    cvNamedWindow(argv[1], CV_WINDOW_NORMAL);
    __m128i mascaraAlpha = _mm_set1_epi32(0xFF000000);
    int i, col;
    int j, v;
    for (i = 0; i < Img1->height ; i++){
        __m128i* pImg1 = (Img1->imageData + i*Img1->widthStep);
        __m128i* pImg2 = (Img2->imageData + i*Img2->widthStep);
        for (col = 0; col < Img1->widthStep ; col+=16){
            __m128i byteAlpha = _mm_and_si128(*pImg1,mascaraAlpha);
            for(v = 1; v < 4; v++) {
                *pImg2 = _mm_or_si128(_mm_srli_epi32(byteAlpha, 8*v),*pImg2);
            }
            pImg1++;
            pImg2++;
        }
    }
    // img is shown in 'image' window
    cvShowImage(argv[1], Img2);
    cvWaitKey(0);
 

    // memory release for img before exiting the application
    cvReleaseImage(&Img1);
    cvReleaseImage(&Img2);

    // Self-explanatory
    cvDestroyWindow(argv[1]);

    return EXIT_SUCCESS;

}