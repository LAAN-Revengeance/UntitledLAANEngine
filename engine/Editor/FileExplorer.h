#pragma once
#include <string>
#include <windows.h>
#include <commdlg.h>
#include <iostream>
#include <vector>

class FileOpener
{
public:
	static FileOpener& Get();
	static std::string OpenFileDialogue();

	void SetBaseDirectory(const char* path);

private:

	std::string baseDirectory;
	FileOpener();
	~FileOpener();
	FileOpener(const FileOpener&) = delete;
	FileOpener& operator = (const FileOpener&) = delete;
};


