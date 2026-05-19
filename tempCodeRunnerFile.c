#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

// Function to get current time as string
void getCurrentTime(char *buffer, int size) {
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", timeinfo);
}

// Function to generate random number
int getRandomNumber() {
    return rand() % 100 + 1;
}

// Function to serve different pages
void servePage(SOCKET client, const char *path) {
    char response[4096];
    char currentTime[64];
    getCurrentTime(currentTime, sizeof(currentTime));
    
    if (strcmp(path, "/") == 0 || strcmp(path, "/index.html") == 0) {
        sprintf(response,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n\r\n"
            "<!DOCTYPE html>"
            "<html>"
            "<head>"
            "<title>My C Web Server</title>"
            "<style>"
            "body { font-family: Arial, sans-serif; margin: 0; padding: 0; background: linear-gradient(135deg, #667eea 0%%, #764ba2 100%%); min-height: 100vh; }"
            ".container { max-width: 800px; margin: 50px auto; background: white; padding: 30px; border-radius: 15px; box-shadow: 0 10px 30px rgba(0,0,0,0.2); }"
            "h1 { color: #667eea; text-align: center; }"
            ".nav { background: #333; overflow: hidden; border-radius: 10px; margin-bottom: 20px; }"
            ".nav a { float: left; display: block; color: white; text-align: center; padding: 14px 20px; text-decoration: none; }"
            ".nav a:hover { background: #667eea; }"
            ".btn { display: inline-block; background: #667eea; color: white; padding: 10px 20px; text-decoration: none; border-radius: 5px; margin: 5px; }"
            ".btn:hover { background: #764ba2; }"
            ".status { background: #f0f0f0; padding: 10px; border-radius: 5px; margin: 10px 0; }"
            "footer { text-align: center; margin-top: 30px; color: #666; }"
            "</style>"
            "</head>"
            "<body>"
            "<div class='container'>"
            "<h1>🚀 Welcome to My C Web Server</h1>"
            "<div class='nav'>"
            "<a href='/'>Home</a>"
            "<a href='/about'>About</a>"
            "<a href='/game'>Number Game</a>"
            "<a href='/time'>Server Time</a>"
            "<a href='/stats'>Stats</a>"
            "</div>"
            "<div class='status'>"
            "<strong>Server Status:</strong> Online ✅<br>"
            "<strong>Current Time:</strong> %s<br>"
            "<strong>Server Port:</strong> 8080<br>"
            "<strong>Powered by:</strong> C Language with Winsock2"
            "</div>"
            "<h2>Features:</h2>"
            "<ul>"
            "<li>Dynamic HTML generation</li>"
            "<li>Multiple page routing</li>"
            "<li>Number guessing game</li>"
            "<li>Real-time server statistics</li>"
            "<li>Beautiful responsive design</li>"
            "</ul>"
            "<center><a href='/game' class='btn'>🎮 Play Number Game</a> "
            "<a href='/time' class='btn'>🕐 Check Server Time</a></center>"
            "<footer>Created with ❤️ using C language | Server running on Windows</footer>"
            "</div>"
            "</body>"
            "</html>", currentTime);
            
    } else if (strcmp(path, "/about") == 0) {
        sprintf(response,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n\r\n"
            "<!DOCTYPE html>"
            "<html>"
            "<head><title>About - C Web Server</title>"
            "<style>"
            "body { font-family: Arial, sans-serif; background: linear-gradient(135deg, #667eea 0%%, #764ba2 100%%); min-height: 100vh; }"
            ".container { max-width: 800px; margin: 50px auto; background: white; padding: 30px; border-radius: 15px; }"
            "h1 { color: #667eea; }"
            ".nav { background: #333; overflow: hidden; border-radius: 10px; margin-bottom: 20px; }"
            ".nav a { float: left; display: block; color: white; text-align: center; padding: 14px 20px; text-decoration: none; }"
            ".nav a:hover { background: #667eea; }"
            "</style>"
            "</head>"
            "<body>"
            "<div class='container'>"
            "<div class='nav'>"
            "<a href='/'>Home</a>"
            "<a href='/about'>About</a>"
            "<a href='/game'>Game</a>"
            "<a href='/time'>Time</a>"
            "</div>"
            "<h1>📖 About This Web Server</h1>"
            "<p>This web server is written entirely in C programming language using Winsock2 library.</p>"
            "<h2>Technical Details:</h2>"
            "<ul>"
            "<li>Language: C</li>"
            "<li>Library: Winsock2 (Windows Sockets)</li>"
            "<li>Protocol: HTTP/1.1</li>"
            "<li>Port: 8080</li>"
            "<li>Features: Dynamic content generation, URL routing</li>"
            "</ul>"
            "<h2>Developer:</h2>"
            "<p>Created by Zakir - A demonstration of C's power in web development!</p>"
            "<a href='/' class='btn'>Back to Home</a>"
            "</div>"
            "</body>"
            "</html>");
            
    } else if (strcmp(path, "/time") == 0) {
        sprintf(response,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n\r\n"
            "<!DOCTYPE html>"
            "<html>"
            "<head><title>Server Time</title>"
            "<style>"
            "body { font-family: Arial, sans-serif; background: linear-gradient(135deg, #667eea 0%%, #764ba2 100%%); }"
            ".container { max-width: 600px; margin: 100px auto; background: white; padding: 30px; border-radius: 15px; text-align: center; }"
            ".time { font-size: 48px; color: #667eea; font-weight: bold; margin: 20px 0; }"
            ".btn { background: #667eea; color: white; padding: 10px 20px; text-decoration: none; border-radius: 5px; display: inline-block; }"
            "</style>"
            "</head>"
            "<body>"
            "<div class='container'>"
            "<h1>🕐 Current Server Time</h1>"
            "<div class='time'>%s</div>"
            "<p>This time is generated dynamically by the C server!</p>"
            "<a href='/' class='btn'>Back to Home</a>"
            "<script>setTimeout(function(){ location.reload(); }, 1000);</script>"
            "</div>"
            "</body>"
            "</html>", currentTime);
            
    } else if (strcmp(path, "/game") == 0) {
        sprintf(response,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n\r\n"
            "<!DOCTYPE html>"
            "<html>"
            "<head><title>Number Guessing Game</title>"
            "<style>"
            "body { font-family: Arial, sans-serif; background: linear-gradient(135deg, #667eea 0%%, #764ba2 100%%); }"
            ".container { max-width: 500px; margin: 50px auto; background: white; padding: 30px; border-radius: 15px; }"
            "input { padding: 10px; margin: 10px; width: 200px; border: 2px solid #667eea; border-radius: 5px; }"
            "button { background: #667eea; color: white; padding: 10px 20px; border: none; border-radius: 5px; cursor: pointer; }"
            "button:hover { background: #764ba2; }"
            ".message { margin: 20px 0; padding: 10px; border-radius: 5px; }"
            "</style>"
            "</head>"
            "<body>"
            "<div class='container'>"
            "<h1>🎮 Number Guessing Game</h1>"
            "<p>Guess a number between 1 and 100!</p>"
            "<input type='number' id='guess' placeholder='Enter your guess'>"
            "<button onclick='makeGuess()'>Guess!</button>"
            "<div id='message' class='message'></div>"
            "<div id='attempts'></div>"
            "<a href='/' class='btn'>Back to Home</a>"
            "<script>"
            "let secretNumber = Math.floor(Math.random() * 100) + 1;"
            "let attempts = 0;"
            "function makeGuess() {"
            "    let guess = document.getElementById('guess').value;"
            "    let message = document.getElementById('message');"
            "    if(!guess) { message.innerHTML = '❌ Please enter a number!'; return; }"
            "    attempts++;"
            "    if(guess == secretNumber) {"
            "        message.innerHTML = '🎉 Congratulations! You guessed it in ' + attempts + ' attempts! 🎉';"
            "        message.style.background = '#d4edda';"
            "        secretNumber = Math.floor(Math.random() * 100) + 1;"
            "        attempts = 0;"
            "        document.getElementById('attempts').innerHTML = '';"
            "    } else if(guess < secretNumber) {"
            "        message.innerHTML = '📉 Too low! Try a higher number.';"
            "        message.style.background = '#fff3cd';"
            "        document.getElementById('attempts').innerHTML = 'Attempts: ' + attempts;"
            "    } else {"
            "        message.innerHTML = '📈 Too high! Try a lower number.';"
            "        message.style.background = '#fff3cd';"
            "        document.getElementById('attempts').innerHTML = 'Attempts: ' + attempts;"
            "    }"
            "    document.getElementById('guess').value = '';"
            "}"
            "</script>"
            "</div>"
            "</body>"
            "</html>");
            
    } else if (strcmp(path, "/stats") == 0) {
        sprintf(response,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n\r\n"
            "<!DOCTYPE html>"
            "<html>"
            "<head><title>Server Statistics</title>"
            "<style>"
            "body { font-family: Arial, sans-serif; background: linear-gradient(135deg, #667eea 0%%, #764ba2 100%%); }"
            ".container { max-width: 600px; margin: 50px auto; background: white; padding: 30px; border-radius: 15px; }"
            ".stat-box { background: #f0f0f0; padding: 15px; margin: 10px 0; border-radius: 5px; }"
            "</style>"
            "</head>"
            "<body>"
            "<div class='container'>"
            "<h1>📊 Server Statistics</h1>"
            "<div class='stat-box'>"
            "<strong>Server Uptime:</strong> Since startup<br>"
            "<strong>Protocol:</strong> HTTP/1.1<br>"
            "<strong>Max Connections:</strong> 3<br>"
            "<strong>Platform:</strong> Windows<br>"
            "<strong>Current Time:</strong> %s"
            "</div>"
            "<a href='/' class='btn'>Back to Home</a>"
            "</div>"
            "</body>"
            "</html>", currentTime);
    } else {
        // 404 Page
        sprintf(response,
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/html\r\n\r\n"
            "<!DOCTYPE html>"
            "<html>"
            "<head><title>404 - Page Not Found</title>"
            "<style>"
            "body { font-family: Arial, sans-serif; text-align: center; padding: 50px; }"
            "h1 { color: #667eea; font-size: 72px; }"
            "</style>"
            "</head>"
            "<body>"
            "<h1>404</h1>"
            "<h2>Page Not Found 😢</h2>"
            "<p>The page you're looking for doesn't exist.</p>"
            "<a href='/'>Go to Homepage</a>"
            "</body>"
            "</html>");
    }
    
    send(client_socket, response, strlen(response), 0);
}

