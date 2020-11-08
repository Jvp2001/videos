/*
OneLoneCoder.com - Command Line Game Engine
"Who needs a frame buffer?" - @Javidx9

The Original & Best :P (in OOP form)

License
~~~~~~~
One Lone Coder Console Game Engine  Copyright (C) 2018  Javidx9
This program comes with ABSOLUTELY NO WARRANTY.
This is free software, and you are welcome to redistribute it
under certain conditions; See license for details.

Original works located at:
	https://www.github.com/onelonecoder
	https://www.onelonecoder.com
	https://www.youtube.com/javidx9

GNU GPLv3
	https://github.com/OneLoneCoder/videos/blob/master/LICENSE

Disclaimer
~~~~~~~~~~
I don't care what you use this for. It's intended to be educational, and perhaps
to the oddly minded - a little bit of fun. Please hack this, change it and use it
in any way you see fit. BUT, you acknowledge that I am not responsible for anything
bad that happens as a result of your actions. However, if good stuff happens, I
would appreciate a shout out, or at least give the blog some publicity for me.
Cheers!

Background
~~~~~~~~~~
If you've seen any of my videos - I like to do things using the windows console. It's quick
and easy, and allows you to focus on just the code that matters - ideal when you're
experimenting. Thing is, I have to keep doing the same initialisation and display code
each time, so this class wraps that up.

Author
~~~~~~
Twitter: @javidx9 http://twitter.com/javidx9
Blog: http://www.onelonecoder.com
YouTube: http://www.youtube.com/javidx9

Videos:
~~~~~~
Original:				https://youtu.be/cWc0hgYwZyc
Added mouse support:	https://youtu.be/tdqc9hZhHxM
Beginners Guide:		https://youtu.be/u5BhrA8ED0o

Shout Outs!
~~~~~~~~~~~
Thanks to cool people who helped with testing, bug-finding and fixing!
YouTube: 	wowLinh, JavaJack59, idkwid, kingtatgi

Last Updated: 02/07/2018

Usage:
~~~~~~
This class is abstract, so you must inherit from it. Override the OnUserCreate() function
with all the stuff you need for your application (for thready reasons it's best to do
this in this function and not your class constructor). Override the OnUserUpdate(float fElapsedTime)
function with the good stuff, it gives you the elapsed time since the last call so you
can modify your stuff dynamically. Both functions should return true, unless you need
the application to close.

int main()
{
// Use olcConsoleGameEngine derived app
OneLoneCoder_Example game;

// Create a console with resolution 160x100 characters
// Each character occupies 8x8 pixels
game.ConstructConsole(160, 100, 8, 8);

// Start the engine!
game.Start();

return 0;
}

Input is also handled for you - interrogate the m_keys[] array with the virtual
keycode you want to know about. bPressed is set for the frame the key is pressed down
in, bHeld is set if the key is held down, bReleased is set for the frame the key
is released in. The same applies to mouse! m_mousePosX and Y can be used to get
the current cursor position, and m_mouse[1..5] returns the mouse buttons.

The draw routines treat characters like pixels. By default they are set to white solid
blocks - but you can draw any unicode character, using any of the colours listed below.

There may be bugs!

See my other videos for examples!
http://www.youtube.com/javidx9

Lots of programs to try:
http://www.github.com/OneLoneCoder/videos

Chat on the Discord server:
https://discord.gg/WhwHUMV

Be bored by Twitch:
http://www.twitch.tv/javidx9



bool IsWin8OrLater()
{
DWORD version = GetVersion();
DWORD major = (DWORD) (LOBYTE(LOWORD(version)));
DWORD minor = (DWORD) (HIBYTE(LOWORD(version)));

return (major > 6) || ((major == 6) && (minor >= 2) && (minor < 4));
}

*/

#pragma once
#pragma comment(lib, "winmm.lib")

#ifndef UNICODE
#error Please enable UNICODE for your compiler! VS: Project Properties -> General -> \
Character Set -> Use Unicode. Thanks! - Javidx9
#endif

#include <iostream>
#include <chrono>
#include <vector>
#include <list>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <sstream>
#include <streambuf>
using namespace std;

#include <windows.h>


