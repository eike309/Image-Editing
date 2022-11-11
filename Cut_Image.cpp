// Programm which cuts out a rectangular ROI of a certain Size out of an image and saves it. For generating door training images

#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <string>
#include <opencv2/opencv.hpp>
#include<string.h>

using namespace std;
using namespace cv;

vector <Point> mouse_click;
Mat scaledImage;
Mat image;



// function which resizes the input map to a map with Box size 100x100
void scale_to_Box150 ( const InputArray &src,OutputArray &dst, float &Box_size)
{
    float scale = 150/Box_size;
    resize(src, dst,Size(), scale, scale, INTER_AREA);  //maybe other interpolation method better
}


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
 

// ---------------------------------- NEW TRACKBAR FUNCTION  ----- START
int maxScaleUp = 30;
int ROImaxScaleUp = 30;
int dumbMax = 2;
int scaleFactor = 1;
int ROIscaleFactor = 1;
int dumbFactor =1;
string windowName = "Resize Image";
string trackbarValue = "Scale";
string ROItrackbar = "R";
string dumptrackbar = "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO0000000000000000O";
// Callback functions
void scaleImage(int, void*)
{
	
    double scaleFactorDouble = scaleFactor; //1 + scaleFactor/100.0;
    
	// Set the factor to 1 if becomes 0
    if (scaleFactorDouble == 0)
	{
        scaleFactorDouble = 1;
    }
    
    // Resize the image
    resize(image, scaledImage, Size(), scaleFactorDouble, scaleFactorDouble, INTER_LINEAR);
    // Display the image
    imshow(windowName, scaledImage);


}


int main(int argc, char** argv)
{


// ---------------------------------- NEW TRACKBAR FUNCTION  ----- START

int number = 220;


std::ostringstream to_edit_img_name;
to_edit_img_name << "/home/bergen/Programme/pictures/00000/doors_tobe_edited/pos_img_sorted_"  << number << ".png";
image = imread("/home/bergen/Programme/pictures/scaled maps/smalldoors_IGMR.png");

// Create a window to display results and set the flag to Autosize
namedWindow(windowName, WINDOW_AUTOSIZE);

// Create Trackbars and associate a callback function
createTrackbar(trackbarValue, windowName, &scaleFactor, maxScaleUp, scaleImage);
createTrackbar(ROItrackbar, windowName, &ROIscaleFactor, ROImaxScaleUp);
createTrackbar(dumptrackbar, windowName, &dumbFactor, dumbMax);

scaleImage(25,0);

// Display image
imshow(windowName, image);

// ---------------------------------- NEW TRACKBAR FUNCTION  ----- END
     
     setMouseCallback(windowName, CallBackFunc, NULL); //callback function for Mouse event
     waitKey(0);
    // here is the rectangle set up
     Rect mouse_click_rect;
     float rect_size = ROIscaleFactor*scaleFactor;

     mouse_click_rect.x = mouse_click[0].x-rect_size/2;  //rect will be drawn around mouse click
     mouse_click_rect.y = mouse_click[0].y-rect_size/2;
     mouse_click_rect.width =   rect_size;
     mouse_click_rect.height = rect_size;
    //rectangle(img, mouse_click_rect,255, 3, LINE_8);  // draws the rect on the input image
    Mat rect_mat (scaledImage,mouse_click_rect); 
    resize(rect_mat,rect_mat, Size(30,30));
    imshow("My Window", rect_mat);

std::ostringstream edited_img_name;
edited_img_name << "/home/bergen/Programme/pictures/00000/Aufbewahrung/pos_img_b_" << number<< ".png";
imwrite("/home/bergen/Programme/pictures/scaled maps/Smalldoor_IGMR.png", rect_mat);
   waitKey(0);
destroyAllWindows();


     return 0;

}