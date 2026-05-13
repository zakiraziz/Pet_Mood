#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <time.h>
#include <ctype.h>

#pragma comment(lib, "ws2_32.lib")

void log_request(const char *client_ip, const char *request) {
    time_t rawtime;
    struct tm *timeinfo;
    char timestamp[64];
    
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(timestamp, sizeof(timestamp), "[%Y-%m-%d %H:%M:%S]", timeinfo);
    
    printf("%s Client: %s - Request: %s\n", timestamp, client_ip, request);
}

void serve_file(SOCKET client_socket, const char *filename) {
    FILE *file = fopen(filename, "r");
    char buffer[1024];
    char response[4096] = "";
    char content[2048] = "";
    
    if (file == NULL) {
        // 404 Not Found
        snprintf(response, sizeof(response),
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/html\r\n\r\n"
            "<html><body><h1>404 - File Not Found</h1>"
            "<p>The requested file '%s' does not exist.</p>"
            "<a href='/'>Go back to homepage</a></body></html>",
            filename);
        send(client_socket, response, strlen(response), 0);
        return;
    }
    
    // Read file content
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        strcat(content, buffer);
    }
    fclose(file);
    
    // Send response with file content
    snprintf(response, sizeof(response),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n\r\n"
        "%s", content);
    send(client_socket, response, strlen(response), 0);
}

void serve_dynamic_page(SOCKET client_socket) {
    time_t rawtime;
    struct tm *timeinfo;
    char time_str[64];
    
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    char *response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n\r\n"
        "<!DOCTYPE html>"
        "<html>"
        "<head>"
        "<title>My C Web Server</title>"
        "<style>"
        "body { font-family: Arial, sans-serif; margin: 40px; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); }"
        ".container { max-width: 800px; margin: auto; background: white; padding: 30px; border-radius: 10px; box-shadow: 0 0 20px rgba(0,0,0,0.1); }"
        "h1 { color: #333; }"
        ".info { background: #f4f4f4; padding: 15px; border-radius: 5px; margin: 20px 0; }"
        ".status { color: #28a745; font-weight: bold; }"
        "button { background: #667eea; color: white; border: none; padding: 10px 20px; border-radius: 5px; cursor: pointer; }"
        "button:hover { background: #764ba2; }"
        "</style>"
        "<script>"
        "function showAlert() { alert('Hello from C server!'); }"
        "function fetchTime() {"
        "   fetch('/time')"
        "       .then(response => response.text())"
        "       .then(data => document.getElementById('timeData').innerHTML = data);"
        "}"
        "</script>"
        "</head>"
        "<body>"
        "<div class='container'>"
        "<h1>🚀 Welcome to Zakir's C Web Server!</h1>"
        "<p>This website is powered by <strong>C language</strong> and <strong>Winsock2</strong></p>"
        "<div class='info'>"
        "<p><strong>Server Status:</strong> <span class='status'>✅ Running</span></p>"
        "<p><strong>Server Time:</strong> %s</p>"
        "<p><strong>Port:</strong> 8080</p>"
        "<p><strong>Protocol:</strong> HTTP/1.1</p>"
        "</div>"
        "<h2>Features:</h2>"
        "<ul>"
        "<li>HTTP Request Handling</li>"
        "<li>Multiple Endpoints (/, /about, /time, /api)</li>"
        "<li>File Serving Capability</li>"
        "<li>Request Logging</li>"
        "<li>Dynamic Content Generation</li>"
        "<li>AJAX Support</li>"
        "</ul>"
        "<button onclick='showAlert()'>Click Me!</button>"
        "<button onclick='fetchTime()' style='margin-left: 10px;'>Get Server Time</button>"
        "<div id='timeData' style='margin-top: 20px; padding: 10px; background: #e9ecef; border-radius: 5px;'></div>"
        "<hr>"
        "<p><a href='/about'>About Page</a> | <a href='/api'>API Endpoint</a> | <a href='/time'>Server Time (JSON)</a></p>"
        "</div>"
        "</body>"
        "</html>";
    
    char full_response[4096];
    snprintf(full_response, sizeof(full_response), response, time_str);
    send(client_socket, full_response, strlen(full_response), 0);
}

