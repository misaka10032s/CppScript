#include <iostream>
#include <windows.h>
#include <vector>

#include "AudioRecorder.h"

const int BUFFER_DURATION = 1;
const int CHANNEL_COUNT = 1;
const int SAMPLE_RATE = 44100;
const int DATA_SIZE = SAMPLE_RATE * BUFFER_DURATION * CHANNEL_COUNT;
const int BUFFER_SIZE = 10;
const int NUMBER_OF_BUFFERS = 5;

WAVEHDR* hdr1;
AudioRecorder::AudioRecorder(){
	this->running = true;
	buffers[NUMBER_OF_BUFFERS] = {};
	SoundBuffer::Init(NUMBER_OF_BUFFERS);
}


bool AudioRecorder::Init(void(*callback)(WAVEHDR data)){
	this->callback = callback;

	ZeroMemory(&format, sizeof(format));
	format.nChannels = CHANNEL_COUNT;
	format.cbSize = 0;
	format.nSamplesPerSec = SAMPLE_RATE;
	format.wFormatTag = WAVE_FORMAT_PCM;
	format.wBitsPerSample = 8;
	format.nBlockAlign = (CHANNEL_COUNT * format.wBitsPerSample) / 8;
	format.nAvgBytesPerSec = SAMPLE_RATE * format.nBlockAlign;
	
	res = waveInOpen(&micHandle, WAVE_MAPPER, &format, (DWORD_PTR)waveInCallback, 0, CALLBACK_FUNCTION );
	if (res != MMSYSERR_NOERROR)	{
		std::cerr << "waveInOpen() failed" << std::endl;
		return false;
	}

	for (int i = 0; i < NUMBER_OF_BUFFERS; i++)	{
		WAVEHDR header;
		ZeroMemory(&header, sizeof(header));
		

		header.lpData = (LPSTR)new short int[DATA_SIZE];
		header.dwBufferLength = DATA_SIZE;
		header.dwBytesRecorded = 0;
		header.dwUser = 0;
		header.dwFlags = 0;
		header.dwLoops = 0;
		header.dwUser = i;	

		buffers[i] = header;
		SoundBuffer::SetAwaitingCompletion(i); 
		addBuffer(&buffers[i]);
	}
	
	return true;
}

bool AudioRecorder::addBuffer(WAVEHDR* buffer){
	res = waveInPrepareHeader(micHandle, buffer, sizeof(WAVEHDR));
	if (res != MMSYSERR_NOERROR)	{
		std::cerr << "waveInPrepareHeader() failed" << std::endl;
		return false;
	}

	res = waveInAddBuffer(micHandle, buffer, sizeof(WAVEHDR));
	if (res != MMSYSERR_NOERROR)	{
		std::cerr << "waveInAddBuffer() failed" << std::endl;
		return false;
	}
	return true;
}

void AudioRecorder::Update(){
	std::vector<int> ids = SoundBuffer::GetUnpreparedIDs();
	
	for(int i=0; i<(int)ids.size(); i++){
		callback(buffers[ids[i]]);

		buffers[ids[i]].dwUser = ids[i];
		addBuffer(&buffers[ids[i]]);

		SoundBuffer::SetAwaitingCompletion(ids[i]);
	}
}

WAVEHDR AudioRecorder::getNewWAVEHDR(){
	short int data[DATA_SIZE];
	WAVEHDR header;
	ZeroMemory(&header, sizeof(header));

	header.lpData = (LPSTR)&data;
	header.dwBufferLength = DATA_SIZE;
	header.dwBytesRecorded = 0;
	header.dwUser = 0;
	header.dwFlags = 0;
	header.dwLoops = 0;

	return header;
}

void CALLBACK waveInCallback(HWAVEIN hWaveOut, UINT uMesg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2){
	if (uMesg == WIM_DATA)
	{
		WAVEHDR* hdr = (WAVEHDR*)dwParam1;
		int id = hdr->dwUser;

		SoundBuffer::SetUnprepared(id);
	}
}

bool AudioRecorder::StartRecording(){
	res = waveInStart(this->micHandle);
	if (res != MMSYSERR_NOERROR)
	{
		std::cerr << "waveInStart() failed" << std::endl;
		return false;
	}

	return true;
}

void AudioRecorder::StopRecording(){
	waveInStop(this->micHandle);
}

AudioRecorder::~AudioRecorder(){
}