enum COLOUR
{
	FG_BLACK = 0x0000,
	FG_DARK_BLUE = 0x0001,
	FG_DARK_GREEN = 0x0002,
	FG_DARK_CYAN = 0x0003,
	FG_DARK_RED = 0x0004,
	FG_DARK_MAGENTA = 0x0005,
	FG_DARK_YELLOW = 0x0006,
	FG_GREY = 0x0007, // Thanks MS :-/
	FG_DARK_GREY = 0x0008,
	FG_BLUE = 0x0009,
	FG_GREEN = 0x000A,
	FG_CYAN = 0x000B,
	FG_RED = 0x000C,
	FG_MAGENTA = 0x000D,
	FG_YELLOW = 0x000E,
	FG_WHITE = 0x000F,
	BG_BLACK = 0x0000,
	BG_DARK_BLUE = 0x0010,
	BG_DARK_GREEN = 0x0020,
	BG_DARK_CYAN = 0x0030,
	BG_DARK_RED = 0x0040,
	BG_DARK_MAGENTA = 0x0050,
	BG_DARK_YELLOW = 0x0060,
	BG_GREY = 0x0070,
	BG_DARK_GREY = 0x0080,
	BG_BLUE = 0x0090,
	BG_GREEN = 0x00A0,
	BG_CYAN = 0x00B0,
	BG_RED = 0x00C0,
	BG_MAGENTA = 0x00D0,
	BG_YELLOW = 0x00E0,
	BG_WHITE = 0x00F0,
};

