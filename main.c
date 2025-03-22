#include <stdio.h>
#include <stdlib.h>
#include <emmintrin.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

int main(int argc, char** argv) {

    // --- Comienza plantilla base proporcionada --- //

    if (argc != 2) {
        printf("Usage: %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Carga imagen desde archivo (Plantilla)
    IplImage* Img1 = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);

    // Verifica que la imagen se haya cargado correctamente (Plantilla)
    if (!Img1) {
        printf("Error: fichero %s no leido\n", argv[1]);
        return EXIT_FAILURE;
    }

    // Crea ventana para mostrar la imagen (Plantilla)
    cvNamedWindow(argv[1], CV_WINDOW_NORMAL);

    // --- Termina plantilla base proporcionada --- //

    // --- Comienza funcionalidad específica añadida --- //

    // Crea una imagen con un canal alpha adicional
    IplImage* Img2 = cvCreateImage(cvSize(Img1->width, Img1->height), IPL_DEPTH_8U, 4);

    __m128i mascaraAlpha = _mm_set1_epi32(0xFF000000);
    int i, col;
    int v;

    // Recorre cada fila de la imagen original
    for (i = 0; i < Img1->height; i++){
        __m128i* pImg1 = (__m128i*)(Img1->imageData + i * Img1->widthStep);
        __m128i* pImg2 = (__m128i*)(Img2->imageData + i * Img2->widthStep);

        // Recorre cada grupo de píxeles usando SIMD (16 bytes por iteración)
        for (col = 0; col < Img1->widthStep; col += 16){
            // Extrae el canal alpha usando una máscara
            __m128i byteAlpha = _mm_and_si128(*pImg1, mascaraAlpha);

            // Propaga el canal alpha a los canales RGB de la nueva imagen
            for(v = 1; v < 4; v++) {
                *pImg2 = _mm_or_si128(_mm_srli_epi32(byteAlpha, 8*v), *pImg2);
            }

            pImg1++;
            pImg2++;
        }
    }

    // Muestra la imagen con canal alpha en la ventana creada
    cvShowImage(argv[1], Img2);
    cvWaitKey(0);

    // Libera la memoria usada por las imágenes
    cvReleaseImage(&Img1);
    cvReleaseImage(&Img2);

    // Destruye la ventana creada
    cvDestroyWindow(argv[1]);

    // --- Termina funcionalidad específica añadida --- //

    return EXIT_SUCCESS;
}
