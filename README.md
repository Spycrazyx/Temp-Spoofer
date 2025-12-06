
# Temp-Spoofer

A web-based UI (HTML/JS) powered by a C++ backend for temporary hardware ID (HWID) spoofing. **Driver is not included** – this project only contains the user-mode components and interface logic.

## ⭐ Overview

This tool was originally built as a personal experiment to understand how Windows interacts with hardware identifiers through kernel-level drivers. The spoofer temporarily modifies various HWID-related values for testing, debugging, and possibly other (unintended) purposes.  Use this at your own risk.

## Key Features & Benefits

*   **Temporary HWID Spoofing:** Modifies system-reported serials and identifiers without persistent changes (reboot reverts).
*   **Web-Based UI:** User-friendly interface built with HTML/JS for easy interaction.
*   **C++ Backend:** Efficient and performant backend for managing spoofing operations.
*   **Modular Design:** Clear separation of concerns between the UI and core spoofing logic.
*   **Lightweight:** Minimal overhead, ensuring minimal performance impact.

## Prerequisites & Dependencies

Before you begin, ensure you have met the following requirements:

*   **Operating System:** Windows
*   **Compiler:** Visual Studio with C++ support
*   **Dependencies:**
    *   `httplib.h`
    *   A pre-built `library_x64.lib` (assumed to be provided)
    *   Any modern web browser.

## Installation & Setup Instructions

1.  **Open the Solution:**
    Open the `Temp Woofer.sln` file in Visual Studio.

2.  **Build the Project:**
    Build the "Temp Woofer" project in Visual Studio. Make sure to configure the build for x64 architecture.

3.  **Locate the Executable:**
    The built executable will be located in the `x64/Debug` or `x64/Release` directory (depending on your build configuration).

4.  **UI Components:**
    The web-based UI files (HTML, JavaScript) are embedded within the C++ backend.  No separate UI setup is required.

## Usage Examples

1.  **Run the Executable:**
    Execute the `Temp Woofer.exe` file. This will start the backend server.

2.  **Open the UI in your Browser:**
    The application contains embedded HTML and JavaScript to display the WebUI, please refer to the code for the expected usage.

## Configuration Options

*   Currently, configuration options are set within the C++ source code. Modifications to the following can be made by altering the project source.
    *   HWID values to be spoofed.
    *   Backend server port.
    *   Spoofing behavior

## License Information

This project is licensed under the [Mozilla Public License 2.0](LICENSE). See the `LICENSE` file for details.

## Acknowledgments

*   `httplib.h`:  A single-file header-only C++ HTTP/HTTPS server library.
