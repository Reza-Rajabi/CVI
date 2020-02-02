//
//  Lab04_2.cpp
//  Lab04
//
//
// Partially taken from: https://github.com/oreillymedia/Learning-OpenCV-3_examples/blob/master/example_09-02.cpp
// Example 9-2. Toy program for using a mouse to draw boxes on the screen

#include <opencv2/opencv.hpp>

std::string windowName = "Drawing App";
cv::Rect box;
cv::Scalar color;
bool isDrawing = false;
bool isCircle = false;
bool isEllipse = false;
bool isRectangle = false;

void mouse_callback(int event, int x, int y, int flags, void* param);
void writeAppName(cv::Mat& image);
void displayColourMenu();
cv::Scalar getColour();
bool shouldSave();

void draw_box(cv::Mat& img, cv::Rect box, cv::Scalar color_ = color, int thickness = 2) {
    cv::rectangle(img, box.tl(), box.br(), color_, thickness, 8, 0);
}
void draw_circle(cv::Mat& img, cv::Rect box) {
    cv::Point2f centre((box.x + box.br().x)/2 , (box.y + box.br().y)/2);
    int radius = abs(box.width) < abs(box.height) ? abs(box.width/2) : abs(box.height/2);
    cv::circle(img, centre, radius, color, 2, 8, 0);
}
void draw_ellipse(cv::Mat& img, cv::Rect box) {
    cv::Point2f centre((box.x + box.br().x)/2 , (box.y + box.br().y)/2);
    cv::Size axes(abs(box.width/2), abs(box.height/2));
    cv::ellipse(img, centre, axes, 360, 0, 360, color, 2, 8, 0);
}

int main() {
    cv::Mat image(480, 640, CV_8UC3), temp;
    image.copyTo(temp);
    box = cv::Rect(-1,-1,0,0);
    image = cv::Scalar::all(0);
    int keyPressed_ASCII;
    cv::namedWindow(windowName);
    cv::moveWindow(windowName, 10, 30);
    cv::setMouseCallback(windowName, mouse_callback, (void*)&image);
    
    writeAppName(image);
    displayColourMenu();
    color = getColour();
    
    // The main program loop. Here we copy the working image
    // to the temp image, and if the user is drawing, then
    // put the currently contemplated box onto that temp image.
    // Display the temp image, and wait 15ms for a keystroke,
    // then repeat.
    //
    while(true) {
        keyPressed_ASCII = cv::waitKey(15);
        
        image.copyTo(temp);
        if (isCircle && isDrawing) {
            draw_circle(temp, box);
            // temporary showing area
            draw_box(temp, box, cv::Scalar(255,255,255), 1);
        }
        else if (isEllipse && isDrawing) {
            draw_ellipse(temp, box);
            // temporary showing area
            draw_box(temp, box, cv::Scalar(255,255,255), 1);
        }
        else if (isRectangle && isDrawing) draw_box(temp, box);
        cv::imshow( windowName, temp );
        
        if( keyPressed_ASCII == 27 ) { // Esc key
            break;
        }
    }

    if (shouldSave()) cv::imwrite("drawing.png", image);
    
    return 0;
}


// This is our mouse callback. If the user
// presses the left button, we start a box.
// When the user releases that button, then we
// add the box to the current image. When the
// mouse is dragged (with the button down) we
// resize the box.
//
void mouse_callback(int event, int x, int y, int flags, void* param) {
    cv::Mat& image = *(cv::Mat*) param;
    if (flags & cv::EVENT_FLAG_SHIFTKEY) isCircle = true;
    else if (flags & cv::EVENT_FLAG_CTRLKEY) isEllipse = true;
    else isRectangle = true;
        
    switch (event) {
        case cv::EVENT_MOUSEMOVE: {
            if( isDrawing ) {
                box.width  = x-box.x;
                box.height = y-box.y;
            }
        }
            break;
        case cv::EVENT_LBUTTONDOWN: {
            isDrawing = true;
            box = cv::Rect( x, y, 0, 0 );
        }
            break;
        case cv::EVENT_LBUTTONUP: {
            if( box.width < 0  ) {
                box.x += box.width;
                box.width *= -1;
            }
            if( box.height < 0 ) {
                box.y += box.height;
                box.height *= -1;
            }
            
            if (isCircle) {
                draw_circle(image, box);
                isCircle = false;
            }
            else if (isEllipse) {
                draw_ellipse(image, box);
                isEllipse = false;
            }
            else if (isRectangle) {
                draw_box(image, box);
                isRectangle = false;
            }
            isDrawing = false;
            
        }
            break;
    }
    
}

void writeAppName(cv::Mat& image) {
    std::string name = "My Drawing Application";
    int baseLine = 0;
    cv::Size textSize = cv::getTextSize(name, cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 1, 1, &baseLine);
    cv::Point textCenter((image.cols - textSize.width)/2, 40);
    cv::line(image, textCenter + cv::Point(0, 1), textCenter + cv::Point(textSize.width, 1), cv::Scalar(0,0,255), 1, 8);
    cv::putText(image, name, textCenter, cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 1, cv::Scalar(0,255,0));

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

bool shouldSave() {
    char answer = '\0';
    bool save = false;
    while (answer == '\0') {
        std::cout << "\nDo you want to save the image Y/N ? ";
        std::cin >> answer;
        /** flush the buffer */
        std::cin.ignore(INT_MAX, '\n');
        if (answer == 'y' || answer == 'Y') save = true;
        else if (answer == 'n' || answer == 'N') /** do nothing */;
        else {
            std::cout << "\nPlease enter only Y (as YES) or N (as NO). Try again.\n" << std::endl;
            answer = '\0';
        }
    }
    return save;
}
