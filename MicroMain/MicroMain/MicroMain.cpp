#include "MicroMain.h"
#include "Windows.h"
#include "Winuser.h"
#include "mmeapi.h"
#include <stdio.h>
#include "DShow.h"
#include <tchar.h>
#include <strsafe.h>
#include <deque>
#include <heapapi.h>
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <commdlg.h>

#pragma comment(lib, "strmiids.lib")

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
HWND hWnd, My;//Основное окно
HWND StringEx;//ButtonMusicTest
HWND SreenFull;
HWND WindowsSizeSreen;
HWND ExitButt;
HWND OptionButt;
HWND GameButt;
HWND  Button;
HWND BackButt;
HWND ButtGameStart;
HWND NextLevelButt;
HWND LastLevelButt;


LPWSTR SoftMusic = (LPWSTR)L"..\\Music\\ZeroBlood.mp3";
LPWSTR SoftLevel1Music = (LPWSTR)L"..\\Music\\AM.mp3";
LPWSTR SoftLevel2Music = (LPWSTR)L"..\\Music\\Viagra.mp3";
LPWSTR SoftLevel3Music = (LPWSTR)L"..\\Music\\Yarmak.mp3";

LPWSTR UserLevelMusic = (LPWSTR)L"";

DWORD   dwThreadIdMusiBackground;
HANDLE  hThreadMusicBackground;

HWND FaliOpen;//Отрытие FaleDialog
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
HDC hdc;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

//Путь к пользовательскому файлу
LPWSTR UserPathMusic;

//Предобъявление
VOID MidiWin(LPWSTR Path);
VOID TreaderMusic(LPWSTR Path);
VOID StartGame();
VOID SetBackgroundIcons();
VOID DrawButton(LPDRAWITEMSTRUCT lpInfo);
VOID TreaderBackgroundMusic(LPWSTR Path);
bool enterFullscreen(HWND hwnd, int fullscreenWidth, int fullscreenHeight, int colourBits, int refreshRate);
bool exitFullscreen(HWND hwnd, int windowX, int windowY, int windowedWidth, int windowedHeight, int windowedPaddingX, int windowedPaddingY);

void MyButton(HWND & hwnd, int x, int y, int Width, int Height, int xe, int xy, HWND hParent);

//Сообщение
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MICROMAIN, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MICROMAIN));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//Окно свойство
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEXW);

    wcex.style          = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MICROMAIN));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MICROMAIN);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//Элементы
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   //11
   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   //Игра
   Button = CreateWindow(L"button", L"Начать игру", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
       700, 560, 120, 30, hWnd, (HMENU)4, (HINSTANCE)GetWindowLongA(hWnd, -6), NULL);
   StringEx = CreateWindowEx(NULL, L"Edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL,
       10, 60, 250, 30, hWnd, (HMENU)2, (HINSTANCE)GetWindowLongA(hWnd, -6), NULL);
   FaliOpen = CreateWindow(L"button", L"Изменить музыку", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
       300, 200, 120, 30, hWnd, (HMENU)5, (HINSTANCE)GetWindowLongA(hWnd, -6), NULL);
   NextLevelButt = CreateWindow(L"button", L"N", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
       950, 400, 30, 30, hWnd, (HMENU)10, (HINSTANCE)GetWindowLongA(hWnd, -6), NULL);
   LastLevelButt = CreateWindow(L"button", L"L", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
       550, 400, 30, 30, hWnd, (HMENU)11, (HINSTANCE)GetWindowLongA(hWnd, -6), NULL);



   //Настройки
   SreenFull = CreateWindow(L"button", L"Полноэкраный", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
       600, 480, 120, 30, hWnd, (HMENU)1, (HINSTANCE)GetWindowLongA(hWnd, -6), NULL);
   WindowsSizeSreen = CreateWindow(L"button", L"Оконый", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
       600, 520, 120, 30, hWnd, (HMENU)3, (HINSTANCE)GetWindowLongA(hWnd, -6), NULL);

   //Меню
   GameButt = CreateWindow(L"button", L"Игра", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_OWNERDRAW,
       700, 440, 120, 30, hWnd, (HMENU)8, (HINSTANCE)GetWindowLongA(hWnd, -6), NULL);
   OptionButt = CreateWindow(L"button", L"Настройки", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
       700, 530, 120, 30, hWnd, (HMENU)7, (HINSTANCE)GetWindowLongA(hWnd, -6), NULL);
   ExitButt = CreateWindow(L"button", L"Выход", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
       700, 620, 120, 30, hWnd, (HMENU)6, (HINSTANCE)GetWindowLongA(hWnd, -6), NULL);


   BackButt = CreateWindow(L"Button", L"Назад", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
       700, 620, 120, 30 ,hWnd, (HMENU)9, (HINSTANCE)GetWindowLong(hWnd, -6),NULL);
   
  // ButtGameStart = CreateWindow(L"Button", L"Назад", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
    //   700, 520, 120, 30, hWnd, (HMENU)10, (HINSTANCE)GetWindowLong(hWnd, -6), NULL);


   if (!hWnd)
   {
      return FALSE;
   }
  
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

  // SetBackgroundIcons();

   //TreaderMusic(SoftMusic);
   //TreaderBackgroundMusic(SoftMusic);

   //Экран начало
   enterFullscreen(hWnd, 1080, 1920, 5, 60);

   ShowWindow(StringEx, SW_HIDE);
   ShowWindow(ButtGameStart, SW_HIDE);
   ShowWindow(SreenFull, SW_HIDE);
   ShowWindow(BackButt, SW_HIDE);
   ShowWindow(WindowsSizeSreen, SW_HIDE);
   ShowWindow(Button, SW_HIDE);
   ShowWindow(FaliOpen, SW_HIDE);
   ShowWindow(NextLevelButt, SW_HIDE);
   ShowWindow(LastLevelButt, SW_HIDE);

   MyButton(My, 10, 10, 100, 30, 20, 20, hWnd);

   SetWindowLong(My, GWL_EXSTYLE, GetWindowLong(My, GWL_EXSTYLE) | WS_EX_LAYERED);
   SetLayeredWindowAttributes(My, RGB(255, 0, 0), 10, LWA_COLORKEY);

   return TRUE;
}

