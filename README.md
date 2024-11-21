# Water Quality Monitor

## Overview
This application is designed to monitor and analyse water quality data, with a focus on pollutant tracking and compliance with safety standards. It was developed using **C++/Qt6** with a modular and user-friendly interface that adapts to changes in locale for basic internationalisation.

## Getting Started

### Prerequisites
- **C++17** or higher
- **Qt6** libraries, including:
    - **QtWidgets**
    - **QtCharts**
    - **QtWebView** (optional, for web content)

### Installation and Setup
1. **Extract the Starter Code**: 
 - Unzip the provided starter code package to a working directory.
2. **Build the Project**:
    - Create a `build` directory and run `cmake`:
      ```bash
      mkdir build && cd build
      cmake ..
      ```
    - Compile the project:
      ```bash
      make
      ```
    - Run the application:
      ```bash
      ./[application-executable-name]
      ```

3. **Data Setup**:
    - Place your CSV dataset in the specified directory (`/data` folder).
    - Ensure that the application has access to the data file, and adjust file paths as necessary.

## Application Structure

### Main Components
- **Dashboard**: Provides an overview of the application’s main pages and core functionality.
- **Data Page**: Displays pollutant data in a table format, allowing us to browse relevant fields. Utilises the model/view architecture from the starter code.
- **Analysis Pages**:
    - **Pollutant Trends**: Visualises changes in pollutant levels over time.
    - **Safety Compliance**: Displays alerts for pollutant levels that exceed recommended limits.
    - **Geographical Hotspots**: Provides a map view or heat map of sampling points.
    - **Summary Statistics**: Calculates and displays averages, maximums, minimums, and other summary data.

## Key Features
- **Internationalisation**: The app supports language changes based on system locale settings.
- **Modular UI**: Using a "card" approach, each data type or control is grouped logically within the interface.
- **Tooltips**: Hovering over elements displays additional context, improving usability.

## Design Choices
- **Model/View Architecture**: Chosen for flexibility in data handling and to separate data presentation from underlying storage.
- **Data Visualisation**: Includes basic charts and compliance indicators to ensure intuitive data interpretation.
- **"Card" Layout**: The UI is modularised into “cards,” each focused on a specific data group or control area.