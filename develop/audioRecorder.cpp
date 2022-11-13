#include <iostream>
#include <string>

#include "library/AudioRecorder.h"
// https://github.com/wyrover/AudioRecorder

void done()
{
	std::cout << "Done" << std::endl;
	int tmp;
	std::cin >> tmp;
}

int main()
{
	AudioRecorder recorder;
	recorder.Init();
	recorder.StartRecording();
	done();
	return 0;
}