enum PIXEL_TYPE
{
	PIXEL_SOLID = 0x2588,
	PIXEL_THREEQUARTERS = 0x2593,
	PIXEL_HALF = 0x2592,
	PIXEL_QUARTER = 0x2591,
};
//Modified version of this: https://www.codeproject.com/Tips/73227/Keys-Enumeration-Win32
enum KEYS
{
	KEY_Backspace = 0x08,
	KEY_Tab = 0x09,
	KEY_Clear = 0x0C,
	KEY_Enter = 0x0D,
	KEY_Shift = 0x10,
	KEY_Control = 0x11,
	KEY_Alt = 0x12,
	KEY_Pause = 0x13,                          
	KEY_CapsLock = 0x14,
	KEY_Escape = 0x1B,
	KEY_Space = 0x20,
	KEY_PageUp = 0x21,
	KEY_PageDown = 0x22,
	KEY_End = 0x23,
	KEY_Home = 0x24,
	KEY_Left = 0x25,
	KEY_Up = 0x26,
	KEY_Right = 0x27,
	KEY_Down = 0x28,
	KEY_Select = 0x29,
	KEY_Print = 0x2A,
	KEY_Execute = 0x2B,
	KEY_PrintScreen = 0x2C,
	KEY_Insert = 0x2D,
	KEY_Delete = 0x2E,
	KEY_Help = 0x2F,
	KEY_Zero = 0x30,
	KEY_One = 0x31,
	KEY_Two = 0x32,
	KEY_Three = 0x33,
	KEY_Four = 0x34,
	KEY_Five = 0x35,
	KEY_Six = 0x36,
	KEY_Seven = 0x37,
	KEY_Eight = 0x38,
	KEY_Nine = 0x39,
	KEY_A = 0x41,
	KEY_B = 0x42,
	KEY_C = 0x43,
	KEY_D = 0x44,
	KEY_E = 0x45,
	KEY_F = 0x46,
	KEY_G = 0x47,
	KEY_H = 0x48,
	KEY_I = 0x49,
	KEY_J = 0x4A,
	KEY_K = 0x4B,
	KEY_L = 0x4C,
	KEY_M = 0x4D,
	KEY_N = 0x4E,
	KEY_O = 0x4F,
	KEY_P = 0x50,
	KEY_Q = 0x51,
	KEY_R = 0x52,
	KEY_S = 0x53,
	KEY_T = 0x54,
	KEY_U = 0x55,
	KEY_V = 0x56,
	KEY_W = 0x57,
	KEY_X = 0x58,
	KEY_Y = 0x59,
	KEY_Z = 0x5A,
	KEY_LeftWindowsKey = 0x5B,
	KEY_RightWindowsKey = 0x5C,
	KEY_ApplicationsKey = 0x5D,
	KEY_Sleep = 0x5F,
	KEY_NumPad0 = 0x60,
	KEY_NumPad1 = 0x61,
	KEY_NumPad2 = 0x62,
	KEY_NumPad3 = 0x63,
	KEY_NumPad4 = 0x64,
	KEY_NumPad5 = 0x65,
	KEY_NumPad6 = 0x66,
	KEY_NumPad7 = 0x67,
	KEY_NumPad8 = 0x68,
	KEY_NumPad9 = 0x69,
	KEY_Multiply = 0x6A,
	KEY_Add = 0x6B,
	KEY_Seperator = 0x6C,
	KEY_Subtract = 0x6D,
	KEY_Decimal = 0x6E,
	KEY_Divide = 0x6F,
	KEY_F1 = 0x70,
	KEY_F2 = 0x71,
	KEY_F3 = 0x72,
	KEY_F4 = 0x73,
	KEY_F5 = 0x74,
	KEY_F6 = 0x75,
	KEY_F7 = 0x76,
	KEY_F8 = 0x77,
	KEY_F9 = 0x78,
	KEY_F10 = 0x79,
	KEY_F11 = 0x7A,
	KEY_F12 = 0x7B,
	KEY_F13 = 0x7C,
	KEY_F14 = 0x7D,
	KEY_F15 = 0x7E,
	KEY_F16 = 0x7F,
	KEY_F17 = 0x80,
	KEY_F18 = 0x81,
	KEY_F19 = 0x82,
	KEY_F20 = 0x83,
	KEY_F21 = 0x84,
	KEY_F22 = 0x85,
	KEY_F23 = 0x86,
	KEY_F24 = 0x87,
	KEY_Numlock = 0x90,
	KEY_ScrollLock = 0x91,
	KEY_LeftShift = 0xA0,
	KEY_RightShift = 0xA1,
	KEY_LeftControl = 0xA2,
	KEY_RightContol = 0xA3,
	KEY_LeftMenu = 0xA4,
	KEY_RightMenu = 0xA5,
	KEY_BrowserBack = 0xA6,
	KEY_BrowserForward = 0xA7,
	KEY_BrowserRefresh = 0xA8,
	KEY_BrowserStop = 0xA9,
	KEY_BrowserSearch = 0xAA,
	KEY_BrowserFavorites = 0xAB,
	KEY_BrowserHome = 0xAC,
	KEY_VolumeMute = 0xAD,
	KEY_VolumeDown = 0xAE,
	KEY_VolumeUp = 0xAF,
	KEY_NextTrack = 0xB0,
	KEY_PreviousTrack = 0xB1,
	KEY_StopMedia = 0xB2,
	KEY_PlayPause = 0xB3,
	KEY_LaunchMail = 0xB4,
	KEY_SelectMedia = 0xB5,
	KEY_LaunchApp1 = 0xB6,
	KEY_LaunchApp2 = 0xB7,
	KEY_OEM1 = 0xBA,
	KEY_OEMPlus = 0xB8,
	KEY_OEMComma = 0xBC,
	KEY_OEMMinus = 0xBD,
	KEY_OEMPeriod = 0xBE,
	KEY_OEM2 = 0xBF,
	KEY_OEM3 = 0xC0,
	KEY_OEM4 = 0xDB,
	KEY_OEM5 = 0xDC,
	KEY_OEM6 = 0xDD,
	KEY_OEM7 = 0xDE,
	KEY_OEM8 = 0xDF,
	KEY_OEM102 = 0xE2,
	KEY_Process = 0xE5,
	KEY_Packet = 0xE7,
	KEY_Attn = 0xF6,
	KEY_CrSel = 0xF7,
	KEY_ExSel = 0xF8,
	KEY_EraseEOF = 0xF9,
	KEY_Play = 0xFA,
	KEY_Zoom = 0xFB,
	KEY_PA1 = 0xFD,
	KEY_OEMClear = 0xFE
};
class olcSprite
{
public:
	olcSprite()
	{

	}

	olcSprite(int w, int h)
	{
		Create(w, h);
	}

	olcSprite(wstring sFile)
	{
		if (!Load(sFile))
			Create(8, 8);
	}

	int nWidth = 0;
	int nHeight = 0;

private:
	wchar_t *m_Glyphs = nullptr;
	short *m_Colours = nullptr;

	void Create(int w, int h)
	{
		nWidth = w;
		nHeight = h;
		m_Glyphs = new wchar_t[w*h];
		m_Colours = new short[w*h];
		for (int i = 0; i < w*h; i++)
		{
			m_Glyphs[i] = L' ';
			m_Colours[i] = FG_BLACK;
		}
	}

public:
	void SetGlyph(int x, int y, wchar_t c)
	{
		if (x <0 || x >= nWidth || y < 0 || y >= nHeight)
			return;
		else
			m_Glyphs[y * nWidth + x] = c;
	}

