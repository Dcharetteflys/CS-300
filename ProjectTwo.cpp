//============================================================================
// Name        : ProjectTwo.cpp
// Author      : David Charette  
// Date        : 4/16/2025
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <Windows.h>
#include <vector>


using namespace std;

const int GLOBAL_SLEEP_TIME = 5000;//default time for sleep

// Struct to hold the course information
struct Course {
    string courseId;
    string courseName;
    vector<string> prerequisites;
};

class BinarySearchTree {
private:
    // structures to hold courses
    struct Node {
        Course course;
        Node* left;
        Node* right;

        // default constructor
        Node() {
            left = nullptr;
            right = nullptr;
        }

        // initialize with a course
        Node(const Course& aCourse) {
            course = aCourse;
            left = nullptr;
            right = nullptr;
        }
    };

    Node* root;
    int size;
    void inOrder(Node* node);

public:
    BinarySearchTree();
    ~BinarySearchTree(); // Destructor for memory cleanup
    void InOrder();
    void Insert(Course aCourse);
    void Remove(string courseId);
    Course Search(string courseId);
    int Size();
    void deleteTree(Node* node); // Helper function for destructor
};

 
// Default constructor
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
    size = 0;
}

// Destructor to clean up memory
BinarySearchTree::~BinarySearchTree() {
    deleteTree(root);
}

