#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <time.h>
#include <dirent.h>

#define PORT 8080
#define BUFFER_SIZE 4096
#define MAX_CLIENTS 10
#define WEB_ROOT "./www"

// MIME types
typedef struct {
    char* extension;
    char* mime_type;
} MimeType;

MimeType mime_types[] = {
    {".html", "text/html"},
    {".htm", "text/html"},
    {".css", "text/css"},
    {".js", "application/javascript"},
    {".json", "application/json"},
    {".png", "image/png"},
    {".jpg", "image/jpeg"},
    {".jpeg", "image/jpeg"},
    {".gif", "image/gif"},
    {".svg", "image/svg+xml"},
    {".ico", "image/x-icon"},
    {".txt", "text/plain"},
    {".pdf", "application/pdf"},
    {".zip", "application/zip"},
    {NULL, NULL}
};

// Function prototypes
void* handle_client(void* client_socket);
char* get_mime_type(const char* path);
void send_file(int client_socket, const char* path);
void send_404(int client_socket);
void send_response(int client_socket, int status_code, const char* status_text, 
                   const char* content_type, const char* body, size_t body_len);
void log_request(const char* method, const char* path, int status_code);
void create_default_page(void);
void serve_directory(int client_socket, const char* path);
char* url_decode(const char* url);

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    pthread_t thread_id;
    
    // Create default web page if it doesn't exist
    create_default_page();
    
    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    
    // Allow socket reuse
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Setsockopt failed");
        exit(1);
    }
    
    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    // Bind socket
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }
    
    // Listen for connections
    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        exit(1);
    }
    
    printf("\n========================================\n");
    printf("  🌐 HTTP Web Server Started\n");
    printf("  📡 Port: %d\n", PORT);
    printf("  📁 Web Root: %s\n", WEB_ROOT);
    printf("  🔗 URL: http://localhost:%d\n", PORT);
    printf("========================================\n");
    printf("Press Ctrl+C to stop the server\n\n");
    
    // Accept and handle clients
    while (1) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }
        
        // Get client IP
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        printf("📥 New connection from %s\n", client_ip);
        
        // Handle client in new thread
        int* new_sock = malloc(sizeof(int));
        *new_sock = client_socket;
        pthread_create(&thread_id, NULL, handle_client, new_sock);
        pthread_detach(thread_id);
    }
    
    close(server_socket);
    return 0;
}

void* handle_client(void* arg) {
    int client_socket = *(int*)arg;
    free(arg);
    
    char buffer[BUFFER_SIZE];
    char method[16], path[256], version[16];
    
    // Read request
    ssize_t bytes_read = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_read <= 0) {
        close(client_socket);
        return NULL;
    }
    buffer[bytes_read] = '\0';
    
    // Parse request line (GET /index.html HTTP/1.1)
    if (sscanf(buffer, "%s %s %s", method, path, version) != 3) {
        send_404(client_socket);
        close(client_socket);
        return NULL;
    }
    
    // Decode URL
    char* decoded_path = url_decode(path);
    if (decoded_path) {
        strcpy(path, decoded_path);
        free(decoded_path);
    }
    
    // Remove query parameters
    char* query = strchr(path, '?');
    if (query) *query = '\0';
    
    // Build full path
    char full_path[512];
    if (strcmp(path, "/") == 0) {
        strcpy(full_path, WEB_ROOT "/index.html");
    } else {
        snprintf(full_path, sizeof(full_path), "%s%s", WEB_ROOT, path);
    }
    
    // Log request
    printf("  📝 %s %s", method, path);
    
    // Check if file exists
    struct stat st;
    if (stat(full_path, &st) == 0) {
        // Check if it's a directory
        if (S_ISDIR(st.st_mode)) {
            serve_directory(client_socket, full_path);
            log_request(method, path, 200);
        } else {
            send_file(client_socket, full_path);
            log_request(method, path, 200);
        }
    } else {
        send_404(client_socket);
        log_request(method, path, 404);
    }
    
    close(client_socket);
    return NULL;
}

void send_file(int client_socket, const char* path) {
    int file_fd = open(path, O_RDONLY);
    if (file_fd < 0) {
        send_404(client_socket);
        return;
    }
    
    // Get file size
    struct stat st;
    fstat(file_fd, &st);
    
    // Get MIME type
    char* mime_type = get_mime_type(path);
    
    // Send headers
    char headers[256];
    snprintf(headers, sizeof(headers),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %ld\r\n"
             "Server: Mini-C-Server\r\n"
             "Connection: close\r\n"
             "\r\n",
             mime_type, st.st_size);
    
    send(client_socket, headers, strlen(headers), 0);
    
    // Send file
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = read(file_fd, buffer, BUFFER_SIZE)) > 0) {
        send(client_socket, buffer, bytes_read, 0);
    }
    
    close(file_fd);
}

