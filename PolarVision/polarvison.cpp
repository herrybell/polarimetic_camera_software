#include"polarvison.h"
#include "omp.h"
cv::Mat Dolp(Mat I0, Mat I45,Mat I90,Mat I135)
{      Mat L_out(1024, 1224, CV_32FC3);
       std::vector<Mat>I0_S;
       cv::split(I0, I0_S);
       std::vector<Mat>I45_S;
       cv::split(I45, I45_S);
       std::vector<Mat>I90_S;
       cv::split(I90, I90_S);
       std::vector<Mat>I135_S;
       cv::split(I135, I135_S);
       std::vector<Mat>L_out_S;
       cv::split(L_out, L_out_S);

   #pragma omp parallel for num_threads(3)
       for (int i = 0; i < 3; i++)
       {   //all_starts=clock();
           I0_S[i].convertTo(I0_S[i], CV_32FC1);
           I45_S[i].convertTo(I45_S[i], CV_32FC1);
           I90_S[i].convertTo(I90_S[i], CV_32FC1);
           I135_S[i].convertTo(I135_S[i], CV_32FC1);
           int row = I0_S[i].rows;
           int col = I0_S[i].cols;

           Mat S0 = (I0_S[i] + I45_S[i] + I90_S[i] + I135_S[i]) / 2;
           Mat S1 = (I0_S[i] - I90_S[i]) ;
           Mat S2 = (I45_S[i] - I135_S[i]) ;

           for (int m = 0; m < row; m++)

           {

               for (int n = 0; n < col; n++)

               {
                   L_out_S[i].at<float>(m, n) =sqrt(S1.at<float>(m, n)*S1.at<float>(m, n)+S2.at<float>(m, n)*S2.at<float>(m, n))/S0.at<float>(m, n) ;
               }
           }


       }

       merge(L_out_S, L_out);

       L_out.convertTo(L_out, CV_8UC3);
       return L_out*255;
}


cv::Mat Aop(Mat I0, Mat I45,Mat I90,Mat I135)
{
    Mat L_out(1024, 1224, CV_32FC3);
         std::vector<Mat>I0_S;
         cv::split(I0, I0_S);
         std::vector<Mat>I45_S;
         cv::split(I45, I45_S);
         std::vector<Mat>I90_S;
         cv::split(I90, I90_S);
         std::vector<Mat>I135_S;
         cv::split(I135, I135_S);
         std::vector<Mat>L_out_S;
         cv::split(L_out, L_out_S);

    #pragma omp parallel for num_threads(3)
         for (int i = 0; i < 3; i++)
{
             I0_S[i].convertTo(I0_S[i], CV_32FC1);
             I45_S[i].convertTo(I45_S[i], CV_32FC1);
             I90_S[i].convertTo(I90_S[i], CV_32FC1);
             I135_S[i].convertTo(I135_S[i], CV_32FC1);
             int row = I0_S[i].rows;
             int col = I0_S[i].cols;


             Mat S1 = (I0_S[i] - I90_S[i]) ;
             Mat S2 = (I45_S[i] - I135_S[i]) ;

            Mat theta(row, col, CV_32FC1);
             for (int i = 0; i < row; i++)

             {

                 for (int j = 0; j < col; j++)

                 {

                     theta.at<float>(i, j) = floor( 28.662 * atan(S2.at<float>(i, j) / S1.at<float>(i, j)));


                     if (S1.at<float>(i, j) > 0)
                     {
                         if (S2.at<float>(i, j) <= 0)
                         {
                             theta.at<float>(i, j) = theta.at<float>(i, j) + 180;
                         }
                     }
                     else if (S1.at<float>(i, j) < 0)

                     {
                         theta.at<float>(i, j) =  theta.at<float>(i, j) + 90;
                     }
                     else if (S1.at<float>(i, j) == 0)
                     {
                         if (S2.at<float>(i, j) < 0)
                         {
                              theta.at<float>(i, j) = theta.at<float>(i, j) + 180;
                         }
                         else if (S2.at<float>(i, j) == 0)
                         {
                            theta.at<float>(i, j) = -1;
                         }
                     }




                 }


            }

             L_out_S[i]=theta;

         }
         merge(L_out_S, L_out);

         L_out.convertTo(L_out, CV_8UC3);
         return L_out*255;
}