//Команды
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            if (HIWORD(wParam) == BN_CLICKED)
            {
                switch (LOWORD(wParam))
                {

                //Музыка
                case 4:
                {
                    TreaderMusic(SoftMusic);
                    //MidiWin(SoftMusic);
                }
                break;

                //Полный экран
                case 1:
                {
                    enterFullscreen(hWnd,1080,1920,5,60);
                }
                break;

                //Окрный режим
                case 3:
                {
                    exitFullscreen(hWnd, 100,100,800,600,50,50);
                }
                break;

                //FaliDialog
                case 5:
                {
                    OPENFILENAME ofn;
                    char szFile[260]; 
                    HWND hwnd; 
                    HANDLE hf;
                    hwnd = NULL;
                    ZeroMemory(&ofn, sizeof(ofn));
                    ofn.lStructSize = sizeof(ofn);
                    ofn.hwndOwner = hwnd;
                    ofn.lpstrFile = (LPWSTR)szFile;
                    ofn.nMaxFile = sizeof(szFile);
                    ofn.lpstrFilter = L"Music (*.mp3)\0*.mp3\0";
                    ofn.nFilterIndex = 2;
                    ofn.lpstrFileTitle = NULL;
                    ofn.nMaxFileTitle = 0;
                    ofn.lpstrInitialDir = NULL;
                    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
                    GetOpenFileName(&ofn);

                    SetWindowText(StringEx, ofn.lpstrFile);
                    UserPathMusic = ofn.lpstrFile;
                }
                break;

                //Exit
                case 6:
                {
                    DestroyWindow(hWnd);
                }
                break;

                //Настройки
                case 7: {
                    ShowWindow(SreenFull, SW_SHOW);
                    ShowWindow(WindowsSizeSreen, SW_SHOW);
                    ShowWindow(BackButt, SW_SHOW);

                    ShowWindow(GameButt, SW_HIDE);
                    ShowWindow(OptionButt, SW_HIDE);
                    ShowWindow(ExitButt, SW_HIDE);
                    
                }
                break;

                //Играть
                case 8:
                {
                    ShowWindow(Button, SW_SHOW);
                    ShowWindow(FaliOpen, SW_SHOW);
                    ShowWindow(StringEx, SW_SHOW);
                    ShowWindow(BackButt, SW_SHOW);
                    ShowWindow(ButtGameStart, SW_SHOW);

                    ShowWindow(GameButt, SW_HIDE);
                    ShowWindow(OptionButt, SW_HIDE);
                    ShowWindow(ExitButt, SW_HIDE);

                    ShowWindow(NextLevelButt, SW_SHOW);
                    ShowWindow(LastLevelButt, SW_SHOW);
                    
                }
                break;
                
                //Назад
                case 9:
                {
                    ShowWindow(GameButt, SW_SHOW);
                    ShowWindow(OptionButt, SW_SHOW);
                    ShowWindow(ExitButt, SW_SHOW);

                    ShowWindow(Button, SW_HIDE);
                    ShowWindow(FaliOpen, SW_HIDE);
                    ShowWindow(StringEx, SW_HIDE);
                    ShowWindow(SreenFull, SW_HIDE);
                    ShowWindow(WindowsSizeSreen, SW_HIDE);
                    ShowWindow(NextLevelButt, SW_HIDE);
                    ShowWindow(LastLevelButt, SW_HIDE);
                    ShowWindow(BackButt, SW_HIDE);
                }
                break;

                //Старт игры
                case 10:
                {
                    StartGame();
                }
                break;


                }
            }
            /*
            if (HIWORD(wParam) == BN_SETFOCUS)
            {
                exitFullscreen(hWnd, 100, 100, 800, 600, 50, 50);
            }*/


            int wmId = LOWORD(wParam);
            switch (wmId)
                {
               /* case IDM_EXIT:
                    DestroyWindow(hWnd);
                    break;*/
               // default:
                //    return DefWindowProc(hWnd, message, wParam, lParam);
                }
            }
            break;
            case WM_PAINT:
            {
               
                PAINTSTRUCT ps;
                hdc = BeginPaint(hWnd, &ps);

                SetBackgroundIcons();

                EndPaint(hWnd, &ps);
            }
            break;
     /*   case WM_DRAWITEM:
        {
            DrawButton((LPDRAWITEMSTRUCT)lParam);
            return true;
        }*/
        case WM_SETCURSOR:
        {
            if ((HWND)wParam == My)
            {
                SetFocus(My);
            }
            else if ((HWND)wParam != My)
            {
                SetFocus(hWnd);
            }
            return false;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return 0;
    }