void send_404(int client_socket) {
    char* body = 
        "<!DOCTYPE html>"
        "<html>"
        "<head><title>404 Not Found</title>"
        "<style>body{font-family:Arial;text-align:center;padding:50px;}"
        "h1{color:#e74c3c;font-size:72px;margin:0;}"
        "p{color:#666;font-size:24px;}"
        ".emoji{font-size:100px;}</style>"
        "</head>"
        "<body>"
        "<div class='emoji'>🔍</div>"
        "<h1>404</h1>"
        "<p>Page Not Found</p>"
        "<p><a href='/'>Go Home</a></p>"
        "</body>"
        "</html>";
    
    char response[1024];
    snprintf(response, sizeof(response),
             "HTTP/1.1 404 Not Found\r\n"
             "Content-Type: text/html\r\n"
             "Content-Length: %zu\r\n"
             "Connection: close\r\n"
             "\r\n"
             "%s",
             strlen(body), body);
    
    send(client_socket, response, strlen(response), 0);
}

void serve_directory(int client_socket, const char* dir_path) {
    DIR* dir = opendir(dir_path);
    if (!dir) {
        send_404(client_socket);
        return;
    }
    
    char* html = malloc(4096);
    char* ptr = html;
    ptr += sprintf(ptr, 
        "<!DOCTYPE html>"
        "<html>"
        "<head><title>Directory Listing</title>"
        "<style>"
        "body{font-family:'Courier New',monospace;padding:20px;background:#f5f5f5;}"
        ".container{max-width:800px;margin:0 auto;background:white;padding:30px;border-radius:10px;box-shadow:0 2px 10px rgba(0,0,0,0.1);}"
        "h1{color:#333;border-bottom:3px solid #3498db;padding-bottom:10px;}"
        "ul{list-style:none;padding:0;}"
        "li{padding:8px 0;border-bottom:1px solid #eee;}"
        "a{color:#3498db;text-decoration:none;font-size:16px;}"
        "a:hover{text-decoration:underline;}"
        ".dir{color:#e67e22;font-weight:bold;}"
        ".file{color:#2ecc71;}"
        "</style>"
        "</head>"
        "<body>"
        "<div class='container'>"
        "<h1>📁 Directory Listing</h1>"
        "<ul>");
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') continue; // Skip hidden files
        
        char full_path[512];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);
        
        struct stat st;
        stat(full_path, &st);
        
        char icon[10];
        char class[10];
        if (S_ISDIR(st.st_mode)) {
            strcpy(icon, "📂");
            strcpy(class, "dir");
        } else {
            strcpy(icon, "📄");
            strcpy(class, "file");
        }
        
        // Build URL path
        char url_path[512];
        char* base = strstr(dir_path, WEB_ROOT);
        if (base) {
            base += strlen(WEB_ROOT);
            snprintf(url_path, sizeof(url_path), "%s/%s", base, entry->d_name);
        } else {
            snprintf(url_path, sizeof(url_path), "/%s", entry->d_name);
        }
        
        // Clean up URL
        char* clean = url_path;
        while (*clean == '/') clean++;
        
        ptr += sprintf(ptr, 
            "<li><span class='%s'>%s</span> <a href='/%s'>%s</a></li>\n",
            class, icon, clean, entry->d_name);
    }
    closedir(dir);
    
    ptr += sprintf(ptr,
        "</ul>"
        "<hr>"
        "<p><small>Mini HTTP Server v1.0</small></p>"
        "</div>"
        "</body>"
        "</html>");
    
    send_response(client_socket, 200, "OK", "text/html", html, ptr - html);
    free(html);
}

void send_response(int client_socket, int status_code, const char* status_text,
                   const char* content_type, const char* body, size_t body_len) {
    char response[4096];
    snprintf(response, sizeof(response),
             "HTTP/1.1 %d %s\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %zu\r\n"
             "Server: Mini-C-Server\r\n"
             "Connection: close\r\n"
             "\r\n",
             status_code, status_text, content_type, body_len);
    
    send(client_socket, response, strlen(response), 0);
    send(client_socket, body, body_len, 0);
}

