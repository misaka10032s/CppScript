#include "opencv2\opencv.hpp"
#include "opencv2\highgui.hpp"
// https://github.com/Majoneesileuka/object-tracking-tutorials/blob/master/OpenDisplayAndShowVideo.cpp

// CV_FOURCC('P', 'I', 'M', '1') = MPEG-1 codec
// CV_FOURCC('M', 'J', 'P', 'G') = motion-jpeg codec
// CV_FOURCC('M', 'P', '4', '2') = MPEG-4.2 codec 
// CV_FOURCC('D', 'I', 'V', '3') = MPEG-4.3 codec 
// CV_FOURCC('D', 'I', 'V', 'X') = MPEG-4 codec 
// CV_FOURCC('U', '2', '6', '3') = H263 codec 
// CV_FOURCC('I', '2', '6', '3') = H263I codec 
// CV_FOURCC('F', 'L', 'V', '1') = FLV1 codec

int main() {

	// Create video capturing object
	// 0 opens default camera, otherwise filename as argument
    std::string fileName;
    std::cin >> fileName;
	cv::VideoCapture video;

    if(fileName != "0") video = cv::VideoCapture("audioData/" + fileName);
    else video = cv::VideoCapture(0);

	// Check that video is opened
	if (!video.isOpened()) return -1;

	// For saving the frame
	cv::Mat frame;

	// Get video resolution
	int frameWidth = video.get(cv::CAP_PROP_FRAME_WIDTH);
	int frameHeigth = video.get(cv::CAP_PROP_FRAME_HEIGHT);

	// Create video writer object
	cv::VideoWriter output("audioData/output_" + fileName, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, cv::Size(frameWidth, frameHeigth));

	// Loop through available frames
	while (video.read(frame)) {

		// Display the frame
		cv::imshow("Video feed", frame);

		// Write video frame to output
		output.write(frame);

		// For breaking the loop
		if (cv::waitKey(25) >= 0) break;

	} // end while (video.read(frame))

	// Release video capture and writer
	output.release();
	video.release();

	// Destroy all windows
	cv::destroyAllWindows();

    system("pause");
	return 0;

}