	void SetColour(int x, int y, short c)
	{
		if (x <0 || x >= nWidth || y < 0 || y >= nHeight)
			return;
		else
			m_Colours[y * nWidth + x] = c;
	}

	wchar_t GetGlyph(int x, int y)
	{
		if (x <0 || x >= nWidth || y < 0 || y >= nHeight)
			return L' ';
		else
			return m_Glyphs[y * nWidth + x];
	}

	short GetColour(int x, int y)
	{
		if (x <0 || x >= nWidth || y < 0 || y >= nHeight)
			return FG_BLACK;
		else
			return m_Colours[y * nWidth + x];
	}

	wchar_t SampleGlyph(float x, float y)
	{
		int sx = (int)(x * (float)nWidth);
		int sy = (int)(y * (float)nHeight - 1.0f);
		if (sx <0 || sx >= nWidth || sy < 0 || sy >= nHeight)
			return L' ';
		else
			return m_Glyphs[sy * nWidth + sx];
	}

	short SampleColour(float x, float y)
	{
		int sx = (int)(x * (float)nWidth);
		int sy = (int)(y * (float)nHeight - 1.0f);
		if (sx <0 || sx >= nWidth || sy < 0 || sy >= nHeight)
			return FG_BLACK;
		else
			return m_Colours[sy * nWidth + sx];
	}

	bool Save(wstring sFile)
	{
		FILE *f = nullptr;
		_wfopen_s(&f, sFile.c_str(), L"wb");
		if (f == nullptr)
			return false;

		fwrite(&nWidth, sizeof(int), 1, f);
		fwrite(&nHeight, sizeof(int), 1, f);
		fwrite(m_Colours, sizeof(short), nWidth * nHeight, f);
		fwrite(m_Glyphs, sizeof(wchar_t), nWidth * nHeight, f);

		fclose(f);

		return true;
	}

	bool Load(wstring sFile)
	{
		delete[] m_Glyphs;
		delete[] m_Colours;
		nWidth = 0;
		nHeight = 0;

		FILE *f = nullptr;
		_wfopen_s(&f, sFile.c_str(), L"rb");
		if (f == nullptr)
			return false;

		fread(&nWidth, sizeof(int), 1, f);
		fread(&nHeight, sizeof(int), 1, f);

		Create(nWidth, nHeight);

		fread(m_Colours, sizeof(short), nWidth * nHeight, f);
		fread(m_Glyphs, sizeof(wchar_t), nWidth * nHeight, f);

		fclose(f);
		return true;
	}

	bool LoadFromResource(unsigned int id)
	{
		HRSRC res = FindResource(NULL, MAKEINTRESOURCE(id), RT_RCDATA);
		if (!res) return false;

		HGLOBAL res_handle = LoadResource(NULL, res);
		if (!res_handle) return false;

		char* res_data = (char*)LockResource(res_handle);
		DWORD res_size = SizeofResource(NULL, res);


		


		istringstream iss(res_data);
		istream *is = &iss;
		
		delete[] m_Glyphs;
		delete[] m_Colours;
		nWidth = 0;
		nHeight = 0;

		is->read((char*)&nWidth, sizeof(int) * 1);
		is->read((char*)&nHeight, sizeof(int) * 1);
		
		Create(nWidth, nHeight);
		
		iss.read((char*)m_Colours, sizeof(short) * nWidth * nHeight);		
		iss.read((char*)m_Glyphs, sizeof(wchar_t) * nWidth * nHeight);


		return true;
	}

};


class olcConsoleGameEngineOOP
{
public:
	olcConsoleGameEngineOOP();
	~olcConsoleGameEngineOOP();

public:
	int ConstructConsole(int width, int height, int fontw, int fonth);
	void Start();
	
public:
	virtual void Draw(int x, int y, wchar_t c = 0x2588, short col = 0x000F);
	void Fill(int x1, int y1, int x2, int y2, wchar_t c = 0x2588, short col = 0x000F);
	void DrawString(int x, int y, wstring c, short col = 0x000F);
	void DrawStringAlpha(int x, int y, wstring c, short col = 0x000F);
	void Clip(int &x, int &y);
	void DrawLine(int x1, int y1, int x2, int y2, wchar_t c = 0x2588, short col = 0x000F);
	void DrawCircle(int xc, int yc, int r, wchar_t c = 0x2588, short col = 0x000F);
	void FillCircle(int xc, int yc, int r, wchar_t c = 0x2588, short col = 0x000F);
	void DrawSprite(int x, int y, olcSprite *sprite);
	void DrawPartialSprite(int x, int y, olcSprite *sprite, int ox, int oy, int w, int h);
	void DrawWireFrameModel(const vector<pair<float, float>> &vecModelCoordinates, float x, float y, float r = 0.0f, float s = 1.0f, short col = FG_WHITE, wchar_t c = PIXEL_SOLID);
	int ScreenWidth();
	int ScreenHeight();

private:
	void GameThread();

protected:
	// User MUST OVERRIDE THESE!!
	virtual bool OnUserCreate() = 0;
	virtual bool OnUserUpdate(float fElapsedTime) = 0;

