//
//  Lab04_1.cpp
//  Lab04
//
//

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

const std::string imageName_1 = "unnamed.jpg";
const std::string imageName_2 = "bridge-and-trees.jpg";
const std::string windowName_original_1 = "original image 1";
const std::string windowName_original_2 = "original image 2";
const std::string windowName_light = "light image";
const std::string windowName_sharp = "sharp image";
const std::string windowName_blend = "blend image";


char AorB();
double getAlpha();

int main ( /** No argument is needed */ ) {
    // read the image from the project directory
    cv::Mat image_1 = imread(imageName_1, cv::IMREAD_COLOR);
    cv::Mat image_2 = imread(imageName_2, cv::IMREAD_COLOR);
    // Check for invalid input
    if( image_1.empty() || image_2.empty() )
    {
        std::cout << "Could not read the image" << std::endl ;
        return 1;
    }
    // define named windows
    namedWindow(windowName_original_1, cv::WINDOW_AUTOSIZE);
    namedWindow(windowName_original_2, cv::WINDOW_AUTOSIZE);
    namedWindow(windowName_light, cv::WINDOW_AUTOSIZE);
    namedWindow(windowName_sharp, cv::WINDOW_AUTOSIZE);
    namedWindow(windowName_blend, cv::WINDOW_AUTOSIZE);
    cv::moveWindow(windowName_light, 200, 0);
    cv::moveWindow(windowName_sharp, 400, 0);
    cv::moveWindow(windowName_blend, 400, 0);
    // make image with +100 brightnes of original
    cv::Mat image_light = image_1 + cv::Scalar(100,100,100);
    // make image with *1.5 brightnes of original
    cv::Mat image_sharp = image_1 * 1.5;
    // make blend image with original images
    cv::Mat image_blend;
    
    char option = AorB();
    
    if (option == 'A') {
        /** while space-bar key (ASCII 32 ) has not pressed  show the image  */
        while(cv::waitKey(33) != 32) {
            // display the image in the window
            imshow(windowName_original_1, image_1);
            // display the image with +100 brightness
            imshow(windowName_light, image_light);
            // display the image with *1.5 sharpness
            imshow(windowName_sharp, image_sharp);
        }
        std::cout << "Original image at pixel 100,100: " << std::endl;
        std::cout << "      Blue:  " << (int) image_1.at<cv::Vec3b>(100,100)[0] << std::endl;
        std::cout << "      Green: " << (int) image_1.at<cv::Vec3b>(100,100)[1] << std::endl;
        std::cout << "      Red:   " << (int) image_1.at<cv::Vec3b>(100,100)[2] << std::endl;
        
        std::cout << "Light image at pixel 100,100: " << std::endl;
        std::cout << "      Blue:  " << (int) image_light.at<cv::Vec3b>(100,100)[0] << std::endl;
        std::cout << "      Green: " << (int) image_light.at<cv::Vec3b>(100,100)[1] << std::endl;
        std::cout << "      Red:   " << (int) image_light.at<cv::Vec3b>(100,100)[2] << std::endl;
        
        std::cout << "Sharp image at pixel 100,100: " << std::endl;
        std::cout << "      Blue:  " << (int) image_sharp.at<cv::Vec3b>(100,100)[0] << std::endl;
        std::cout << "      Green: " << (int) image_sharp.at<cv::Vec3b>(100,100)[1] << std::endl;
        std::cout << "      Red:   " << (int) image_sharp.at<cv::Vec3b>(100,100)[2] << std::endl;
        
    }
    else {
       /** while space-bar key (ASCII 32 ) has not pressed  show the image  */
        double alpha;
        bool answered = false;
        while(cv::waitKey(33) != 32) {
            // display the first image
            imshow(windowName_original_1, image_1);
            // display the second image
            imshow(windowName_original_2, image_2);
            
            if (!answered) {
                alpha = getAlpha();
                answered = true;
                // to make blend image we can do
                // image_blend = ((1 - alpha) * image_1) + (alpha * image_2);
                // changing the size of the background (image_2) to the same size as forground (image_1)
                int center_box_x = (image_2.rows - image_1.rows)/2;
                int center_box_y = (image_2.cols - image_1.cols)/2;
                cv::Mat backgroundImage = image_2(cv::Rect(center_box_x, center_box_y, image_1.cols, image_1.rows));
                addWeighted(backgroundImage, (1 - alpha), image_1, alpha, 0.0, image_blend, -1);
            }
            
            // display the blend image
            imshow(windowName_blend, image_blend);
        }
        
        const int chanels[] = {0}; // only blue
        const int histSize[] = {8};
        const float hranges[] = {0, 180};
        const float sranges[] = {0, 256};
        const float* ranges[] = {hranges , sranges};
        cv::Mat hist;
        cv::calcHist(&image_blend, 1, chanels, cv::Mat(), hist, 1, histSize, ranges, true, false);
        
        std::cout << "Histogram values for blue chanel of the blended image: " << std::endl;
        std::cout << hist << std::endl;
        
    }
    
    // destroy windows
    cv::destroyWindow(windowName_original_1);
    cv::destroyWindow(windowName_original_2);
    cv::destroyWindow(windowName_light);
    cv::destroyWindow(windowName_sharp);
    cv::destroyWindow(windowName_blend);

    return 0;
}


char AorB() {
    char answer = '\0';
    while (answer == '\0') {
        std::cout << "Enter one option (A/B): ";
        std::cin >> answer;
        /** flush the buffer */
        std::cin.ignore(INT_MAX, '\n');
        if (toupper(answer) == 'A' || toupper(answer) == 'B') /** do nothing */;
        else {
            std::cout << "Please enter only A or B. Try again.\n" << std::endl;
            answer = '\0';
        }
    }
    return toupper(answer);
}

double getAlpha() {
    double answer = -1.0;
    while (answer == -1.0) {
        std::cout << "Enter Alpha (0 to 1): ";
        std::cin >> answer;
        if (!std::cin.fail() && answer >= 0.0 && answer <= 1.0) /** do nothing */;
        else {
            std::cin.clear();
            std::cout << "Please enter a number between [0,1]. Try again.\n" << std::endl;
            answer = -1.0;
        }
        /** flush the buffer */
        std::cin.ignore(INT_MAX, '\n');
    }
    return answer;
}
