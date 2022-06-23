// KarutaGunma.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "DSP.h"
#include "KarutaGunma.h"

#define MAX_LOADSTRING 100

namespace plt = matplotlibcpp;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
    std::vector<double> y = { 1, 3, 2, 4 };
    plt::plot(y);
    plt::savefig("minimal.pdf");
    //sf_open to open file

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_KARUTAGUNMA, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_KARUTAGUNMA));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_KARUTAGUNMA));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_KARUTAGUNMA);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case ID_FILE_LOADFILE:
                   {
                    // common dialog box structure, setting all fields to 0 is important
                    OPENFILENAME ofn = { 0 };
                    TCHAR szFile[260] = { 0 };
                    // Initialize remaining fields of OPENFILENAME structure
                    ofn.lStructSize = sizeof(ofn);
                    ofn.hwndOwner = hWnd;
                    ofn.lpstrFile = szFile;
                    ofn.nMaxFile = sizeof(szFile);
                    ofn.lpstrFilter = _T("Wav\0*.WAV\0All\0*.*\0");
                    ofn.nFilterIndex = 1;
                    ofn.lpstrFileTitle = NULL;
                    ofn.nMaxFileTitle = 0;
                    ofn.lpstrInitialDir = NULL;
                    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

                    if (GetOpenFileName(&ofn) == TRUE)
                    {
                        // use ofn.lpstrFile here
                        std::wstring arr_w(szFile);
                        std::string fileName(arr_w.begin(), arr_w.end());
                        
                        SndfileHandle myf = SndfileHandle(fileName);

                        /*
                        std::string output = " ";

                        output += "Opened file  :" + fileName + '\n';
                        output += "Sample rate  :" + myf.samplerate() + '\n';
                        output += "Channels     :" + myf.channels() + '\n';
                        output += "Error        :" + std::string( myf.strError()) + '\n';
                        output += "Frames       :" + int(myf.frames()) + '\n\n';
                        */
                        

                        //write to file
                        std::ofstream myfile;
                        myfile.open("example.txt",std::ios::app);
                        myfile << "\n\n";
                        myfile << "Opened file  :" << fileName << '\n';
                        myfile << "Error        :" << std::string(myf.strError()) <<'\n';
                        myfile << "Sample rate  :" << myf.samplerate() << '\n';
                        myfile << "Channels     :" << myf.channels() << '\n';
                        myfile << "Frames       :" << int(myf.frames()) << "\n\n";
                        myfile.close();

                        FILE* dataOut;
                        fopen_s(&dataOut,"./dataOut.txt", "w");

                        convert_to_text(myf, dataOut, 0);

                        fclose(dataOut);

                        /*wav::WAVData wavData(fileName);
                        wav::WAVData copy = wavData;


                        copy.WriteToFile("./test_write.wav");

                        return 0;
                        */
                    }
                   }
                
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            RECT rec;
            TextOut(hdc, 10, 30, L"sample12char", 13);
            SetTextColor(hdc, RGB(0, 255, 255));
            rec.top = 50;
            rec.left = 10;
            rec.right = 290;
            rec.bottom = 100;
            DrawText(hdc, L"�g�p�� sample\n�v���t�B�b�N�X(&A)", -1, &rec,
                DT_WORDBREAK | DT_CENTER);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;

    }


    return (INT_PTR)FALSE;
}
