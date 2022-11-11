//Tool to create more Training Images by rotating/mirroring them
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/ml.hpp>
#include <opencv2/tracking.hpp>
#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;
using namespace cv::ml;

string windowName = "Image";
vector <Point> mouse_click;



void CallBackFunc(int event, int x, int y, int flags, void*)
{
     if  ( event == EVENT_LBUTTONDOWN )
     { 
       mouse_click.push_back(Point(x,y));
     }
}

//load all images from a directory //some parts from: https://docs.opencv.org/master/d0/df8/samples_2cpp_2train_HOG_8cpp-example.html#_a29
void load_train_images(vector<Mat> &train_img_lst, String &img_path) 
{
    vector<cv::String> fn;
    glob(img_path, fn, false);
    size_t count = fn.size(); //number of png files in images folder
    for (size_t i=0; i<count; i++)
     {
        Mat fn_img = imread( fn[i] ); // load the image

        if ( fn_img.empty() )
        {
            cout << fn[i] << " is invalid!" << endl; // invalid image, skip it.
            continue;
        }
       /* int imgScale = 3;
        resize(fn_img, fn_img, Size(fn_img.cols * imgScale, fn_img.rows * imgScale));   /// resizing try by me
        */
        train_img_lst.push_back(fn_img);
     }
    cout << train_img_lst.size() << endl;
}

//SOurce: https://cppsecrets.com/users/204211510411798104971091085153504964103109971051084699111109/C00-OpenCV-to-rotate-an-image.php
Mat rotate(Mat src, double angle)   //rotate function returning mat object with parametres imagefile and angle    
{
    Mat dst;      //Mat object for output image file
    Point2f pt(src.cols/2., src.rows/2.);          //point from where to rotate    
    Mat r = getRotationMatrix2D(pt, angle, 1.0);      //Mat object for storing after rotation
    warpAffine(src, dst, r, Size(src.cols, src.rows));  ///applie an affine transforation to image.
    return dst;         //returning Mat object for output image file
}

main()
{
vector<Mat> all_images;
vector<Mat> img_lst;
String img_path = "/home/bergen/Programme/pictures/new_detections";
load_train_images(img_lst, img_path);

    for( size_t i = 0 ; i < img_lst.size(); i++ )
    {
        Mat rect_mat = img_lst[i];

    //mirror image
    Mat rect_mat_mirrored;
    flip(rect_mat,rect_mat_mirrored,1);
    all_images.push_back(rect_mat_mirrored);

    //create 3 rotated copys and push them in vector
    Mat rect_mat_90 = rotate(rect_mat,90); Mat rect_mat_180 = rotate(rect_mat,180);
    Mat rect_mat_270 = rotate(rect_mat,270);
    all_images.push_back(rect_mat_90);all_images.push_back(rect_mat_180);all_images.push_back(rect_mat_270);

    //create 3 copys and rotate them (of the mirrored image)
    Mat rect_mat_mirrored_90 = rotate(rect_mat_mirrored,90);Mat rect_mat_mirrored_180 = rotate(rect_mat_mirrored,180);
    Mat rect_mat_mirrored_270 = rotate(rect_mat_mirrored,270);
    all_images.push_back(rect_mat_mirrored_90);all_images.push_back(rect_mat_mirrored_180);all_images.push_back(rect_mat_mirrored_270);
 
    }

for ( size_t j = 0; j < all_images.size(); j++ )
        {
            // extension to save all variations as pngs
            Mat for_img3 =all_images[j];

            std::ostringstream name;
            name << "/home/bergen/Programme/pictures/00000/0_NEGATIVES/FINAL_NEGATIVES_IGMR_HM/FINAL_neg_img_IGMR_HM_f_mirr_rot" << j << ".png"; //CMP_difficult_door_4_
            imwrite(name.str(), for_img3);
        }
        return 0;
}