int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    int client_len;
    char buffer[1024];
    
    // Seed random number generator
    srand(time(NULL));
    
    printf("========================================\n");
    printf("   C Web Server Starting...\n");
    printf("========================================\n");
    
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Failed to initialize Winsock. Error Code: %d\n", WSAGetLastError());
        return 1;
    }
    
    printf("Winsock initialized successfully.\n");
    
    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        printf("Failed to create socket. Error Code: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    
    printf("Socket created successfully.\n");
    
    // Setup server structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);
    
    // Bind socket
    if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed. Error Code: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    
    printf("Socket bound to port 8080.\n");
    
    // Listen for connections
    if (listen(server_socket, 5) == SOCKET_ERROR) {
        printf("Listen failed. Error Code: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    
    printf("========================================\n");
    printf("✓ Server running on http://localhost:8080\n");
    printf("✓ Ready to accept connections...\n");
    printf("✓ Press Ctrl+C to stop the server\n");
    printf("========================================\n\n");
    
    // Main server loop
    int requestCount = 0;
    while(1) {
        client_len = sizeof(struct sockaddr_in);
        client_socket = accept(server_socket, (struct sockaddr *)&client, &client_len);
        
        if (client_socket == INVALID_SOCKET) {
            printf("Accept failed. Error Code: %d\n", WSAGetLastError());
            continue;
        }
        
        requestCount++;
        printf("[Request #%d] New connection from %s:%d\n", 
               requestCount, 
               inet_ntoa(client.sin_addr), 
               ntohs(client.sin_port));
        
        // Receive request
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        
        if (bytes_received > 0) {
            // Parse HTTP request to get path
            char method[10], path[256], version[10];
            sscanf(buffer, "%s %s %s", method, path, version);
            
            printf("   Request: %s %s\n", method, path);
            
            // Serve the requested page
            servePage(client_socket, path);
        }
        
        // Close client connection
        closesocket(client_socket);
        printf("   Connection closed.\n\n");
    }
    
    // Cleanup (never reached in this loop, but good practice)
    closesocket(server_socket);
    WSACleanup();
    
    return 0;
}