char* get_mime_type(const char* path) {
    // Find last dot
    const char* ext = strrchr(path, '.');
    if (!ext) return "application/octet-stream";
    
    for (int i = 0; mime_types[i].extension; i++) {
        if (strcmp(ext, mime_types[i].extension) == 0) {
            return mime_types[i].mime_type;
        }
    }
    return "application/octet-stream";
}

char* url_decode(const char* url) {
    char* decoded = malloc(strlen(url) + 1);
    char* ptr = decoded;
    
    while (*url) {
        if (*url == '%') {
            // Hex conversion
            char hex[3];
            hex[0] = url[1];
            hex[1] = url[2];
            hex[2] = '\0';
            *ptr++ = (char)strtol(hex, NULL, 16);
            url += 3;
        } else if (*url == '+') {
            *ptr++ = ' ';
            url++;
        } else {
            *ptr++ = *url++;
        }
    }
    *ptr = '\0';
    return decoded;
}

void log_request(const char* method, const char* path, int status_code) {
    time_t now;
    time(&now);
    char* time_str = ctime(&now);
    time_str[strlen(time_str) - 1] = '\0';
    
    printf(" -> %d %s [%s]\n", status_code, method, time_str);
}

void create_default_page() {
    // Create www directory
    mkdir(WEB_ROOT, 0755);
    
    char index_path[256];
    snprintf(index_path, sizeof(index_path), "%s/index.html", WEB_ROOT);
    
    FILE* file = fopen(index_path, "r");
    if (file) {
        fclose(file);
        return; // File already exists
    }
    
    // Create default index.html
    file = fopen(index_path, "w");
    if (!file) return;
    
    fprintf(file,
        "<!DOCTYPE html>\n"
        "<html>\n"
        "<head>\n"
        "    <meta charset='UTF-8'>\n"
        "    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n"
        "    <title>Welcome - Mini HTTP Server</title>\n"
        "    <style>\n"
        "        * { margin: 0; padding: 0; box-sizing: border-box; }\n"
        "        body {\n"
        "            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;\n"
        "            background: linear-gradient(135deg, #667eea 0%%, #764ba2 100%%);\n"
        "            min-height: 100vh;\n"
        "            display: flex;\n"
        "            justify-content: center;\n"
        "            align-items: center;\n"
        "            padding: 20px;\n"
        "        }\n"
        "        .container {\n"
        "            background: white;\n"
        "            border-radius: 20px;\n"
        "            padding: 50px;\n"
        "            max-width: 700px;\n"
        "            width: 100%%;\n"
        "            box-shadow: 0 20px 60px rgba(0,0,0,0.3);\n"
        "            text-align: center;\n"
        "        }\n"
        "        .emoji {\n"
        "            font-size: 80px;\n"
        "            margin-bottom: 20px;\n"
        "        }\n"
        "        h1 {\n"
        "            font-size: 36px;\n"
        "            color: #333;\n"
        "            margin-bottom: 10px;\n"
        "        }\n"
        "        .subtitle {\n"
        "            font-size: 18px;\n"
        "            color: #666;\n"
        "            margin-bottom: 30px;\n"
        "        }\n"
        "        .info {\n"
        "            background: #f8f9fa;\n"
        "            border-radius: 10px;\n"
        "            padding: 20px;\n"
        "            text-align: left;\n"
        "            margin: 20px 0;\n"
        "        }\n"
        "        .info-item {\n"
        "            padding: 10px 0;\n"
        "            border-bottom: 1px solid #eee;\n"
        "            display: flex;\n"
        "            justify-content: space-between;\n"
        "        }\n"
        "        .info-item:last-child { border-bottom: none; }\n"
        "        .label { color: #888; font-weight: bold; }\n"
        "        .value { color: #333; }\n"
        "        .features {\n"
        "            display: grid;\n"
        "            grid-template-columns: repeat(auto-fit, minmax(150px, 1fr));\n"
        "            gap: 15px;\n"
        "            margin: 30px 0;\n"
        "        }\n"
        "        .feature {\n"
        "            background: #f0f4ff;\n"
        "            padding: 15px;\n"
        "            border-radius: 10px;\n"
        "            font-size: 14px;\n"
        "            color: #555;\n"
        "        }\n"
        "        .feature span { font-size: 30px; display: block; margin-bottom: 5px; }\n"
        "        .footer {\n"
        "            margin-top: 20px;\n"
        "            color: #aaa;\n"
        "            font-size: 14px;\n"
        "        }\n"
        "        a { color: #667eea; text-decoration: none; }\n"
        "        a:hover { text-decoration: underline; }\n"
        "        .test-link {\n"
        "            display: inline-block;\n"
        "            background: #667eea;\n"
        "            color: white;\n"
        "            padding: 12px 30px;\n"
        "            border-radius: 30px;\n"
        "            text-decoration: none;\n"
        "            margin-top: 10px;\n"
        "            transition: background 0.3s;\n"
        "        }\n"
        "        .test-link:hover {\n"
        "            background: #5a67d8;\n"
        "            text-decoration: none;\n"
        "        }\n"
        "    </style>\n"
        "</head>\n"
        "<body>\n"
        "    <div class='container'>\n"
        "        <div class='emoji'>🚀</div>\n"
        "        <h1>Welcome to Mini HTTP Server</h1>\n"
        "        <p class='subtitle'>A lightweight web server written in C</p>\n"
        "        \n"
        "        <div class='info'>\n"
        "            <div class='info-item'>\n"
        "                <span class='label'>🕐 Server Time</span>\n"
        "                <span class='value' id='time'></span>\n"
        "            </div>\n"
        "            <div class='info-item'>\n"
        "                <span class='label'>📡 Server Status</span>\n"
        "                <span class='value' style='color:#2ecc71;'>● Running</span>\n"
        "            </div>\n"
        "            <div class='info-item'>\n"
        "                <span class='label'>📁 Serving</span>\n"
        "                <span class='value'>%s/</span>\n"
        "            </div>\n"
        "        </div>\n"
        "        \n"
        "        <div class='features'>\n"
        "            <div class='feature'>\n"
        "                <span>📄</span>\n"
        "                Static Files\n"
        "            </div>\n"
        "            <div class='feature'>\n"
        "                <span>📂</span>\n"
        "                Directory Listing\n"
        "            </div>\n"
        "            <div class='feature'>\n"
        "                <span>🔧</span>\n"
        "                Multi-threaded\n"
        "            </div>\n"
        "            <div class='feature'>\n"
        "                <span>📊</span>\n"
        "                MIME Support\n"
        "            </div>\n"
        "        </div>\n"
        "        \n"
        "        <a href='/test.html' class='test-link'>🔗 Test Page →</a>\n"
        "        <a href='/' class='test-link' style='background:#95a5a6;'>📂 Browse Files</a>\n"
        "        \n"
        "        <div class='footer'>\n"
        "            Made with ❤️ using C • <a href='https://github.com'>GitHub</a>\n"
        "        </div>\n"
        "    </div>\n"
        "    \n"
        "    <script>\n"
        "        document.getElementById('time').textContent = new Date().toLocaleString();\n"
        "    </script>\n"
        "</body>\n"
        "</html>\n",
        WEB_ROOT);
    
    fclose(file);
    
    // Create test.html
    char test_path[256];
    snprintf(test_path, sizeof(test_path), "%s/test.html", WEB_ROOT);
    file = fopen(test_path, "w");
    if (file) {
        fprintf(file,
            "<!DOCTYPE html>\n"
            "<html>\n"
            "<head><title>Test Page</title>\n"
            "<style>\n"
            "body{font-family:Arial;padding:50px;text-align:center;background:#f0f0f0;}\n"
            ".card{background:white;padding:40px;border-radius:15px;max-width:500px;margin:auto;box-shadow:0 4px 6px rgba(0,0,0,0.1);}\n"
            "h1{color:#3498db;}\n"
            ".success{color:#2ecc71;font-size:48px;}\n"
            "ul{text-align:left;padding:20px;}\n"
            "li{padding:8px 0;border-bottom:1px solid #eee;}\n"
            "</style>\n"
            "</head>\n"
            "<body>\n"
            "<div class='card'>\n"
            "<div class='success'>✅</div>\n"
            "<h1>Test Page Working!</h1>\n"
            "<p>Your C web server is successfully serving static HTML files.</p>\n"
            "<ul>\n"
            "<li>✓ MIME type detection working</li>\n"
            "<li>✓ File serving working</li>\n"
            "<li>✓ HTML parsing working</li>\n"
            "</ul>\n"
            "<p><a href='/' style='color:#3498db;text-decoration:none;'>← Back to Home</a></p>\n"
            "</div>\n"
            "</body>\n"
            "</html>\n");
        fclose(file);
    }
    
    printf("✓ Default web page created at %s/index.html\n", WEB_ROOT);
}