	// Optional for clean up 
	virtual bool OnUserDestroy();


	int Error(const wchar_t *msg);
	static BOOL CloseHandler(DWORD evt);

protected:


	struct sKeyState
	{
		bool bPressed;
		bool bReleased;
		bool bHeld;
	} m_keys[256], m_mouse[5];

	int m_mousePosX;
	int m_mousePosY;

public:
	sKeyState GetKey(int nKeyID) { return m_keys[nKeyID]; }
	int GetMouseX() { return m_mousePosX; }
	int GetMouseY() { return m_mousePosY; }
	sKeyState GetMouse(int nMouseButtonID) { return m_mouse[nMouseButtonID]; }
	bool IsFocused() { return m_bConsoleInFocus; }


protected:
	class olcAudioSample
	{
	public:
		olcAudioSample()
		{

		}

		olcAudioSample(std::wstring sWavFile)
		{
			// Load Wav file and convert to float format
			FILE *f = nullptr;
			_wfopen_s(&f, sWavFile.c_str(), L"rb");
			if (f == nullptr)
				return;

			char dump[4];
			std::fread(&dump, sizeof(char), 4, f); // Read "RIFF"
			if (strncmp(dump, "RIFF", 4) != 0) return;
			std::fread(&dump, sizeof(char), 4, f); // Not Interested
			std::fread(&dump, sizeof(char), 4, f); // Read "WAVE"
			if (strncmp(dump, "WAVE", 4) != 0) return;

			// Read Wave description chunk
			std::fread(&dump, sizeof(char), 4, f); // Read "fmt "
			std::fread(&dump, sizeof(char), 4, f); // Not Interested
			std::fread(&wavHeader, sizeof(WAVEFORMATEX) - 2, 1, f); // Read Wave Format Structure chunk
																	// Note the -2, because the structure has 2 bytes to indicate its own size
																	// which are not in the wav file

																	// Just check if wave format is compatible with olcCGE
			if (wavHeader.wBitsPerSample != 16 || wavHeader.nSamplesPerSec != 44100)
			{
				std::fclose(f);
				return;
			}

			// Search for audio data chunk
			long nChunksize = 0;
			std::fread(&dump, sizeof(char), 4, f); // Read chunk header
			std::fread(&nChunksize, sizeof(long), 1, f); // Read chunk size
			while (strncmp(dump, "data", 4) != 0)
			{
				// Not audio data, so just skip it
				std::fseek(f, nChunksize, SEEK_CUR);
				std::fread(&dump, sizeof(char), 4, f);
				std::fread(&nChunksize, sizeof(long), 1, f);
			}

			// Finally got to data, so read it all in and convert to float samples
			nSamples = nChunksize / (wavHeader.nChannels * (wavHeader.wBitsPerSample >> 3));
			nChannels = wavHeader.nChannels;

			// Create floating point buffer to hold audio sample
			fSample = new float[nSamples * nChannels];
			float *pSample = fSample;

			// Read in audio data and normalise
			for (long i = 0; i < nSamples; i++)
			{
				for (int c = 0; c < nChannels; c++)
				{
					short s = 0;
					std::fread(&s, sizeof(short), 1, f);
					*pSample = (float)s / (float)(MAXSHORT);
					pSample++;
				}
			}

			// All done, flag sound as valid
			std::fclose(f);
			bSampleValid = true;
		}

		WAVEFORMATEX wavHeader;
		float *fSample = nullptr;
		long nSamples = 0;
		int nChannels = 0;
		bool bSampleValid = false;
	};

	// This vector holds all loaded sound samples in memory
	std::vector<olcAudioSample> vecAudioSamples;