//Создание потока
VOID TreaderMusic(LPWSTR Path) {
    DWORD   dwThreadIdArray;
    HANDLE  hThreadArray;

    hThreadArray = CreateThread(
        NULL,                   
        0,                      
        (LPTHREAD_START_ROUTINE)MidiWin,
        Path,
        0,                      
        &dwThreadIdArray);   

    if (hThreadArray == NULL)
    {
        ExitProcess(3);
    }

    CloseHandle(hThreadArray);
}

//Фоновая музыка
/*VOID TreaderBackgroundMusic(LPWSTR Path) {
    hThreadMusicBackground = CreateThread(
        NULL,
        0,
        (LPTHREAD_START_ROUTINE)MidiWin,
        Path,
        0,
        &dwThreadIdMusiBackground);

    if (hThreadMusicBackground == NULL)
    {
        ExitProcess(3);
    }

    CloseHandle(hThreadMusicBackground);
}
*/

//Воспроизведение музыки
VOID MidiWin(LPWSTR Path) {
    IGraphBuilder* pGraph = NULL;
    IMediaControl* pControl = NULL;
    IMediaEvent* pEvent = NULL;

    // Initialize the COM library.
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        printf("ERROR - Could not initialize COM library");
        return;
    }

    // Create the filter graph manager and query for interfaces.
    hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
        IID_IGraphBuilder, (void**)&pGraph);
    if (FAILED(hr))
    {
        printf("ERROR - Could not create the Filter Graph Manager.");
        return;
    }

    hr = pGraph->QueryInterface(IID_IMediaControl, (void**)&pControl);
    hr = pGraph->QueryInterface(IID_IMediaEvent, (void**)&pEvent);

    // Build the graph. IMPORTANT: Change this string to a file on your system.
    hr = pGraph->RenderFile((LPCWSTR)Path, NULL);
    if (SUCCEEDED(hr))
    {
        // Run the graph.
        hr = pControl->Run();
        if (SUCCEEDED(hr))
        {
            // Wait for completion.
            long evCode;
            pEvent->WaitForCompletion(INFINITE, &evCode);

            // Note: Do not use INFINITE in a real application, because it
            // can block indefinitely.
        }
    }
    pControl->Release();
    pEvent->Release();
    pGraph->Release();
    CoUninitialize();
}

