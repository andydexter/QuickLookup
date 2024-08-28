[![LinkedIn](https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555)](https://www.linkedin.com/in/andreas-nicolaou-a11009164)

# QuickLookup

## Description
QuickLookup is a project that provides a fast and convinient way of searching for information from a custom data source.

A console based program, it is designed to help with and streamline clerical work or similar tasks, for example, frequently having to look up employee IDs to copy into documents, where the current system is slow and cumbersome.

You may implement the "lookup" function to search for any information you need, according to your use case.

There are also a variety of customization options available, such as changing the input type, window size, display format, and more.

Currently, the projecy is only available for Windows, but support for other operating systems is [planned](https://github.com/andydexter/QuickLookup/milestone/1) for the near future.

## Features
- Easy and intuitive interface
- Option for real-time lookup
- Variety of keyboard shortcuts for quick navigation
- Complete freedom on lookup function implementation allowing for any use case
- Designed with efficiency and optimization in mind
- Customizable settings for a personalized experience

## Installation
1. Clone the repository: `git clone https://github.com/your-username/QuickLookup.git`
2. Create a new directory for the build files: `mkdir out/build`
3. Navigate to the directory: `cd out/build`
4. Compile the project (flags are optional): `cmake ../.. -DALIGN_CENTER=ON -DGETCH_INPUT=ON && cmake --build .`
5. Run the executable `out/build/Debug/QuickLookupExample.exe` (Preferably by opening the file so you don't mess up your terminal window)

Alternatively you could open the project in Visual Studio and build it using one of the presets.

## Usage
The following instructions will guide you on using a QuickLookup application, I recommend you try out the Example Application which you can build using the instructions at [Installation](#installation). 

For instructions on how to implement the project for your own use case, refer to [Implementation](#implementation)

1. Open the QuickLookup application.
2. Enter your search query, should show up in the search bar.
3. Press Enter when you're ready to start navigating the search results.
4. Navigate through the search results using the following keybindings:
	- Move up one row: `Up Arrow` | `Left Arrow` 
	- Move down one row: `Down Arrow` | `Right Arrow` | `ENTER`
	- Go to first row: `Home` | `Page Up`
	- Go to last row: `End` | `Page Down`
	- Go to the search bar: `Ctrl + Z`
	- Copy Selected Row and Exit Program: `Ctrl + C`

### Options
You may customize the application by enabling or disabling any of the following options by adding the `-DOPTION=ON/OFF` flag when compiling the project:

- `ALIGN_CENTER` (Default `OFF`) - Aligns the search results to the center of the console window.

- `GETCH_INPUT` (Default `OFF`) - Uses the `_getch()` function for input instead of the standard input stream. This allows for refreshing search results after every character entry giving **real-time lookup**. When off, you must press enter before previewing any results. This option is very handy but may *not* work on all operating systems, so it is turned off by default. It may also not be optimal in cases where the `lookup` function is slow. It does *not* work with **Unicode** characters.

## Implementation
To implement the QuickLookup project for your own use case, you must `#include "QuickLookup.h"` in your project and define the `lookup` function. 

The `lookup` function is the function that will be called to search for data. It should take a `std::string` (the search query) as an argument and set the `rows` vector to the search results.
You may implement this in any way you like, for example, by searching a local or remote database, a file, or an array.
Each `Row` in the `rows` vector contains a `string display`. This is the string that will be displayed in the console. This can be anything you like e.g the key, value, or any combination of the 2.
Each `Row` also contains a `string value` which is the string that will be copied to the clipboard when the user presses `Ctrl + C` while this row is selected.

Lastly, you must instantiate a `QL::QuickLookup` object and call the `QuickLookup::run(int windowWidth, int windowHeight, int windowMargin)` function to start the application with the following settings:
- `Window Width` defines the **width** of the application's console window measured in characters.
- `Window Height` defines the **height** of the application's console window measured in characters.
- `Window Margin` defines the **left and right** margin of the application's console window measured in characters.

Running `QuickLookup::run(40, 10, 1)` will generate a terminal of size `40 x 10`, with `1` empty column on either side, and will be showing up to `6` search results.

For ***Unicode support***, you should check out [this](https://stackoverflow.com/questions/78894522/inconsistent-format-of-utf-8-characters-in-c/) question's answer to configure the console to accept Unicode Characters. Do note that you currently can't copy a Unicode string to clipboard, though there are [plans](https://github.com/andydexter/QuickLookup/issues/2) for it.

## Contributing
Contributions are greatly appreciated! Feel free to share any questions or ideas in the **Discussions** tab, create an **Issue** for any bugs or feature requests, or:
1. Fork the repository.
2. Create a new branch: `git checkout -b feature/your-feature`
3. Make your changes and commit them: `git commit -m 'Add some feature'`
4. Push to the branch: `git push origin feature/your-feature`
5. Submit a pull request.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more information.
