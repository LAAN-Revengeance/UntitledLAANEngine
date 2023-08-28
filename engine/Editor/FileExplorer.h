#pragma once
#include <string>
#include <windows.h>
#include <commdlg.h>
#include <iostream>

class FileOpener
{
public:
	FileOpener();
	~FileOpener();

	static std::string OpenFileDialogue();

private:

};


