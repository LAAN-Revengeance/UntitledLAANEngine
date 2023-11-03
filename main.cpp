#include "engine/Application/EditorApp.h"
#include "engine/Application/StandaloneApp.h"

int main(int argc, char** argv)
{
    //EditorApp application;
    //application.Run();

    GaemApp application("resources/scenes/classroomScene");
    application.Run();

    return 0;
}
