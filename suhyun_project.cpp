#include "pch.h"
#include <stdio.h>
#include "tipsware.h"
#include <shellapi.h>
#include <shlwapi.h>

#pragma comment (lib, "Shlwapi.lib")

int WndMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_DROPFILES) 
    {
        void* p_list_box = FindControl(1000); 
        ListBox_ResetContent(p_list_box); 

        int count = DragQueryFile((HDROP)wParam, 0xFFFFFFFF, NULL, 0);
        char temp_path[MAX_PATH];

        for (int i = 0; i < count; i++)
        {
            DragQueryFile((HDROP)wParam, i, temp_path, MAX_PATH);
            if (!PathIsDirectoryA(temp_path))
                ListBox_InsertString(p_list_box, -1, temp_path);
        }
        return 1;
    }
    return 0;
}

void MoveFileToEasyBox(void* ap_list_box, int a_index)
{
    char path[MAX_PATH], new_path[MAX_PATH], dest_path[MAX_PATH];
    int len = ListBox_GetText(ap_list_box, a_index, path, MAX_PATH);
    char* p_ext = PathFindExtensionA(path);// PathFindFileNameA
    if (p_ext) 
    {
        if (*p_ext == '.') sprintf_s(new_path, MAX_PATH, "c:\\easy_box\\%s", p_ext + 1);
        else sprintf_s(new_path, MAX_PATH, "c:\\easy_box\\etc");
        CreateDirectory(new_path, NULL);

        sprintf_s(dest_path, MAX_PATH, "%s\\%s", new_path, PathFindFileNameA(path));
        if (MoveFile(path, dest_path)) ListBox_DeleteString(ap_list_box, a_index);
    }
}

void OnCommand(INT32 a_ctrl_id, INT32 a_notify_code, void* ap_ctrl)
{
    if (a_ctrl_id == 1001)
    { 
        void* p_list_box = FindControl(1000); 
        int index = ListBox_GetCurSel(p_list_box); 
        if (index != LB_ERR) MoveFileToEasyBox(p_list_box, index);
    }
    else if (a_ctrl_id == 1002)
    { 
        void* p_list_box = FindControl(1000); 
        int count = ListBox_GetCount(p_list_box); 
        for (int i = count - 1; i >= 0; i--) MoveFileToEasyBox(p_list_box, i);
    }
}

CMD_USER_MESSAGE(OnCommand, NULL, WndMessage)

int main()
{

    ChangeWorkSize(526, 266); 
    CreateDirectory("c:\\easy_box", NULL);  

    int wnd_style = ::GetWindowLong(gh_main_wnd, GWL_EXSTYLE);
    ::SetWindowLong(gh_main_wnd, GWL_EXSTYLE, wnd_style | WS_EX_ACCEPTFILES);
    ::SetWindowPos(gh_main_wnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    CreateListBox(10, 6, 508, 220, 1000);
    CreateButton("파일 이동", 10, 230, 80, 28, 1001);
    CreateButton("전체 분배", 94, 230, 80, 28, 1002);

    ShowDisplay();
    return 0;

}