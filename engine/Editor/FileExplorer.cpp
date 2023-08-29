#include "FileExplorer.h"

FileOpener::FileOpener()
{
    char currentDir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentDir);
    baseDirectory = currentDir;
}

FileOpener::~FileOpener()
{

}

FileOpener& FileOpener::Get()
{
    static FileOpener f_instance;
    return f_instance;
}

std::string FileOpener::OpenFileDialogue(unsigned int type)
{
    Get();
    OPENFILENAME ofn;
    char szFile[256] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (type == OPEN_FILE) {
        GetOpenFileName(&ofn);
    }
    else {
        GetSaveFileName(&ofn);
    }
 
    SetCurrentDirectory(Get().baseDirectory.c_str());
    return std::string(szFile);
}

void FileOpener::SetBaseDirectory(const char* path)
{
    Get().baseDirectory = path;
    SetCurrentDirectory(baseDirectory.c_str());
}
