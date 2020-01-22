//
//  Lab03.cpp
//  Lab03
//
//

#include <opencv2/opencv.hpp>
#include <iostream>

cv::Rect getInnerRect(cv::Size currentImageSize);
void displayColourMenu();
cv::Scalar getColour();
bool shouldContinue();
cv::Point getAPoint(std::string message);
//void paint(cv::Rect, cv::Scalar, cv::Mat&); //used cv::rectangle method instead


int main( /** No argument is needed */ ) {
    // a
    /**
     * define an image (Matrix) of size 480*640 pixels
     * of type `8-bits Unsigned Integer with 3 chanells` (8UC3)
     * initialized all to zero by default
     */
    cv::Mat image = cv::Mat::zeros(480, 640, CV_8UC3);
    
    bool rendre = true;
    bool keepGoing = true;
    bool windowSizeChanged = true;
    std::string windowName = "Lab03";
    std::string guide;
    cv::Rect innerRect;
    cv::Scalar colour;
    cv::Size currentImageSize = image.size();
    cv::Size previousImageSize = image.size();
    cv::Point aPoint;
    int keyPressed_ASCII;
    
    /** display an introduction */
    std::cout << "Press Space-Bar to exit any time while on image.\n" << std::endl;
    std::cout << "Please enter top-left and bottom-right coordinates of a rectangle inside the ";
    std::cout << windowName << " window.\n" << std::endl;
    
    while (rendre) {
        /** display the image  and capture the user input  */
        cv::imshow(windowName, image);
        keyPressed_ASCII = cv::waitKey(33);
        
        /** check if space-bar key has pressed  and exit (ASCII 32 ) */
        if ( keyPressed_ASCII == 32 ) break;

        if (keepGoing) {
            // b - i
            /** Ask user for coordinates of a rectangle  inside the window */
            if (windowSizeChanged) {
                /** display acceptable coordinates */
                guide = "(" + std::to_string(currentImageSize.width) + "," + std::to_string(currentImageSize.height) + ").";
                std::cout << "\nThe top-left point must be between (0,0) and " << guide << std::endl;
                std::cout << "The bottom-righ point must be between top-left point and " << guide << std::endl;
                std::cout << "\nEnter two comma separated integer for each coordinate:\n" << std::endl;
            }
            innerRect = getInnerRect(currentImageSize);
            
            // b - ii
            /** Ask user for color */
            displayColourMenu();
            colour = getColour();
            
            // b - iii
            /** Color the specified rectangle in the image */
            //paint(innerRect, colour, image); //used cv::rectangle method instead
            cv::rectangle(image, innerRect, colour, cv::FILLED);
            
            // b - iv
            /** Display the specifications of the inner rectangle */
            std::cout << "\nArea: " << innerRect.area() << std::endl;
            std::cout << "Width: " << innerRect.width << std::endl;
            std::cout << "Height: " << innerRect.height << std::endl;
            
            // b - v
            /** Request a Point and see if it is inside the inner rectangle */
            aPoint = getAPoint("\nEnter a point (x,y): ");
            std::string in_OR_out = innerRect.contains(aPoint) ? "inside" : "outside";
            std::string outPut = "Point is " + in_OR_out + " the rectangle.";
            std::cout << outPut << std::endl;
            
            // b - vi
            /** Color the specified rectangle in the image */
            keepGoing = shouldContinue();
        }
                
        /** check if `n` or `N` is pressed (ASCII 110, 78) */
        if ( keyPressed_ASCII == 78 || keyPressed_ASCII == 110 || !keepGoing ) {
            bool couldSaveImage = cv::imwrite(windowName + ".png", image);
            std::cout << ( couldSaveImage ? "All Saved." : "Could not save the image.") << std::endl;
            std::cout << "Exiting ..." << std::endl;
            cv::waitKey(1000);
            rendre = false;
        }
        
        /** recalculte the actual window size in case user has changed it */
        currentImageSize = image.size();
        if (previousImageSize != currentImageSize) {
            windowSizeChanged = true;
            previousImageSize = currentImageSize;
        }
        else windowSizeChanged = false;
        
    }
    
    return 0;
    
}



