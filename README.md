# Image-Editing
Tools for a Bachelor's thesis in which a training set was created on which an SVM for object detection was then trained.
The training set consists of images of objects from a floor plan.

Cut_Image.cpp:                This tool uses a trackbar and scaling to cut small objects out of a floor plan and convert
                              them to the same size. Used for creation of training set.
                              
Image_Data_Augmentation.cpp:  This tool rotates and mirrors all images from the specified folders to enlarge the data set.
                              
