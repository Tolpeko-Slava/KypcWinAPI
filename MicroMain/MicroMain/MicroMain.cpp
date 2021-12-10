#pragma comment(lib, "strmiids.lib")

#include "MicroMain.h"
#include "Windows.h"
#include "DShow.h"
#include <stdio.h>
#include <tchar.h>

#define MAX_LOADSTRING 100
#define ID_TIMER 100
#define TIMER_INTERVAL 50
#define NO_TIMER -1

#define ID_TIMER_Music 101
#define TIMER_INTERVAL_Music 75
long Event;// *Param1, * Param2;
LONG_PTR Param1, Param2;

// Глобальные переменные:
HINSTANCE hInst; // текущий экземпляр
HWND hWnd;       //Основное окно
HWND StringEx;   //ButtonMusicTest
HWND SreenFull;
HWND WindowsSizeSreen;
HWND ExitButt;
HWND OptionButt;
HWND GameButt;
HWND ButtonLevel1Start;
HWND BackButt;

//Потоки
HANDLE  hThreadFon;
DWORD  dwThreadId;

HANDLE  hThreadGame;
DWORD   dwThreadIdGame;

//Игра
UINT SrenXCordinat; //ширина
UINT SrenYCordinat; //высота
HBITMAP hBmp = (HBITMAP)LoadImage(NULL, L"..\\BackgroundGame\\temp1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
HBITMAP hBmpT = (HBITMAP)LoadImage(NULL, L"..\\BackgroundGame\\tempT.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
UINT xf = 765;
UINT yf = 720;
UINT xft = 745;
UINT yft = 760;
int IdTemer = NO_TIMER;
int IdTemerMusic = NO_TIMER;
int KofeX = 1, KofeY = 1;
RECT bootonTable;
bool ESCFlag = false;
bool FlagSpace = false;
UINT GameOverHight;
HCURSOR CurcisShow = NULL;
UINT GameFinal = 0;

//Флаги
bool FlagMenu = true;
bool FlagOption = false;
bool FlagGameMenu = false;

UINT Blok1X = 50;
UINT Blok1Y = 40;
HBITMAP Blok1 = (HBITMAP)LoadImage(NULL, L"..\\BackgroundGame\\Blok1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
bool FlagBlok1 = true;

UINT Blok2X = 220;
UINT Blok2Y = 40;
bool FlagBlok2 = true;

UINT Blok3X = 500;
UINT Blok3Y = 40;
bool FlagBlok3 = true;

UINT Blok4X = 790;
UINT Blok4Y = 40;
bool FlagBlok4 = true;

UINT Blok5X = 1000;
UINT Blok5Y = 40;
bool FlagBlok5 = true;

UINT Blok6X = 240;
UINT Blok6Y = 120;
bool FlagBlok6 = true;

UINT Blok7X = 440;
UINT Blok7Y = 120;
bool FlagBlok7 = true;

UINT Blok8X = 630;
UINT Blok8Y = 120;
bool FlagBlok8 = true;

UINT Blok9X = 460;
UINT Blok9Y = 340;
bool FlagBlok9 = true;


//Музыка
IGraphBuilder* pGraph = NULL;
IMediaEvent* pEvent = NULL;
IMediaControl* pControl = NULL;

IGraphBuilder* pGraphG = NULL;
IMediaEvent* pEventG = NULL;
IMediaControl* pControlG = NULL;

long timeMusic = 22080;
HRESULT hr;
bool FlagGameStart = false;

LPWSTR SoftMusic = (LPWSTR)L"D:\\KypcWinAPI\\MicroMain\\Music\\ZeroBlood.mp3";
LPWSTR SoftLevel1Music = (LPWSTR)L"D:\\KypcWinAPI\\MicroMain\\Music\\Yarmak.mp3";
LPWSTR SoftLevel2Music = (LPWSTR)L"D:\\KypcWinAPI\\MicroMain\\Music\\Viagra.mp3";
LPWSTR SoftLevel3Music = (LPWSTR)L"D:\\KypcWinAPI\\MicroMain\\Music\\AM.mp3";

//LPWSTR UserLevelMusic = (LPWSTR)L"";

LPWSTR SoftBackground = (LPWSTR)L"D:\\KypcWinAPI\\MicroMain\\BackgroundGame\\Menu.bmp";
LPWSTR SoftLevel1Background = (LPWSTR)L"D:\\KypcWinAPI\\MicroMain\\BackgroundGame\\Level1.bmp";

LPWSTR UsengBackground;

DWORD   dwThreadIdMusiBackground;
HANDLE  hThreadMusicBackground;


//Меню приложения
POINT ExitButton{ 700, 620};
POINT OptionButton{ 700, 530};
POINT GameButton{ 700, 440};
POINT BackButton{ 700, 620};
POINT ScrenFullButton{ 700, 480};
POINT ScrenWindowButton{ 700, 520};
POINT FileOpenButton{ 685, 500};
POINT StartGameButton{ 700, 560};


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
bool FlagFileDialof = false;

//Предобъявление
VOID MidiWin(LPWSTR Path);
VOID TreaderMusic(LPWSTR Path);
VOID TreaderMusicGame();
VOID StartGame();
VOID SetBackgroundIcons();
VOID DrawButton(LPDRAWITEMSTRUCT lpInfo);
VOID MidiWinG();
VOID MoveBall();
VOID MoveTable(int KofeMove);
VOID SetStartGame();
VOID SetGame();
VOID ChekElement();
UINT ChekBlok(UINT XPik, UINT YPik, UINT XBlok, UINT YBlok);
bool enterFullscreen(HWND hwnd, int fullscreenWidth, int fullscreenHeight, int colourBits, int refreshRate);
bool exitFullscreen(HWND hwnd, int windowX, int windowY, int windowedWidth, int windowedHeight, int windowedPaddingX, int windowedPaddingY);

void MyButton(HWND & hwnd, int x, int y, int Width, int Height, int xe, int xy, UINT num, HWND hParent);

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
           // TranslateMessage(&msg);
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

   
   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, 0, hInstance, nullptr);

   //Игра
   StringEx = CreateWindowEx(NULL, L"Edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL,
       1900, 60,5, 30, hWnd, (HMENU)2, (HINSTANCE)GetWindowLongA(hWnd, -6), NULL);

   MyButton(ButtonLevel1Start, StartGameButton.x, StartGameButton.y, 120, 30, 40, 30, 4, hWnd);
   MyButton(FaliOpen, FileOpenButton.x, FileOpenButton.y, 150, 30, 40, 30, 5, hWnd);


   //Настройки
   MyButton(SreenFull, ScrenFullButton.x, ScrenFullButton.y, 120, 30, 40, 30, 1, hWnd);
   MyButton(WindowsSizeSreen, ScrenWindowButton.x, ScrenWindowButton.y, 120, 30, 40, 30, 3, hWnd);

   //Меню
   MyButton(GameButt, GameButton.x, GameButton.y, 120, 30, 40, 30, 8, hWnd);
   MyButton(OptionButt, OptionButton.x, OptionButton.y, 120, 30, 40, 30, 7, hWnd);
   MyButton(ExitButt, ExitButton.x, ExitButton.y, 120, 30, 40, 30, 6, hWnd);
   
   MyButton(BackButt, BackButton.x,BackButton.y, 120, 30, 40, 30, 9, hWnd);


   if (!hWnd)
   {
      return FALSE;
   }
  
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   TreaderMusic(SoftMusic);

   //Экран начало
   enterFullscreen(hWnd, 1080, 1920, 5, 60);

   ShowWindow(SreenFull, SW_HIDE);
   ShowWindow(BackButt, SW_HIDE);
   ShowWindow(WindowsSizeSreen, SW_HIDE);
   ShowWindow(ButtonLevel1Start, SW_HIDE);
   ShowWindow(FaliOpen, SW_HIDE);

   //Изменение цвета при наведении
   SetWindowLong(FaliOpen, GWL_EXSTYLE, GetWindowLong(FaliOpen, GWL_EXSTYLE) | WS_EX_LAYERED);
   SetLayeredWindowAttributes(FaliOpen, RGB(255, 0, 0), 10, LWA_COLORKEY);

   FlagGameStart = false;

   UsengBackground = SoftBackground;

   IdTemerMusic = SetTimer(hWnd, ID_TIMER_Music, TIMER_INTERVAL_Music, (TIMERPROC)NULL);

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

                    pControl->Stop();
                    CloseHandle(hThreadFon);

                   /* pControl->Release();
                    pEvent->Release();
                    pGraph->Release();*/

                    /*if (hThreadFon == NULL)
                    {
                        dwThreadId = 0;
                        hThreadFon = NULL;
                    }*/

                  //  UserLevelMusic = NULL;
                  //  UserPathMusic = SoftLevel1Music;
                    if (FlagFileDialof == true)
                    {
                        GetWindowText(StringEx, UserPathMusic, 450);
                    }
                    else
                    {
                        UserPathMusic = SoftLevel1Music;
                    }
                    TreaderMusicGame();

                    ShowWindow(BackButt, SW_HIDE);
                    ShowWindow(ButtonLevel1Start, SW_HIDE);
                    ShowWindow(FaliOpen, SW_HIDE);

                    FlagGameStart = true;

                    bootonTable.left = 0;
                    bootonTable.right = SrenXCordinat;
                    bootonTable.bottom = yft + 25;
                    bootonTable.top = yft;

                    GameOverHight = 3;
                    KillTimer(hWnd,IdTemerMusic);
                    pEvent->FreeEventParams(Event, Param1, Param2);
                    IdTemerMusic = SetTimer(hWnd, ID_TIMER_Music, TIMER_INTERVAL_Music, (TIMERPROC)NULL);
                  //  SetGame();
                    SetStartGame();
                    StartGame();
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
                    OPENFILENAME ofn = {0};
                    TCHAR szFile[420] = {0};

                    ofn.lStructSize = sizeof(ofn);
                    ofn.hwndOwner = hWnd;
                    ofn.lpstrFile = szFile;
                    ofn.nMaxFile = sizeof(szFile);
                    ofn.lpstrFilter = L"*\0*.mp3\0";
                    //
                    ofn.nFilterIndex = 1;
                    ofn.lpstrFileTitle = NULL;
                    ofn.nMaxFileTitle = 0;
                    ofn.lpstrInitialDir = NULL;
                    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
                    GetOpenFileName(&ofn);


                    UserPathMusic = ofn.lpstrFile;
                   // SetWindowText(StringEx, UserPathMusic);
                    SetWindowText(StringEx, ofn.lpstrFile);
                    FlagFileDialof = true;
                    InvalidateRect(hWnd, NULL, false);
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
                    FlagOption = true;
                    FlagMenu = false;
                }
                break;

                //Играть
                case 8:
                {
                    ShowWindow(ButtonLevel1Start, SW_SHOW);
                    ShowWindow(FaliOpen, SW_SHOW);
                    ShowWindow(BackButt, SW_SHOW);

                    ShowWindow(GameButt, SW_HIDE);
                    ShowWindow(OptionButt, SW_HIDE);
                    ShowWindow(ExitButt, SW_HIDE);
                    
                    FlagGameMenu = true;
                    FlagMenu = false;
                }
                break;
                
                //Назад
                case 9:
                {
                    if (FlagGameStart == false) {
                        ShowWindow(GameButt, SW_SHOW);
                        ShowWindow(OptionButt, SW_SHOW);
                        ShowWindow(ExitButt, SW_SHOW);

                        ShowWindow(ButtonLevel1Start, SW_HIDE);
                        ShowWindow(FaliOpen, SW_HIDE);
                        ShowWindow(SreenFull, SW_HIDE);
                        ShowWindow(WindowsSizeSreen, SW_HIDE);
                        ShowWindow(BackButt, SW_HIDE);

                        FlagOption = false;
                        FlagGameMenu = false;
                        FlagMenu = true;
                    }
                    if (FlagGameStart == true)
                    {
                        UsengBackground = SoftBackground;
                        
                        pControlG->Stop();
                        CloseHandle(hThreadGame);

                       /*if (hThreadGame != NULL) 
                        {
                            dwThreadIdGame = 0;
                            hThreadGame = NULL;
                        }*/


                        TreaderMusic(SoftMusic);

                        KillTimer(hWnd, IdTemerMusic);
                        pEventG->FreeEventParams(Event, Param1, Param2);
                        IdTemerMusic = SetTimer(hWnd, ID_TIMER_Music, TIMER_INTERVAL_Music, (TIMERPROC)NULL);

                        ShowWindow(ButtonLevel1Start, SW_SHOW);
                        ShowWindow(FaliOpen, SW_SHOW);

                        ShowWindow(BackButt, SW_HIDE);
                        ShowWindow(BackButt, SW_SHOW);

                        FlagGameStart = false;
                        ESCFlag = false;
                        FlagGameMenu = true;
                    }
                }
                break;

                }
            }


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

        case WM_SIZE: {
            SrenXCordinat = LOWORD(lParam); 
            SrenYCordinat = HIWORD(lParam);
            if ((SrenXCordinat < 700)&&(SrenXCordinat>40))
            {
                SetWindowPos(hWnd, HWND_TOP, 100, 50, 700, 600, SWP_SHOWWINDOW);
            }
            ChekElement();
            if (FlagGameStart == true)
            {
                SetWindowPos(hWnd, HWND_TOP, 100, 50, SrenXCordinat, SrenYCordinat, SWP_SHOWWINDOW);
            }
        }

        break;

            case WM_PAINT:
            {
               
                PAINTSTRUCT ps;
                hdc = BeginPaint(hWnd, &ps);

                SetBackgroundIcons();             

                if (FlagGameStart == true)
                {
                    GameFinal = 0;
                        HDC bmpDC = CreateCompatibleDC(hdc);
                        SelectObject(bmpDC, hBmp);

                        BitBlt(hdc, xf, yf, 20, 20, bmpDC, 0, 0, SRCCOPY);
                        DeleteDC(bmpDC);

                        bmpDC = CreateCompatibleDC(hdc);
                        SelectObject(bmpDC, hBmpT);

                        BitBlt(hdc, xft, yft, 140, 25, bmpDC, 0, 0, SRCCOPY);
                        DeleteDC(bmpDC);

                        if (FlagBlok1 == true)
                        {
                            bmpDC = CreateCompatibleDC(hdc);
                            SelectObject(bmpDC, Blok1);

                            BitBlt(hdc, Blok1X, Blok1Y, 140, 25, bmpDC, 0, 0, SRCCOPY);
                            DeleteDC(bmpDC);

                            GameFinal = GameFinal + 1;
                        }

                        if (FlagBlok2 == true)
                        {
                            bmpDC = CreateCompatibleDC(hdc);
                            SelectObject(bmpDC, Blok1);

                            BitBlt(hdc, Blok2X, Blok2Y, 140, 25, bmpDC, 0, 0, SRCCOPY);
                            DeleteDC(bmpDC);

                            GameFinal = GameFinal + 1;
                        }

                        if (FlagBlok3 == true)
                        {
                            bmpDC = CreateCompatibleDC(hdc);
                            SelectObject(bmpDC, Blok1);

                            BitBlt(hdc, Blok3X, Blok3Y, 140, 25, bmpDC, 0, 0, SRCCOPY);
                            DeleteDC(bmpDC);

                            GameFinal = GameFinal + 1;
                        }

                        if (FlagBlok4 == true)
                        {
                            bmpDC = CreateCompatibleDC(hdc);
                            SelectObject(bmpDC, Blok1);

                            BitBlt(hdc, Blok4X, Blok4Y, 140, 25, bmpDC, 0, 0, SRCCOPY);
                            DeleteDC(bmpDC);

                            GameFinal = GameFinal + 1;
                        }

                        if (FlagBlok5 == true)
                        {
                            bmpDC = CreateCompatibleDC(hdc);
                            SelectObject(bmpDC, Blok1);

                            BitBlt(hdc, Blok5X, Blok5Y, 140, 25, bmpDC, 0, 0, SRCCOPY);
                            DeleteDC(bmpDC);

                            GameFinal = GameFinal + 1;
                        }

                        if (FlagBlok6 == true)
                        {
                            bmpDC = CreateCompatibleDC(hdc);
                            SelectObject(bmpDC, Blok1);

                            BitBlt(hdc, Blok6X, Blok6Y, 140, 25, bmpDC, 0, 0, SRCCOPY);
                            DeleteDC(bmpDC);

                            GameFinal = GameFinal + 1;
                        }

                        if (FlagBlok7 == true)
                        {
                            bmpDC = CreateCompatibleDC(hdc);
                            SelectObject(bmpDC, Blok1);

                            BitBlt(hdc, Blok7X, Blok7Y, 140, 25, bmpDC, 0, 0, SRCCOPY);
                            DeleteDC(bmpDC);

                            GameFinal = GameFinal + 1;
                        }

                        if (FlagBlok8 == true)
                        {
                            bmpDC = CreateCompatibleDC(hdc);
                            SelectObject(bmpDC, Blok1);

                            BitBlt(hdc, Blok8X, Blok8Y, 140, 25, bmpDC, 0, 0, SRCCOPY);
                            DeleteDC(bmpDC);

                            GameFinal = GameFinal + 1;
                        }

                        if (FlagBlok9 == true)
                        {
                            bmpDC = CreateCompatibleDC(hdc);
                            SelectObject(bmpDC, Blok1);

                            BitBlt(hdc, Blok9X, Blok9Y, 140, 25, bmpDC, 0, 0, SRCCOPY);
                            DeleteDC(bmpDC);

                            GameFinal = GameFinal + 1;
                        }

                        if (GameFinal == 0)
                        {
                            GameOverHight = 0;
                            SetStartGame();
                        }
                }

                EndPaint(hWnd, &ps);

            }
            break;

        //Прорисовка
        case WM_DRAWITEM:
        {
            DrawButton((LPDRAWITEMSTRUCT)lParam);
            return true;
        }

            //esc при игре 
            case WM_KEYDOWN:
            {
                switch (wParam)
                {
                    //ESC
                    case 0x1B:
                    {
                        if (FlagGameStart == true) {
                            if (ESCFlag == false) {
                                InvalidateRect(hWnd, NULL, false);
                                KillTimer(hWnd, IdTemer);
                                ShowWindow(BackButt, SW_SHOW);
                                ESCFlag = true;
                                SetCursor(CurcisShow);
                                break;
                            }
                            else
                            {
                                SetCursor(NULL);
                                IdTemer = SetTimer(hWnd, ID_TIMER, TIMER_INTERVAL, (TIMERPROC)NULL);
                                ShowWindow(BackButt, SW_HIDE);
                                ESCFlag = false;
                                break;
                            }
                        }
                    }
                    break;

                    //<-
                    case 0x25:
                    {
                        if (FlagGameStart == true) {
                            if (ESCFlag == false)
                            {
                                if (FlagSpace == true)
                                {
                                    MoveTable(-1);
                                    InvalidateRect(hWnd, &bootonTable, false);
                                }
                                else
                                {
                                    MoveTable(-1);
                                    xf = xf - 10;
                                    InvalidateRect(hWnd, NULL, false);
                                }
                            }
                        }
                       // break;
                    }
                    break;

                    //->
                    case 0x27:
                    {
                        if (FlagGameStart == true) {
                            if (ESCFlag == false)
                            {
                                if (FlagSpace == true)
                                {
                                    MoveTable(1);
                                    InvalidateRect(hWnd, &bootonTable, false);
                                }
                                else
                                {
                                    MoveTable(1);
                                    xf = xf + 10;
                                    InvalidateRect(hWnd, NULL, false);
                                }
                            }
                        }
                       // break;
                    }
                    break;

                    //Space
                    case 0x20:
                    {
                        if (FlagGameStart == true) {
                            if (ESCFlag == false)
                            {
                                IdTemer = SetTimer(hWnd, ID_TIMER, TIMER_INTERVAL, (TIMERPROC)NULL);
                                FlagSpace = true;
                            }
                        }
                        // break;
                    }
                    break;
                }
            }

        case WM_TIMER:
        {
            case ID_TIMER:
            {
                if (ESCFlag == false)
                {
                    if (FlagSpace == true)
                    {
                        MoveBall();
                        InvalidateRect(hWnd, NULL, false);
                    }
                }
            }

            case ID_TIMER_Music:
            {
                if (FlagGameStart == false)
                {
                    pEvent->GetEvent( &Event, &Param1, &Param2, 10);
                    if (Event == EC_COMPLETE)
                    {
                         pControl->Release();
                         pEvent->Release();
                         pGraph->Release();
                         CloseHandle(hThreadFon);
                         TreaderMusic(SoftMusic);
                    }
                    pEvent->FreeEventParams( Event, Param1, Param2);
                }
                else
                {
                    pEventG->GetEvent(&Event, &Param1, &Param2, 10);
                    if (Event == EC_COMPLETE)
                    {
                        pControlG->Release();
                        pEventG->Release();
                        pGraphG->Release();
                        CloseHandle(hThreadGame);
                        if (FlagFileDialof == true)
                        {
                            GetWindowText(StringEx, UserPathMusic, 450);
                        }
                        else
                        {
                            UserPathMusic = SoftLevel1Music;
                        }
                        TreaderMusicGame();
                    }
                    pEventG->FreeEventParams(Event, Param1, Param2);
                }
                break;
            }
        }

        //Вхождение корсора в окно
        case WM_SETCURSOR:
        {
            //Игра
            if ((HWND)wParam == GameButt)
            {
                SetFocus(GameButt);
            }

            //Настройки
            else if ((HWND)wParam == OptionButt)
            {
                SetFocus(OptionButt);
            }

            //Выход
            else if ((HWND)wParam == ExitButt)
            {
                SetFocus(ExitButt);
            }

            //Оконый
            else if ((HWND)wParam == WindowsSizeSreen)
            {
                SetFocus(WindowsSizeSreen);
            }

            //Полноэкранный
            else if ((HWND)wParam == SreenFull)
            {
                SetFocus(SreenFull);
            }

            //Играть
            else if ((HWND)wParam == ButtonLevel1Start)
            {
                SetFocus(ButtonLevel1Start);
            }

            //Назад
            else if ((HWND)wParam == BackButt)
            {
                SetFocus(BackButt);
            }

            //Изменить музыку
            else if ((HWND)wParam == FaliOpen)
            {
                SetFocus(FaliOpen);
            }

            else if ((HWND)wParam != GameButt)
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

//Создание потока фона
VOID TreaderMusic(LPWSTR Path) {
    hThreadFon = CreateThread(
        NULL,                   
        0,                      
        (LPTHREAD_START_ROUTINE)MidiWin,
        Path,
        0,                      
        &dwThreadId);   
    DWORD ret = GetLastError();
    if (hThreadFon == NULL)
    {
        ExitProcess(3);
    }

    //CloseHandle(hThreadFon);
}

//Создание потока игры
VOID TreaderMusicGame() {
    hThreadGame = CreateThread(
        NULL,
        0,
        (LPTHREAD_START_ROUTINE)MidiWinG,
        NULL,
        0,
        &dwThreadIdGame);
    DWORD ret = GetLastError();
    if (hThreadGame == NULL)
    {
        ExitProcess(3);
    }

  //  CloseHandle(hThreadGame);
}

//Воспроизведение музыки
VOID MidiWin(LPWSTR Path) {
    hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        printf("ERROR - Could not initialize COM library");
        return;
    }

    hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
        IID_IGraphBuilder, (void**)&pGraph);
    if (FAILED(hr))
    {
        printf("ERROR - Could not create the Filter Graph Manager.");
        return;
    }

    hr = pGraph->QueryInterface(IID_IMediaControl, (void**)&pControl);
    hr = pGraph->QueryInterface(IID_IMediaEvent, (void**)&pEvent);

    hr = pGraph->RenderFile((LPCWSTR)Path, NULL);
    if (SUCCEEDED(hr))
    {
        hr = pControl->Run();
        if (SUCCEEDED(hr))
        {
            // Wait for completion.
            long evCode;
            pEvent->WaitForCompletion(timeMusic, &evCode);
        }
    }
   /* pControl->Release();
    pEvent->Release();
    pGraph->Release();*/
    CoUninitialize();
}