	// This structure represents a sound that is currently playing. It only
	// holds the sound ID and where this instance of it is up to for its
	// current playback
	struct sCurrentlyPlayingSample
	{
		int nAudioSampleID = 0;
		long nSamplePosition = 0;
		bool bFinished = false;
		bool bLoop = false;
	};
	std::list<sCurrentlyPlayingSample> listActiveSamples;

	// Load a 16-bit WAVE file @ 44100Hz ONLY into memory. A sample ID
	// number is returned if successful, otherwise -1
	unsigned int LoadAudioSample(std::wstring sWavFile);

	// Add sample 'id' to the mixers sounds to play list
	void PlaySample(int id, bool bLoop = false);

	void StopSample(int id);

	// The audio system uses by default a specific wave format
	bool CreateAudio(unsigned int nSampleRate = 44100, unsigned int nChannels = 1,
		unsigned int nBlocks = 8, unsigned int nBlockSamples = 512);

	// Stop and clean up audio system
	bool DestroyAudio();

	// Handler for soundcard request for more data
	void waveOutProc(HWAVEOUT hWaveOut, UINT uMsg, DWORD dwParam1, DWORD dwParam2);

	// Static wrapper for sound card handler
	static void CALLBACK waveOutProcWrap(HWAVEOUT hWaveOut, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);

	// Audio thread. This loop responds to requests from the soundcard to fill 'blocks'
	// with audio data. If no requests are available it goes dormant until the sound
	// card is ready for more data. The block is fille by the "user" in some manner
	// and then issued to the soundcard.
	void AudioThread();

	// Overridden by user if they want to generate sound in real-time
	virtual float onUserSoundSample(int nChannel, float fGlobalTime, float fTimeStep);

	// Overriden by user if they want to manipulate the sound before it is played
	virtual float onUserSoundFilter(int nChannel, float fGlobalTime, float fSample);

	// The Sound Mixer - If the user wants to play many sounds simultaneously, and
	// perhaps the same sound overlapping itself, then you need a mixer, which
	// takes input from all sound sources for that audio frame. This mixer maintains
	// a list of sound locations for all concurrently playing audio samples. Instead
	// of duplicating audio data, we simply store the fact that a sound sample is in
	// use and an offset into its sample data. As time progresses we update this offset
	// until it is beyound the length of the sound sample it is attached to. At this
	// point we remove the playing souind from the list.
	//
	// Additionally, the users application may want to generate sound instead of just
	// playing audio clips (think a synthesizer for example) in whcih case we also
	// provide an "onUser..." event to allow the user to return a sound for that point
	// in time.
	//
	// Finally, before the sound is issued to the operating system for performing, the
	// user gets one final chance to "filter" the sound, perhaps changing the volume
	// or adding funky effects
	float GetMixerOutput(int nChannel, float fGlobalTime, float fTimeStep);

protected:
	int m_nScreenWidth;
	int m_nScreenHeight;
	CHAR_INFO *m_bufScreen;
	wstring m_sAppName;
	HANDLE m_hOriginalConsole;
	CONSOLE_SCREEN_BUFFER_INFO m_OriginalConsoleInfo;
	HANDLE m_hConsole;
	HANDLE m_hConsoleIn;
	SMALL_RECT m_rectWindow;
	short m_keyOldState[256] = { 0 };
	short m_keyNewState[256] = { 0 };
	bool m_mouseOldState[5] = { 0 };
	bool m_mouseNewState[5] = { 0 };
	bool m_bConsoleInFocus = true;
	
	bool m_bEnableSound = false;
	unsigned int m_nSampleRate;
	unsigned int m_nChannels;
	unsigned int m_nBlockCount;
	unsigned int m_nBlockSamples;
	unsigned int m_nBlockCurrent;
	short* m_pBlockMemory = nullptr;
	WAVEHDR *m_pWaveHeaders = nullptr;
	HWAVEOUT m_hwDevice = nullptr;
	std::thread m_AudioThread;
	std::atomic<bool> m_bAudioThreadActive = false;
	std::atomic<unsigned int> m_nBlockFree = 0;
	std::condition_variable m_cvBlockNotZero;
	std::mutex m_muxBlockNotZero;
	std::atomic<float> m_fGlobalTime = 0.0f;

	static atomic<bool> m_bAtomActive;
	static condition_variable m_cvGameFinished;
	static mutex m_muxGame;
};

