#pragma once
#include <string>
#include <windows.h>
#include <commdlg.h>
#include <iostream>

const unsigned int OPEN_FILE = 0;
const unsigned int SAVE_FILE = 1;

class FileOpener
{
public:
	static FileOpener& Get();
	static std::string OpenFileDialogue(unsigned int type = OPEN_FILE);

	void SetBaseDirectory(const char* path);

private:

	std::string baseDirectory;
	FileOpener();
	~FileOpener();
	FileOpener(const FileOpener&) = delete;
	FileOpener& operator = (const FileOpener&) = delete;
};
