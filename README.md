# Non-Blocking HTTP Server in C++98 🌐

```[ Project Completion Date : 2022 - 02 - 10 ]```

## Project Overview 📋

webserv is a lightweight HTTP server built from scratch in C++98. It prioritizes efficiency and clean code, adhering to the SOLID principles for ultimate maintainability.

## Key Features 🔧

- **Non-Blocking I/O (⚡):** Handles multiple client connections concurrently using `select()` (or equivalent) for maximum server throughput. This means your server can handle a ton of requests at once, keeping things blazing fast!
- **Proxy Design Pattern (🔄):** Encapsulates network communication logic promoting code reusability and making testing a breeze. Think of it as modular magic, making your code easy to understand and maintain. ✨
- **Configuration Parsing (⚙️):** Reads server configuration from external files, allowing for flexible management with minimal effort. No more hardcoding configurations, just pure flexibility!
- **Static File Serving (📁):** Efficiently delivers static content (HTML, CSS, images) based on client requests. This means lightning-fast delivery of those essential website elements.
- **Dynamic Request Handling (🔄):** Supports common HTTP methods (GET, POST, DELETE) for handling dynamic interactions. This makes your server truly versatile, able to handle a wide range of requests.
- **Error Handling (❌):** Provides user-friendly error messages for invalid requests or server issues. No more cryptic errors, just clear and informative messages for easy troubleshooting.
- **SOLID Principles (🔨):** Adheres to Single Responsibility, Open-Closed, Liskov Substitution, Interface Segregation, and Dependency Inversion principles for a well-structured and maintainable codebase. Think of it as the foundation for a code fortress, built to be strong and reliable.
- **CGI Integration (🔌):** Enables execution of external CGI scripts for server-side processing.

## Project Conclusion 🎓

Building webserv has been a rewarding experience, deepening my understanding of:

- Low-level network programming 
- System resource management
- Adherence to HTTP protocols
- Applying design patterns and SOLID principles for clean, maintainable code

This project demonstrates my ability to code efficiently while keeping maintainability as a top priority. I believe in building code that not only works well but is also easy to understand and improve upon in the future.

## Contributers 🔨
* * Youness Chaaibi      -- <a href="https://github.com/ychaaibi">ychaaibi</a>
* * Youssef Alaoui Sossi -- <a href="https://github.com/yoalaoui">yoaloui </a>
