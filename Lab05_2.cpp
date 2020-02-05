//
//  Lab05_2.cpp
//  Lab05
//
//

#include <opencv2/opencv.hpp>
#include <iostream>


const std::string imageName = "unnamed.jpg";
const std::string windowName_0 = "Source Image";
const std::string windowName_1 = "Processed Image";

float getUserEntry (std::string message);
void applyImpulsiveNoise(cv::Mat src, cv::Mat& dest, float probability);

int main( /** No argument is needed */ ) {
    
    // read the image from the project directory
    cv::Mat image = imread(imageName, cv::IMREAD_COLOR);
    // Check if could find the image
    if( image.empty() ) {
        std::cout << "Could not read the image" << std::endl ;
        return 1;
    }
     
    // prepare for a salt-pepper image from the first image
    float probability;
    cv::Mat copy(image.rows, image.cols, CV_32FC3);
    image.convertTo(copy, CV_32FC3, 1.0/255.0);
    cv::Mat saltPepper(image.rows, image.cols, CV_32FC3);
    
    // define named windows
    namedWindow(windowName_0, cv::WINDOW_AUTOSIZE);
    namedWindow(windowName_1, cv::WINDOW_AUTOSIZE);
    cv::moveWindow(windowName_1, 300, 100);
        
    // while user didn't enter exit sign on command line, or did not press any keys
    std::cout << "Enter 9 to go to the next demo!" << std::endl;
    while( cv::waitKey(33) < 0 ) {
        // display and update the image in the window
        imshow(windowName_0, image);
        cv::waitKey(33);
        
        probability = getUserEntry("Enter probability [0 to 1]: ");
        if (probability == 9.0) break;
        
        applyImpulsiveNoise(copy, saltPepper, probability);
        
        // display and update the changed image in the window
        imshow(windowName_1, saltPepper);
        cv::waitKey(33);
    }
    
    // store a salt-peper with probability = 0.1
    std::cout << "Going to make a noisy image with probability = 0.1\n\t..." << std::endl;
    applyImpulsiveNoise(copy, saltPepper, 0.1);
    imshow(windowName_0, saltPepper);
    cv::waitKey(33);
    std::cout << "Done!" << std::endl;
    
    // smooth with 3x3 box kernel
    std::cout << "Going to filter noisy image with 3x3 box kernel.\n\t..." << std::endl;
    cv::Matx<float,3,3> kernel = cv::Matx<float,3,3>::all(1.0f / 9.0f);
    filter2D(saltPepper, copy, CV_32FC3, kernel);
    imshow(windowName_1, copy);
    cv::waitKey(33);
    std::cout << "Done!" << std::endl;
    
    // print and compare
    std::cout << "Noisy/Filtered values for pixel at 300,300 - blue chanel: " << std::endl;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            int row = 300+i-2;
            int col = 300+j-2;
            std::cout << "   Noisy(" << row << ", " << col << "): ";
            std::cout << saltPepper.at<cv::Vec3f>(row, col)[0] << std::endl;
            std::cout << "Filtered(" << row << ", " << col << "): ";
            std::cout << copy.at<cv::Vec3f>(row, col)[0] << std::endl;
        }
        std::cout << std::endl;
    }
    
    if ( getUserEntry("Enter 9 to continue! ") == 9 );
    
    // bilinear 3x3 box kernel
    std::cout << "Going to filter noisy image with bilinear 3x3 box kernel.\n\t..." << std::endl;
    cv::Matx bilinearC = cv::Matx<float,1,3>({1,2,1});
    cv::Matx bilinearR = cv::Matx<float,3,1>({1,2,1});
    cv::Matx<float,3,3> kernel_ = cv::Matx<float,3,3>::all(1.0f / 16.0f);
    kernel_ = kernel_.mul(bilinearR * bilinearC);
    filter2D(saltPepper, copy, CV_32FC3, kernel_);
    imshow(windowName_1, copy);
    cv::waitKey(33);
    std::cout << "Done!" << std::endl;
    
    if ( getUserEntry("Enter 9 to continue! ") == 9 );
    
    // median filter
    std::cout << "Going to filter noisy image with median blur 3x3 box kernel.\n\t..." << std::endl;
    medianBlur(saltPepper, copy, 3);
    imshow(windowName_1, copy);
    cv::waitKey(33);
    std::cout << "Done!" << std::endl;
    std::cout << "That was it!" << std::endl;
       
    if ( getUserEntry("Enter 9 to exit! ") == 9 );
    
    cv::destroyAllWindows();
    return 0;
    
}

float getUserEntry (std::string message) {
    float answer = -1.0;
    while ( answer == -1.0 ) {
        std::cout << message;
        std::cin >> answer;
        if (!std::cin.fail() && answer >= 0.0 && answer <= 1.0) /** do nothing */;
        else if ( answer == 9.0 ) /** do nothing */;
        else {
            std::cin.clear();
            std::cout << "Please try again.\n" << std::endl;
            answer = -1.0;
        }
        /** flush the buffer */
        std::cin.ignore(INT_MAX, '\n');
    }
    return answer;
}

void applyImpulsiveNoise(cv::Mat src, cv::Mat& dest, float probability) {
    cv::Mat x(src.rows, src.cols, CV_32FC1);
    cv::RNG range;
    range.fill(x, cv::RNG::UNIFORM, 0.0f, 1.0f);
    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            cv::Vec3f& s_ = dest.at<cv::Vec3f>(i,j);
            cv::Vec3f img = src.at<cv::Vec3f>(i,j);
            float x_ = x.at<float>(i,j);
            if (x_ < probability) {
                float y = range.uniform(0.0f, 1.0f);
                s_[0] = s_[1] =  s_[2] =  y * 255;
            }
            else {
                s_[0] = img[0];
                s_[1] = img[1];
                s_[2] = img[2];
            }
        }
    }
}