//Воспроизведение игравой музыки
VOID MidiWinG() {
    hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        printf("ERROR - Could not initialize COM library");
        return;
    }

    hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
        IID_IGraphBuilder, (void**)&pGraphG);
    if (FAILED(hr))
    {
        printf("ERROR - Could not create the Filter Graph Manager.");
        return;
    }
    
    hr = pGraphG->QueryInterface(IID_IMediaControl, (void**)&pControlG);
    hr = pGraphG->QueryInterface(IID_IMediaEvent, (void**)&pEventG);

        hr = pGraphG->RenderFile((LPCWSTR)UserPathMusic, NULL);
        DWORD ret = GetLastError();
        if (SUCCEEDED(hr))
        {
            hr = pControlG->Run();
            if (SUCCEEDED(hr))
            {
                long evCode;
                pEventG->WaitForCompletion(timeMusic, &evCode);
            }
        }

    /*pControlG->Release();
    pEventG->Release();
    pGraphG->Release();*/
    CoUninitialize();
}

//Полноэкраный режим
bool enterFullscreen(HWND hwnd, int fullscreenWidth, int fullscreenHeight, int colourBits, int refreshRate) {
    DEVMODE fullscreenSettings;
    bool isChangeSuccessful;

    ZeroMemory(&fullscreenSettings, sizeof(DEVMODE));

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

//Начало игры
VOID StartGame() {
    UsengBackground = SoftLevel1Background;

    SetBackgroundIcons();

    InvalidateRect(hWnd,NULL,false);
    
    IdTemer = SetTimer(hWnd, ID_TIMER, TIMER_INTERVAL, (TIMERPROC)NULL);

    KillTimer(hWnd, IdTemer);

    KofeX = 1;
    KofeY = 1;
}

//Кординаты пикселя
VOID MoveBall()
{
    xf = xf + KofeX * 5;
    yf = yf + KofeY * 5;

    UINT chek;

    if (FlagBlok1 == true)
    {
        chek = ChekBlok(xf, yf, Blok1X, Blok1Y);

        if (chek != 0)
        {
            FlagBlok1 = false;
            return;
        }

    }
    if (FlagBlok2 == true)
    {
        chek = ChekBlok(xf, yf, Blok2X, Blok2Y);

        if (chek != 0)
        {
            FlagBlok2 = false;
            return;
        }

    }
    if (FlagBlok3 == true)
    {
        chek = ChekBlok(xf, yf, Blok3X, Blok3Y);

        if (chek != 0)
        {
            FlagBlok3 = false;
            return;
        }

    }
    if (FlagBlok4 == true)
    {
        chek = ChekBlok(xf, yf, Blok4X, Blok4Y);

        if (chek != 0)
        {
            FlagBlok4 = false;
            return;
        }

    }
    if (FlagBlok5 == true)
    {
        chek = ChekBlok(xf, yf, Blok5X, Blok5Y);

        if (chek != 0)
        {
            FlagBlok5 = false;
            return;
        }

    }
    if (FlagBlok6== true)
    {
        chek = ChekBlok(xf, yf, Blok6X, Blok6Y);

        if (chek != 0)
        {
            FlagBlok6 = false;
            return;
        }

    }
    if (FlagBlok7 == true)
    {
        chek = ChekBlok(xf, yf, Blok7X, Blok7Y);

        if (chek != 0)
        {
            FlagBlok7 = false;
            return;
        }

    }
    if (FlagBlok8 == true)
    {
        chek = ChekBlok(xf, yf, Blok8X, Blok8Y);

        if (chek != 0)
        {
            FlagBlok8 = false;
            return;
        }

    }
    if (FlagBlok9 == true)
    {
        chek = ChekBlok(xf, yf, Blok9X, Blok9Y);

        if (chek != 0)
        {
            FlagBlok9 = false;
            return;
        }

    }

    //Платформа
    if (( yf + 20 == yft) && ( xf <= xft + 140 ) && ( xf >= xft ))
    {
        KofeY = KofeY * (-1);
        return;
    }

    if (xf + 20 >= SrenXCordinat)
    {
        KofeX = KofeX * (- 1);
        return;
    }
    if (xf <= 0)
    {
        KofeX = KofeX * (-1);
        return;
    }
    if (yf + 20 >= SrenYCordinat)
    {
        GameOverHight = GameOverHight - 1;
        SetStartGame();
        return;
    }
    if (yf <= 5)
    {
        KofeY = KofeY * (-1);
        return;
    }
    return;
}

VOID SetStartGame()
{
    KofeX = 1;
    KofeY = 1;
    xft = SrenXCordinat * 745 / 1580;
    yft = SrenYCordinat * 760 / 840;
    xf = xft + 20;
    yf = yft - 40;
    FlagSpace = false;
    if (GameOverHight == 0)
    {
        ShowWindow(BackButt, SW_SHOW);
        KillTimer(hWnd, IdTemer);
        ESCFlag = true;
        FlagGameStart = true;
    }
}

VOID SetGame()
{
    FlagBlok1 = true;
    FlagBlok2 = true;
    FlagBlok3 = true;
    FlagBlok4 = true;
    FlagBlok5 = true;
    FlagBlok6 = true;
    FlagBlok7 = true;
    FlagBlok8 = true;
    FlagBlok9 = true;
    GameOverHight = 3;
    CurcisShow = SetCursor(NULL);
}

//Проверка на блок
UINT ChekBlok( UINT XPik, UINT YPik, UINT XBlok, UINT YBlok)
{
    UINT WBlok = 140;
    UINT HBlok = 25;
    UINT WP = 20;
    UINT HP = 20;

    if ((XPik <= XBlok + WBlok) && (YPik + HP >= YBlok) && (YPik <= YBlok + HBlok) && (XPik + WP >= XBlok))
    {
        if (KofeY > 0)
        {
            KofeY = KofeY * (-1);
            return 1;
        }
        if (KofeY < 0)
        {
            KofeY = KofeY * (-1);
            return 1;
        }
        /*if (KofeX < 0)
        {
            KofeX = KofeX * (-1);
            return 1;
        }
        if (KofeX > 0)
        {
            KofeX = KofeX * (-1);
            return 1;
        }*/
    }
    return 0;
}

//Движение платформы
VOID MoveTable(int KofeMove)
{
    xft = xft + KofeMove * 10;

    if (xft + 140 >= SrenXCordinat)
    {
        xft = xft - 10;
        return;
    }
    if ((xft <= 0)||(xft > SrenXCordinat))
    {
        xft = xft + 10;
        int r = 3;
        return;
    }
    return;
}

//Устоновка картинки фоном
VOID SetBackgroundIcons(){
    HWND hWnd = GetActiveWindow();
    HDC hdc = GetDC(hWnd), hdcMem;

    hdcMem = CreateCompatibleDC(hdc);

    if (FlagGameStart == false)
    {

        HBITMAP arrow = (HBITMAP)LoadImage(NULL, SoftBackground, IMAGE_BITMAP, 1920, 1080, LR_LOADFROMFILE);
        SelectObject(hdcMem, arrow);
    }
    else
    {

        HBITMAP arrowg = (HBITMAP)LoadImage(NULL, SoftLevel1Background, IMAGE_BITMAP, 1920, 1080, LR_LOADFROMFILE);
        SelectObject(hdcMem, arrowg);
    }
    BitBlt(hdc, 0, 0, 1920, 1080, hdcMem, 0, 0, SRCCOPY);

    DeleteDC(hdc);
}

//Проверка окных элементов
VOID ChekElement() 
{
    UINT CenterButtonXElement;

    CenterButtonXElement = SrenXCordinat / 2;

    ExitButton.x = CenterButtonXElement;
    OptionButton.x = CenterButtonXElement;
    GameButton.x = CenterButtonXElement;
    BackButton.x = CenterButtonXElement;
    StartGameButton.x = CenterButtonXElement;
    ScrenFullButton.x = CenterButtonXElement;
    ScrenWindowButton.x = CenterButtonXElement;
    FileOpenButton.x = CenterButtonXElement - 12;

    ExitButton.y = SrenYCordinat * 620 / 840;
    OptionButton.y = SrenYCordinat * 530 / 840;
    GameButton.y = SrenYCordinat * 440 / 840;
    BackButton.y = SrenYCordinat * 620 / 840;
    StartGameButton.y = SrenYCordinat * 560 / 840;
    ScrenFullButton.y = SrenYCordinat * 480 / 840;
    ScrenWindowButton.y = SrenYCordinat * 520 / 840;
    FileOpenButton.y = SrenYCordinat * 500 / 840;

    Blok1X = SrenXCordinat * 50 / 1580;
    Blok2X = SrenXCordinat * 220 / 1580;
    Blok3X = SrenXCordinat * 500 / 1580;
    Blok4X = SrenXCordinat * 790 / 1580;
    Blok5X = SrenXCordinat * 1000 / 1580;
    if (Blok5X + 140 >= SrenXCordinat)
    {
        FlagBlok5 = false;
    }

    Blok6X = SrenXCordinat * 240 / 1580;
    Blok7X = SrenXCordinat * 440 / 1580;
    Blok8X = SrenXCordinat * 630 / 1580;
    Blok9X = SrenXCordinat * 460 / 1580;

    Blok1Y = SrenYCordinat * 40 / 1580;
    Blok2Y = SrenYCordinat * 40 / 1580;
    Blok3Y = SrenYCordinat * 40 / 1580;
    Blok4Y = SrenYCordinat * 40 / 1580;
    Blok5Y = SrenYCordinat * 40 / 1580;
    Blok6Y = SrenYCordinat * 120 / 1580;
    Blok7Y = SrenYCordinat * 120 / 1580;
    Blok8Y = SrenYCordinat * 120 / 1580;
    Blok9Y = SrenYCordinat * 340 / 1580;

    xft = SrenXCordinat * 745 / 1580;
    yft = SrenYCordinat * 760 / 840;
    xf = xft + 20;
    yf = yft - 40;

    SetWindowPos(ExitButt, HWND_TOP, ExitButton.x, ExitButton.y, 120, 30, SWP_SHOWWINDOW);
    SetWindowPos(ButtonLevel1Start, HWND_TOP, StartGameButton.x, StartGameButton.y, 120, 30, SWP_SHOWWINDOW);
    SetWindowPos(FaliOpen, HWND_TOP, FileOpenButton.x, FileOpenButton.y, 150, 30, SWP_SHOWWINDOW);
    SetWindowPos(SreenFull, HWND_TOP, ScrenFullButton.x, ScrenFullButton.y, 120, 30, SWP_SHOWWINDOW);
    SetWindowPos(WindowsSizeSreen, HWND_TOP, ScrenWindowButton.x, ScrenWindowButton.y, 120, 30, SWP_SHOWWINDOW);
    SetWindowPos(GameButt, HWND_TOP, GameButton.x, GameButton.y, 120, 30, SWP_SHOWWINDOW);
    SetWindowPos(OptionButt, HWND_TOP, OptionButton.x, OptionButton.y, 120, 30, SWP_SHOWWINDOW);
    SetWindowPos(BackButt, HWND_TOP, BackButton.x, BackButton.y, 120, 30, SWP_SHOWWINDOW);

    if (FlagOption == true) 
    {
        ShowWindow(ExitButt, SW_HIDE);
        ShowWindow(ButtonLevel1Start, SW_HIDE);
        ShowWindow(FaliOpen, SW_HIDE);
        ShowWindow(GameButt, SW_HIDE);
        ShowWindow(OptionButt, SW_HIDE);
        return;
    }
    if (FlagMenu == true)
    {
        ShowWindow(ButtonLevel1Start, SW_HIDE);
        ShowWindow(FaliOpen, SW_HIDE);
        ShowWindow(SreenFull, SW_HIDE);
        ShowWindow(BackButt, SW_HIDE);
        return;
    }
    if (FlagGameMenu == true)
    {
        ShowWindow(ExitButt, SW_HIDE);
        ShowWindow(SreenFull, SW_HIDE);
        ShowWindow(WindowsSizeSreen, SW_HIDE);
        ShowWindow(GameButt, SW_HIDE);
        ShowWindow(OptionButt, SW_HIDE);
        return;
    }

}

//Прорисовка кнопки
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

    switch (lpInfo->CtlID)
    {
        //Музыка
        case 4:
        {
            wchar_t wtext[] = L"Начать игру";
            SetBkMode(Owner, TRANSPARENT);
            SetTextColor(Owner, RGB(255, 255, 0));
            TextOutW(Owner, 20, 5, wtext, wcslen(wtext));
        }
        break;

        //Полный экран
        case 1:
        {
            wchar_t wtext[] = L"Полноэкранный";
            SetBkMode(Owner, TRANSPARENT);
            SetTextColor(Owner, RGB(255, 255, 0));
            TextOutW(Owner, 5, 5, wtext, wcslen(wtext));
        }
        break;

        //Окрный режим
        case 3:
        {
            wchar_t wtext[] = L"Оконный";
            SetBkMode(Owner, TRANSPARENT);
            SetTextColor(Owner, RGB(255, 255, 0));
            TextOutW(Owner, 30, 5, wtext, wcslen(wtext));
        }
        break;

        //FaliDialog
        case 5:
        {
            wchar_t wtext[] = L"Изменить музыку";
            SetBkMode(Owner, TRANSPARENT);
            SetTextColor(Owner, RGB(255, 255, 0));
            TextOutW(Owner, 15, 5, wtext, wcslen(wtext));
        }
        break;

        //Exit
        case 6:
        {
            wchar_t wtext[] = L"Выход";
            SetBkMode(Owner, TRANSPARENT);
            SetTextColor(Owner, RGB(255, 255, 0));
            TextOutW(Owner, 40, 5, wtext, wcslen(wtext));
        }
        break;

        //Настройки
        case 7: 
        {
            wchar_t wtext[] = L"Настройки";
            SetBkMode(Owner, TRANSPARENT);
            SetTextColor(Owner, RGB(255, 255, 0));
            TextOutW(Owner, 30, 5, wtext, wcslen(wtext));
        }
        break;

        //Играть
        case 8:
        {
            wchar_t wtext[] = L"Игра";
            SetBkMode(Owner, TRANSPARENT);
            SetTextColor(Owner, RGB(255, 255, 0));
            TextOutW(Owner, 45, 5, wtext, wcslen(wtext));
        }
        break;

        //Назад
        case 9:
        {
            wchar_t wtext[] = L"Назад";
            SetBkMode(Owner, TRANSPARENT);
            SetTextColor(Owner, RGB(255, 255, 0));
            TextOutW(Owner, 40, 5, wtext, wcslen(wtext));
        }
        break;

        //Вперед
        case 10:
        {
            wchar_t wtext[] = L"N";
            SetBkMode(Owner, TRANSPARENT);
            SetTextColor(Owner, RGB(255, 255, 0));
            TextOutW(Owner, 10, 5, wtext, wcslen(wtext));
        }
        break;

        //Назад
        case 11:
        {
            wchar_t wtext[] = L"L";
            SetBkMode(Owner, TRANSPARENT);
            SetTextColor(Owner, RGB(255, 255, 0));
            TextOutW(Owner, 10, 5, wtext, wcslen(wtext));
        }
        break;

        default:
            break;
    }



    BitBlt(lpInfo->hDC, 0, 0, lpInfo->rcItem.right, lpInfo->rcItem.bottom, Owner, 0, 0, SRCCOPY);
    DeleteObject(SelectObject(Owner, obj));
    DeleteDC(Owner);
}

//Вспомогательный метод для изменение цвета
void MyButton(HWND & hwnd, int x, int y, int Width, int Height, int xe, int xy, UINT num, HWND hParent)
{
    hwnd = CreateWindow(L"Button", L"Butt", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_OWNERDRAW | BS_PUSHBUTTON, x, y, Width, Height, hParent, (HMENU)num, (HINSTANCE)GetWindowLong(hParent, -6), NULL);
    HRGN rgn = CreateRoundRectRgn(0, 0, Width, Height, xe, xy);
    SetWindowRgn(hwnd, rgn, TRUE);
}