void serve_about_page(SOCKET client_socket) {
    char *response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n\r\n"
        "<!DOCTYPE html>"
        "<html>"
        "<head><title>About - C Web Server</title>"
        "<style>"
        "body { font-family: Arial, sans-serif; margin: 40px; background: #f0f0f0; }"
        ".container { max-width: 800px; margin: auto; background: white; padding: 30px; border-radius: 10px; }"
        "code { background: #f4f4f4; padding: 2px 5px; border-radius: 3px; }"
        "</style>"
        "</head>"
        "<body>"
        "<div class='container'>"
        "<h1>📖 About This Server</h1>"
        "<p>This is a lightweight HTTP server written entirely in <strong>C language</strong> using Windows Winsock2 library.</p>"
        "<h2>Technical Details:</h2>"
        "<ul>"
        "<li>Language: C</li>"
        "<li>Socket Library: Winsock2 (Windows)</li>"
        "<li>HTTP Version: 1.1</li>"
        "<li>Port: 8080</li>"
        "<li>Concurrent Connections: Single-threaded (sequential)</li>"
        "</ul>"
        "<h2>How it works:</h2>"
        "<p>The server listens for incoming TCP connections on port 8080, parses HTTP requests, "
        "and serves appropriate responses based on the requested path.</p>"
        "<p><a href='/'>← Back to Home</a></p>"
        "</div>"
        "</body>"
        "</html>";
    send(client_socket, response, strlen(response), 0);
}

void serve_api_endpoint(SOCKET client_socket) {
    time_t rawtime;
    struct tm *timeinfo;
    char time_str[64];
    
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    char *response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n\r\n"
        "{\n"
        "  \"status\": \"success\",\n"
        "  \"server\": \"C Web Server\",\n"
        "  \"version\": \"1.0\",\n"
        "  \"message\": \"Hello from C API!\"\n"
        "}";
    
    send(client_socket, response, strlen(response), 0);
}

void serve_time_json(SOCKET client_socket) {
    time_t rawtime;
    struct tm *timeinfo;
    char time_str[64];
    
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    char response[512];
    snprintf(response, sizeof(response),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n\r\n"
        "{\n"
        "  \"server_time\": \"%s\",\n"
        "  \"timestamp\": %ld\n"
        "}",
        time_str, (long)rawtime);
    
    send(client_socket, response, strlen(response), 0);
}

int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    int c, bytes_received;
    char client_request[4096];
    char client_ip[16];
    
    printf("🔄 Initializing Winsock...\n");
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("❌ Failed to initialize Winsock. Error code: %d\n", WSAGetLastError());
        return 1;
    }
    
    printf("✅ Winsock initialized successfully.\n");
    
    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        printf("❌ Failed to create socket. Error code: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    
    // Configure server address
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);
    
    // Bind socket
    if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("❌ Bind failed. Error code: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    
    // Listen for connections
    if (listen(server_socket, 10) == SOCKET_ERROR) {
        printf("❌ Listen failed. Error code: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    
    printf("🚀 Server running on http://localhost:8080\n");
    printf("📡 Press Ctrl+C to stop the server\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    
    c = sizeof(struct sockaddr_in);
    
    // Main server loop
    while (1) {
        // Accept client connection
        client_socket = accept(server_socket, (struct sockaddr *)&client, &c);
        if (client_socket == INVALID_SOCKET) {
            printf("❌ Accept failed. Error code: %d\n", WSAGetLastError());
            continue;
        }
        
        // Get client IP address
        inet_ntop(AF_INET, &client.sin_addr, client_ip, sizeof(client_ip));
        
        // Receive client request
        bytes_received = recv(client_socket, client_request, sizeof(client_request) - 1, 0);
        if (bytes_received > 0) {
            client_request[bytes_received] = '\0';
            
            // Parse HTTP method and path
            char method[16] = "";
            char path[256] = "";
            sscanf(client_request, "%s %s", method, path);
            
            // Log the request
            log_request(client_ip, path);
            
            // Route the request based on path
            if (strcmp(path, "/") == 0) {
                serve_dynamic_page(client_socket);
            } else if (strcmp(path, "/about") == 0) {
                serve_about_page(client_socket);
            } else if (strcmp(path, "/api") == 0) {
                serve_api_endpoint(client_socket);
            } else if (strcmp(path, "/time") == 0) {
                serve_time_json(client_socket);
            } else if (strncmp(path, "/file/", 6) == 0) {
                // Serve a specific file (e.g., /file/index.html)
                serve_file(client_socket, path + 6);
            } else {
                // 404 Not Found
                char *response =
                    "HTTP/1.1 404 Not Found\r\n"
                    "Content-Type: text/html\r\n\r\n"
                    "<html><body><h1>404 - Page Not Found</h1>"
                    "<p>The requested URL was not found on this server.</p>"
                    "<a href='/'>Go back to homepage</a></body></html>";
                send(client_socket, response, strlen(response), 0);
            }
        }
        
        // Close client connection
        closesocket(client_socket);
    }
    
    // Cleanup (this part won't be reached in the current loop)
    closesocket(server_socket);
    WSACleanup();
    
    return 0;
}