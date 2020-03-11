//
//  Lab08.cpp
//  Lab08
//
//

#include <opencv2/opencv.hpp>

const char predicts[] = "alg_bin.csv";
const char lables[] = "gt.csv";
const char continuousPredicts[] = "alg_dbl.csv";
const char generalMessage[] = "Select a number (1 to 4) or 0 to exit: ";

int getUserEntry (std::string message);
void openStream(const char* file, std::ifstream& ifs);
int countRows(std::ifstream& ifs);
template <typename T> void loadData(std::ifstream& ifs, T* data, int len);
int calcPR(bool* pred, bool* gt, int len, double* P, double* R);
double calcFb(double P, double R, double beta);
void thresh_v(double* A, bool* B, int len, double thresh);



int main() {
    std::ifstream ifs;
    std::ofstream ofs;
    openStream(predicts, ifs);
    int len = countRows(ifs);
    bool pred[len], gt[len];
    double A[len];
    bool B[len];
    double P, R, F;
    int allRight;
    double thresh;
    
    openStream(predicts, ifs);
    loadData<bool>(ifs, pred, len);
    openStream(lables, ifs);
    loadData<bool>(ifs, gt, len);
    openStream(continuousPredicts, ifs);
    loadData<double>(ifs, A, len);

    int choice = getUserEntry(generalMessage);
    while (choice != 0) {
        if (choice == 1) {
            allRight = calcPR(pred, gt, len, &P, &R);
            if (allRight == 0) { std::cout << "Error calling calcPR \n"; exit(1); }
            F = calcFb(P, R, 1.0);
            
            std::cout << "Precision: " << P << std::endl;
            std::cout << "Recall: " << R << std::endl;
            std::cout << "F1: " << F << std::endl << std::endl;
        }
        else if (choice == 2) {
            thresh = 0.1;
            ofs.open("PR.csv");
            if (ofs.is_open()) {
                ofs << "THRESH,P,R,F1" << std::endl;
                ofs << std::setprecision(2);
            }
            while(ofs.is_open() && thresh < 1.0) {
                thresh_v(A, B, len, thresh);
                allRight = calcPR(B, gt, len, &P, &R);
                if (allRight == 0) { std::cout << "Error calling calcPR \n"; exit(1); }
                F = calcFb(P, R, 1.0);
                
                ofs << thresh << "," << P << "," << R << "," << F;
                
                thresh += 0.1;
                if (thresh == 1.0) ofs.close();
                else ofs << std::endl;
            }
           std::cout << "Done!" << std::endl << std::endl;
        }
        choice = getUserEntry(generalMessage);
    }
    return 0;
    
}

int getUserEntry (std::string message) {
    int answer = -1;
    while ( answer == -1 ) {
        std::cout << message;
        std::cin >> answer;
        if (!std::cin.fail() && answer >= 1 && answer <= 4) /** do nothing */;
        else if ( answer == 0 ) /** do nothing */;
        else {
            std::cin.clear();
            std::cout << "Please try again.\n" << std::endl;
            answer = -1;
        }
        /** flush the buffer */
        std::cin.ignore(INT_MAX, '\n');
    }
    return answer;
}

void openStream(const char* file, std::ifstream& in) {
    in.open(file, std::ifstream::in);
    if (in.fail()) {
        std::cout << "Couldn't open data file." << std::endl;
        exit(1);
    }
}

int countRows(std::ifstream& in) {
    int counter = 0;
    std::string dispose;
    while(getline(in, dispose)) ++counter;
    in.close();
    return counter;
}

template <typename T>
void loadData(std::ifstream& in, T* data, int len) {
    for (int i = 0; i < len; i++) {
        in >> data[i];
        in.ignore(); /// discard the \n character
    }
    in.clear();
    in.close();
}

int calcPR(bool* pred, bool* gt, int len, double* P, double* R) {
    int succeed = 1;
    double TP = 0, TN = 0, FP = 0, FN = 0;
    if (pred && gt && P && R) {     /// if all pointers are valid
        for (int i = 0; i < len; i++) {
            if (pred[i] && gt[i]) TP++;
            else if (pred[i] && !gt[i]) FP++;
            else if (!pred[i] && gt[i]) FN++;
            else if (!pred[i] && !gt[i]) TN++;
        }
        *R = TP/(TP + FN);
        *P = TP/(TP + FP);
    }
    else succeed = 0;
    
    return succeed;
}

double calcFb(double P, double R, double beta) {
    double b = pow(beta,2);
    return (1 + b) * P * R / (b * P + R);
}

void thresh_v(double* A, bool* B, int len, double thresh) {
    for (int i = 0; i < len; i++) {
        B[i] = A[i] > thresh;
    }
}