void displayColourMenu() {
    std::cout << "\nEnter a key representing your colour of choice regarding to this list:" << std::endl;
    std::cout <<                \
        "                   \n\
         k: black           \n\
         w: white           \n\
         r: red             \n\
         g: green           \n\
         b: blue            \n\
         c: cyan            \n\
         y: yellow          \n\
         m: magenta         \n\
         x: a random color  \n\
                            \n" \
    << "Your choice: ";
}

bool shouldContinue() {
    char answer = '\0';
    bool keepGoing = false;
    while (answer == '\0') {
        std::cout << "\nDo you want to add more rectangles Y/N ? ";
        std::cin >> answer;
        /** flush the buffer */
        std::cin.ignore(INT_MAX, '\n');
        if (answer == 'y' || answer == 'Y') keepGoing = true;
        else if (answer == 'n' || answer == 'N') /** do nothing */;
        else {
            std::cout << "\nPlease enter only Y (as YES) or N (as NO). Try again.\n" << std::endl;
            answer = '\0';
        }
    }
    return keepGoing;
}

// instead of this function, cv::rectangle function of the openCV has been used
void paint(cv::Rect rectangle, cv::Scalar colour, cv::Mat& image) {
    cv::Point top_left = cv::Point(rectangle.x, rectangle.y);
    cv::Point bottom_right = cv::Point(rectangle.x + rectangle.width, rectangle.y + rectangle.height);
    for (unsigned i = top_left.y; i < bottom_right.y; i++) {
        for (unsigned j = top_left.x; j < bottom_right.x; j++) {
            image.at<cv::Vec3b>(i,j)[0] = colour[0];
            image.at<cv::Vec3b>(i,j)[1] = colour[1];
            image.at<cv::Vec3b>(i,j)[2] = colour[2];
        }
    }
    
}

cv::Point getAPoint(std::string message) {
    bool answerAccepted = false;
    cv::Point aPoint;
    while (!answerAccepted) {
        std::cout << message;
        std::cin >> aPoint.x;
        std::cin.ignore();
        std::cin >> aPoint.y;
        
        /** clean the state of stdin */
        if (std::cin.fail()) {
            std::cin.clear();
            std::cout << "\nPlease provide integer values separated by comma.\n" << std::endl;
        }
        else answerAccepted = true;
        
        /** flush the buffer */
        std::cin.ignore(INT_MAX, '\n');
    }
    return aPoint;
}

cv::Rect getInnerRect(cv::Size currentImageSize) {
    bool answerAccepted = false;
    cv::Point top_left, bottom_right;
    while (!answerAccepted) {
        top_left = getAPoint("\nTop-left point (x,y): ");
        bottom_right = getAPoint("bottom-righ point (x,y): ");
        
        /** accept if  points are inside the window */
        int maxX = currentImageSize.width;
        int maxY = currentImageSize.height;
        answerAccepted = top_left.x >= 0 && top_left.y >= 0;
        answerAccepted = answerAccepted && bottom_right.x <= maxX && bottom_right.y <= maxY;
        answerAccepted = answerAccepted && top_left.x < bottom_right.x && top_left.y < bottom_right.y;
        if (!answerAccepted)
            std::cout << "\nThe points entered could not satisfy the above conditions. Try again.\n" << std::endl;
    }
    return cv::Rect(top_left, bottom_right);
}

cv::Scalar getColour() {
    bool colourAccepted = false;
    cv::Scalar bgr;
    char colour;
    while (!colourAccepted) {
        std::cin >> colour;
        /** flush the buffer */
        std::cin.ignore(INT_MAX, '\n');
        
        cv::RNG randomGenerator;
        
        switch (colour) {
            case 'k': bgr = cv::Scalar(0,0,0);       break;
            case 'w': bgr = cv::Scalar(255,255,255); break;
            case 'r': bgr = cv::Scalar(0,0,255);     break;
            case 'g': bgr = cv::Scalar(0,255,0);     break;
            case 'b': bgr = cv::Scalar(255,0,0);     break;
            case 'c': bgr = cv::Scalar(255,255,0);   break;
            case 'y': bgr = cv::Scalar(0,255,255);   break;
            case 'm': bgr = cv::Scalar(255,0,255);   break;
            case 'x': bgr = cv::Scalar(randomGenerator(256), \
                                      randomGenerator(256), \
                                      randomGenerator(256)  ); break;
            default: colour = '\0';
        }
        
        if (colour != '\0') colourAccepted = true;
        else std::cout << "\nWrong colour input. Try again.\n" << std::endl;
    }
    return bgr;
}
