
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <sys/ioctl.h>

using namespace cv;
using namespace std;

// map function for mapping number from one range to another
long mapValue(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main(int argc, char* argv[])
{
	// from lightest to darkest (if dark background and light text)
	char brightnessChars[] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'.                         ";

	// get the terminal width and height
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);

	// buffer for ascii image (basically just a big ol' array of characters to write to the screen.
	// (it may be better to use a 2d array or something for just one image, but I chose to do it this way
	// because it fits better with the project I'm using this for
	char screenBuffer[w.ws_row * w.ws_col + (w.ws_col - 1)]; 

	// print out dimensions of screen
	printf ("lines %d\n", w.ws_row);
    	printf ("columns %d\n", w.ws_col);

	//open the video file for reading
	VideoCapture cap(0); 

 	// if not success, exit program
 	if (cap.isOpened() == false)  
 	{
  		cout << "Cannot open the video file" << endl;
  		cin.get(); //wait for any key press
  		return -1;
 	}




 	String window_name = "Ascii rendering";

 	namedWindow(window_name, WINDOW_NORMAL); //create a window

 	while (true)
 	{
 		Mat frame;
 		bool bSuccess = cap.read(frame); // read a new frame from video 

  		//Breaking the while loop at the end of the video
  		if (bSuccess == false) 
  		{
  			 cout << "Found the end of the video" << endl;
   			break;
  		}
	
	  	resize(frame, frame, Size(w.ws_col, w.ws_row), INTER_LINEAR); //shrink to appropriate terminal size
		
	  	cvtColor(frame, frame, COLOR_BGR2GRAY); //convert to grayscale
							
		// convert the brightness values to corresponding character for each pixel, and add to the screenBuffer
		int screenBufferIndex = 0;
		for (int i = 0; i < frame.rows; i++) {
			for (int j = 0; j < frame.cols; j++) {
				int pixelValue = (int)frame.at<uchar>(i,j);
				int brightnessCharIndex = mapValue(pixelValue, 0, 255, (int)(sizeof(brightnessChars)/sizeof(brightnessChars[0])), 0);
				screenBuffer[screenBufferIndex] = brightnessChars[brightnessCharIndex];
				screenBufferIndex++;
			}
	
			// add new line characters for every row except for the last one
			if (i < (frame.rows - 1)) {
				screenBuffer[screenBufferIndex] = '\n';
				screenBufferIndex++;
			}
		}	
		
		// clear screen, then render screenBuffer
		system("clear");
		for (int i = 0; i < (int)(sizeof(screenBuffer)/sizeof(screenBuffer[0])); i++) {
			printf("%c", screenBuffer[i]);
		}
	
	
		// rescale the image and display it in another screen for comparison/analysis
	  	resize(frame, frame, Size(600, 400), INTER_LINEAR);
	  	imshow(window_name, frame);

  		// esc to quit
  		if (waitKey(10) == 27)
  		{
  	 		cout << "stopping program..." << endl;
  	 		break;
  		}
 	}

 	return 0;

}
