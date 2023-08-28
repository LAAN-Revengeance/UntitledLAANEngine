#include "FileExplorer.h"

std::string FileOpener::OpenFileDialogue()
{
    OPENFILENAME ofn;
    char szFile[256] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE) {
        std::cout << szFile;
        //MessageBox(NULL, szFile, "Selected File", MB_OK);
    }
    return std::string(szFile);
}