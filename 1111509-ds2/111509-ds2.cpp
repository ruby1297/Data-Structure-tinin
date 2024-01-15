#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <queue>

using namespace cv;
using namespace std;

const int PATH_WIDTH = 10;

void findEntranceAndExit(const Mat& maze, Point& entrance, Point& mazeexit) {
    int rows = maze.rows;
    int cols = maze.cols;
    entrance = Point(PATH_WIDTH, 0);
    mazeexit = Point(-1, -1);
    for (int i = 0; i < cols; i += PATH_WIDTH) {
        // upper border
        if (maze.at<Vec3b>(0, i) == Vec3b(255, 255, 255) && entrance != Point(i, 0)) {
            mazeexit = Point(i, 0);
            return;
        }
        // lower border
        if (maze.at<Vec3b>(rows - PATH_WIDTH, i) == Vec3b(255, 255, 255) && entrance != Point(i, rows - PATH_WIDTH)) {
            mazeexit = Point(i, rows - PATH_WIDTH);
            return;
        }
    }
    for (int i = 0; i < rows; i += PATH_WIDTH) {
        // left border
        if (maze.at<Vec3b>(i, 0) == Vec3b(255, 255, 255) && entrance != Point(0, i)) {
            mazeexit = Point(0, i);
            return;
        }
        // right border
        if (maze.at<Vec3b>(i, cols - PATH_WIDTH) == Vec3b(255, 255, 255) && entrance != Point(cols - PATH_WIDTH, i)) {
            mazeexit = Point(cols - PATH_WIDTH, i);
            return;
        }
    }
}

void findPath(const Mat& maze, const Point& entrance, const Point& mazeexit, Mat& pathImage) {
    int rows = maze.rows;
    int cols = maze.cols;
    Mat visited(rows, cols, CV_8U, Scalar(0));

    vector<vector<Point>> prev(rows, vector<Point>(cols, Point(-1, -1)));

    queue<Point> q;
    q.push(entrance);
    visited.at<uchar>(entrance.y, entrance.x) = 1;
    int dx[] = { 0, 0, PATH_WIDTH, -PATH_WIDTH };
    int dy[] = { PATH_WIDTH, -PATH_WIDTH, 0, 0 };
    while (!q.empty()) {
        Point p = q.front();
        q.pop();

        for (int i = 0; i < 4; i++) {
            int newX = p.x + dx[i];
            int newY = p.y + dy[i];

            if (newX >= 0 && newX < cols && newY >= 0 && newY < rows && !visited.at<uchar>(newY, newX) &&
                maze.at<Vec3b>(newY, newX) == Vec3b(255, 255, 255)) {
                visited.at<uchar>(newY, newX) = 1;
                q.push(Point(newX, newY));
                prev[newY][newX] = p;
                if (Point(newX, newY) == mazeexit) {
                    // Exit found, reconstruct the path
                    Point current = mazeexit;
                    while (current != entrance) {
                        rectangle(pathImage, Rect(current.x, current.y, PATH_WIDTH, PATH_WIDTH), Scalar(0, 255, 0), FILLED);
                        current = prev[current.y][current.x];
                    }
                    
                    rectangle(pathImage, Rect(current.x, current.y, PATH_WIDTH, PATH_WIDTH), Scalar(0, 255, 0), FILLED);
                    return;
                }
            }
        }
    }
}


int main(int argc, char* argv[]) {
   
    String image_path = argv[1];
    Mat maze = imread(image_path, IMREAD_COLOR);

    if (maze.empty()) {
        cout << "Could not read the image: " << image_path << endl;
        return 1;
    }

    Point entrance, mazeexit;
    findEntranceAndExit(maze, entrance, mazeexit);

    Mat pathImage = maze.clone();
    findPath(maze, entrance, mazeexit, pathImage);

    //imshow("Display Window", pathImage);
    // waitKey(0);
    
	String output_path = argv[2];
    imwrite(output_path, pathImage);
    
    return 0;
}