// Helper function to delete all nodes recursively
void BinarySearchTree::deleteTree(Node* node) {
    if (node == nullptr) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

 //Traverse the tree in order

void BinarySearchTree::InOrder() {
    inOrder(root);
}


 // Insert a course
 
// Insert a course
void BinarySearchTree::Insert(Course aCourse) {
    Node* currentNode = root;

    if (root == nullptr) {
        root = new Node(aCourse);
    }
    else {
        while (currentNode != nullptr) {
            if (aCourse.courseId < currentNode->course.courseId) {
                if (currentNode->left == nullptr) {
                    currentNode->left = new Node(aCourse);
                    break;
                }
                else {
                    currentNode = currentNode->left;
                }
            }
            else {
                if (currentNode->right == nullptr) {
                    currentNode->right = new Node(aCourse);
                    break;
                }
                else {
                    currentNode = currentNode->right;
                }
            }
        }
    }
    size++;
}


// Remove a Course
void BinarySearchTree::Remove(string courseId) {
    Node* parent = nullptr;
    Node* current = root;

    while (current != nullptr) {
        if (current->course.courseId == courseId) {
            if (current->left == nullptr && current->right == nullptr) {
                if (parent == nullptr) {
                    root = nullptr;
                }
                else if (parent->left == current) {
                    parent->left = nullptr;
                }
                else {
                    parent->right = nullptr;
                }
            }
            else if (current->right == nullptr) {
                if (parent == nullptr) {
                    root = current->left;
                }
                else if (parent->left == current) {
                    parent->left = current->left;
                }
                else {
                    parent->right = current->left;
                }
            }
            else if (current->left == nullptr) {
                if (parent == nullptr) {
                    root = current->right;
                }
                else if (parent->left == current) {
                    parent->left = current->right;
                }
                else {
                    parent->right = current->right;
                }
            }
            else {
                Node* successor = current->right;
                while (successor->left != nullptr) {
                    successor = successor->left;
                }
                Node successorCopy = Node(successor->course);
                Remove(successor->course.courseId);
                current->course = successorCopy.course;
            }
            cout << "\nNode found and removed" << endl;
            return;
        }
        else if (current->course.courseId < courseId) {
            parent = current;
            current = current->right;
        }
        else {
            parent = current;
            current = current->left;
        }
    }
    cout << "\nValue not found" << endl;
}


// Search for a course
Course BinarySearchTree::Search(string courseId) {
    Course aCourse;
    Node* currentNode = root;

    while (currentNode != nullptr) {
        if (currentNode->course.courseId == courseId) {
            return currentNode->course;
        }
        else if (courseId < currentNode->course.courseId) {
            currentNode = currentNode->left;
        }
        else {
            currentNode = currentNode->right;
        }
    }

    cout << "Value not found." << endl;
    return aCourse;
}

void BinarySearchTree::inOrder(Node* node) {
    if (node == nullptr) return;

    inOrder(node->left);
    cout << node->course.courseId << ", " << node->course.courseName << endl;
    inOrder(node->right);
}

int BinarySearchTree::Size() {
    return size;
}


// Split function to split the CSV line
vector<string> Split(string lineFeed) {
    char delim = ',';

    vector<string> lineTokens;
    string temp = "";
    for (char ch : lineFeed) {
        if (ch == delim) {
            lineTokens.push_back(temp);
            temp.clear();
        }
        else {
            temp += ch;
        }
    }
    if (!temp.empty()) {
        lineTokens.push_back(temp);
    }
    return lineTokens;
}


// Function to load courses from CSV file
void loadCourses(string csvPath, BinarySearchTree* courseList) {
    ifstream inFS;
    string line;
    vector<string> stringTokens;

    inFS.open(csvPath);
    if (!inFS.is_open()) {
        cout << "Could not open file. Please check inputs. " << endl;
        return;
    }

    while (getline(inFS, line)) {
        Course aCourse;
        stringTokens = Split(line);

        if (stringTokens.size() < 2) {
            cout << "\nError. Skipping line." << endl;
        }
        else {
            aCourse.courseId = stringTokens.at(0);
            aCourse.courseName = stringTokens.at(1);

            for (unsigned int i = 2; i < stringTokens.size(); i++) {
                aCourse.prerequisites.push_back(stringTokens.at(i));
            }

            courseList->Insert(aCourse);
        }
    }

    inFS.close();
}

// Display course information
void displayCourse(Course aCourse) {
    cout << aCourse.courseId << ", " << aCourse.courseName << endl;
    cout << "Prerequisites: ";

    if (aCourse.prerequisites.empty()) {
        cout << "none" << endl;
    }
    else {
        for (unsigned int i = 0; i < aCourse.prerequisites.size(); i++) {
            cout << aCourse.prerequisites.at(i);
            if (aCourse.prerequisites.size() > 1 && i < aCourse.prerequisites.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }
}

// Convert string to uppercase
void convertCase(string& toConvert) {
    for (unsigned int i = 0; i < toConvert.length(); i++) {
        if (isalpha(toConvert[i])) {
            toConvert[i] = toupper(toConvert[i]);
        }
    }
}

int main(int argc, char* argv[]) {
    string csvPath, aCourseKey;

    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    case 3:
        csvPath = argv[1];
        aCourseKey = argv[2];
        break;
    default:
        csvPath = "ABCU_Advising_Program_Input.csv";
    }

    // Define a table to hold all the courses
    BinarySearchTree* courseList = new BinarySearchTree();
    Course course;
    bool goodInput;
    int choice = 0;

    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Display All Courses" << endl;
        cout << "  3. Find Course" << endl;
        cout << "  4. Remove Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";

        aCourseKey = ""; //clear the choice
        string anyKey = " "; //clear the choice
        choice = 0;  //clear the choice

        try {
            cin >> choice;

            if ((choice > 0 && choice < 5) || (choice == 9)) { //Allows for valid choice 
                goodInput = true;
            }
            else {                      //Throws error
                goodInput = false;
                throw 1;
            }

            switch (choice) {
            case 1:

                // Complete the method call to load the courses
                loadCourses(csvPath, courseList);
                cout << courseList->Size() << " courses read" << endl;
                Sleep(GLOBAL_SLEEP_TIME);
                break;

            case 2:
                courseList->InOrder();
                cout << "\nEnter \'yes\' to continue..." << endl;
                cin >> anyKey;
                break;

            case 3:

                cout << "\nSelect a course. " << endl;
                cin >> aCourseKey;

                convertCase(aCourseKey);//converts case of user input

                course = courseList->Search(aCourseKey);

                if (!course.courseId.empty()) {
                    displayCourse(course);
                }
                else {
                    cout << "\nCourse ID " << aCourseKey << " ERROR, course not found." << endl;
                }

                Sleep(GLOBAL_SLEEP_TIME);
                break;

            case 4:

                cout << "\nENTER course you want to delete. " << endl;
                cin >> aCourseKey;

                convertCase(aCourseKey);

                courseList->Remove(aCourseKey);

                Sleep(GLOBAL_SLEEP_TIME);
                break;

            case 9:
                return 0;

            default:
                throw 2;
            }
        }
        catch (int err) {
            cout << "\nERROR, check your input." << endl;
            Sleep(GLOBAL_SLEEP_TIME);
        }

        //need to clear the cin operator of extra input, e.g., 9 9, or any errors generated by bad input, e.g., 'a'
        cin.clear();
        cin.ignore();

        //clear the consolse to redraw a fresh menu
        system("cls");
    }

    cout << "Good bye." << endl;

    Sleep(GLOBAL_SLEEP_TIME);

    return 0;
}