//Полноэкраный режим
bool enterFullscreen(HWND hwnd, int fullscreenWidth, int fullscreenHeight, int colourBits, int refreshRate) {
    DEVMODE fullscreenSettings;
    bool isChangeSuccessful;
   // RECT windowBoundary;

    EnumDisplaySettings(NULL, 0, &fullscreenSettings);
    fullscreenSettings.dmPelsWidth = fullscreenWidth;
    fullscreenSettings.dmPelsHeight = fullscreenHeight;
    fullscreenSettings.dmBitsPerPel = colourBits;
    fullscreenSettings.dmDisplayFrequency = refreshRate;
    fullscreenSettings.dmFields = DM_PELSWIDTH |
        DM_PELSHEIGHT |
        DM_BITSPERPEL |
        DM_DISPLAYFREQUENCY;

    SetWindowLongPtr(hwnd, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST);
    SetWindowLongPtr(hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
    SetWindowPos(hwnd, HWND_TOP, 0, 0, fullscreenWidth, fullscreenHeight, SWP_SHOWWINDOW);
    isChangeSuccessful = ChangeDisplaySettings(&fullscreenSettings, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL;
    ShowWindow(hwnd, SW_MAXIMIZE);

    return isChangeSuccessful;
}

//Оконый режим
bool exitFullscreen(HWND hwnd, int windowX, int windowY, int windowedWidth, int windowedHeight, int windowedPaddingX, int windowedPaddingY) {
    bool isChangeSuccessful;

    SetWindowLongPtr(hwnd, GWL_EXSTYLE, WS_EX_LEFT);
    SetWindowLongPtr(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
    isChangeSuccessful = ChangeDisplaySettings(NULL, CDS_RESET) == DISP_CHANGE_SUCCESSFUL;
    SetWindowPos(hwnd, HWND_TOP, windowX, windowY, windowedWidth + windowedPaddingX, windowedHeight + windowedPaddingY, SWP_SHOWWINDOW);
    ShowWindow(hwnd, SW_RESTORE);

    return isChangeSuccessful;
}

//Начало игры пример
VOID StartGame() {

}

//Устоновка картинки фоном
VOID SetBackgroundIcons(){
    HWND hWnd = GetActiveWindow();
    HDC hdc = GetDC(hWnd), hdcMem;

    HBITMAP arrow = (HBITMAP)LoadImage(NULL, L"..\\BackgroundGame\\Menu.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hdcMem = CreateCompatibleDC(hdc);

    SelectObject(hdcMem, arrow);
    BitBlt(hdc, 0, 0, 1920, 1080, hdcMem, 0, 0, SRCCOPY);
}

//Прорисовка кнопки
/*
VOID DrawButton(LPDRAWITEMSTRUCT lpInfo) {
    HGDIOBJ obj;
    HBRUSH noactive = CreateSolidBrush(RGB(260, 260, 260)), focus = CreateSolidBrush(RGB(0, 255, 0)), select = CreateSolidBrush(RGB(255, 0, 0));
    HBITMAP bm;
    HDC Owner;

    Owner = CreateCompatibleDC(lpInfo->hDC);
    bm = CreateCompatibleBitmap(lpInfo->hDC, lpInfo->rcItem.right, lpInfo->rcItem.bottom);
    obj = SelectObject(Owner, bm);
    FillRect(Owner, &lpInfo->rcItem, noactive);
    switch (lpInfo->itemAction)
    {
    case ODA_FOCUS:
        if (lpInfo->itemState & ODS_FOCUS)
            FillRect(Owner, &lpInfo->rcItem, focus);
        if (lpInfo->itemState & ODS_CHECKED)
            FillRect(Owner, &lpInfo->rcItem, noactive);
        break;
    case ODA_SELECT:
        if (lpInfo->itemState & ODS_SELECTED)
            FillRect(Owner, &lpInfo->rcItem, select);
        break;
    case ODA_DRAWENTIRE:
        FillRect(Owner, &lpInfo->rcItem, noactive);
        break;
    }

    wchar_t wtext[] = L"Text Button";
    SetBkMode(Owner, TRANSPARENT);
    SetTextColor(Owner, RGB(255, 255, 0));
    TextOutW(Owner, 15, 5, wtext, wcslen(wtext));

    BitBlt(lpInfo->hDC, 0, 0, lpInfo->rcItem.right, lpInfo->rcItem.bottom, Owner, 0, 0, SRCCOPY);
    DeleteObject(SelectObject(Owner, obj));
    DeleteDC(Owner);
}*/

//Вспомогательный метод для изменение цвета
void MyButton(HWND & hwnd, int x, int y, int Width, int Height, int xe, int xy, HWND hParent)
{
    hwnd = CreateWindow(L"Button", L"Butt", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_OWNERDRAW | BS_PUSHBUTTON, x, y, Width, Height, hParent, (HMENU)13, (HINSTANCE)GetWindowLong(hParent, -6), NULL);
    HRGN rgn = CreateRoundRectRgn(0, 0, Width, Height, xe, xy);
    SetWindowRgn(My, rgn, TRUE);
}